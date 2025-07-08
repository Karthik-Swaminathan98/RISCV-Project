#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include "riscv_math.h"

#define SAMPLING_FREQ  256              // Sampling frequency in Hz
#define SINE_FREQ    50               // Frequency of the sine wave in Hz
#define M_PI           3.14159265358979323846
#define Q15_SCALE      (32768)          // Scale factor for Q15 format
#define FFT_SIZES_COUNT 6      // Total number of FFT sizes to test

#define STACK_SIZE 0x1000
extern uint32_t _STACK_TOP;
uint32_t stack_limit;
uint32_t stack_top;


// Array of FFT sizes to test
const int FFT_SIZES[FFT_SIZES_COUNT] = {32, 64, 128, 256, 512, 1024};

void generate_sine_wave_q15(q15_t* input, int N, float signal_freq, float sampling_freq) {
    for (int i = 0; i < N; i++) {
        float value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        input[2 * i] = (q15_t)(value * Q15_SCALE);  // Real part (scaled to Q15 format)
        input[2 * i + 1] = 0;                     // Imaginary part (set to 0)
    }
}

static void fill_stack_pattern_to_sp() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));

    uint32_t *p = (uint32_t*)stack_limit;
    while (p < sp) {
        *p++ = 0xAAAAAAAA;
    }
}

static uint32_t measure_stack_usage() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));

    uint32_t *p = (uint32_t*)stack_limit;
    while (p < sp) {
        if (*p != 0xAAAAAAAA) {
            break;
        }
        p++;
    }

    return ((uint32_t)sp - (uint32_t)p);
}

// Reset performance counters
static void reset_counters() {
    write_csr(NDS_MCYCLE, 0);
    write_csr(NDS_MINSTRET, 0);
}

// Read both cycle and instruction counts
static void read_perf_counters(unsigned int *cycles, unsigned int *instructions) {
    *cycles = read_csr(NDS_MCYCLE);
    *instructions = read_csr(NDS_MINSTRET);
}

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    core_interrupt_disable();

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;
    uint32_t clkFastfreq = sys_clk.cclk * 1e6;
    printf("\n\r");
    printf("CPU Clock Frequency: %lu Hz\n\r", clkFastfreq);

    // Validate stack_top and __StackLimit
    if (stack_limit >= stack_top) {
        printf("Error: Invalid stack limit calculation. Check linker script and STACK_SIZE.\n");
        return -1;
    }


    for (int size_idx = 0; size_idx < FFT_SIZES_COUNT; size_idx++) {
		int N = FFT_SIZES[size_idx];

		// Allocate memory
		q15_t* input = (q15_t*)malloc(2 * N * sizeof(q15_t));
		q15_t* output = (q15_t*)malloc(2 * N * sizeof(q15_t)); // Output buffer for magnitudes

		if (input == NULL || output == NULL) {
			printf("Memory allocation failed for FFT size N = %d\n\r", N);
			return -1;
		}

		// Generate 50 Hz sine wave
		generate_sine_wave_q15(input, N, SINE_FREQ, SAMPLING_FREQ);

		// Initialize FFT instance
		riscv_cfft_instance_q15 fft_instance;
		riscv_cfft_init_q15(&fft_instance, N);

		printf("FFT Size: %d\n\r", N);

        reset_counters();
        fill_stack_pattern_to_sp();
        unsigned int start_cycles, start_inst, end_cycles, end_inst;
        read_perf_counters(&start_cycles, &start_inst);

        // Perform FFT using Q15
        riscv_cfft_q15(&fft_instance, input, 0, 1);

        read_perf_counters(&end_cycles, &end_inst);
        uint32_t cycle_count = end_cycles - start_cycles;
        uint32_t inst_count = end_inst - start_inst;
        uint32_t stack_used = measure_stack_usage();
        float time_sec = (float)cycle_count / clkFastfreq;
        float time_us = time_sec * 1e6f;

        // Calculate the magnitude of the FFT output using riscv_cmplx_mag_q15()
        riscv_cmplx_mag_q15(input, output, N);

        // Print results
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", inst_count);
        printf("Execution Time (approx)     : %.3f us (%.6f s)\n\r", time_us, time_sec);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
		printf("\n");

		free(input);
		free(output);
	}

    return 0;
}
