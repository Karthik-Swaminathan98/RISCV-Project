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
#define STACK_SIZE 0x1000   // Define stack size as 4096 bytes

extern uint32_t _STACK_TOP;
uint32_t __StackLimit;       // To store the calculated stack limit

// Array of FFT sizes to test
const int FFT_SIZES[FFT_SIZES_COUNT] = {32, 64, 128, 256, 512, 1024};

void generate_sine_wave_q15(q15_t* input, int N, float signal_freq, float sampling_freq) {
    for (int i = 0; i < N; i++) {
        float value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        input[2 * i] = (q15_t)(value * Q15_SCALE);  // Real part (scaled to Q15 format)
        input[2 * i + 1] = 0;                     // Imaginary part (set to 0)
    }
}

// Function to fill stack pattern
void fill_stack_pattern_to_sp() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));  // Get current stack pointer

    uint32_t *p = (uint32_t*)__StackLimit;      // Start from stack limit
    while (p < sp) {
        *p++ = 0xAAAAAAAA;  // Fill stack memory with the pattern
    }
}

// Function to measure stack usage
uint32_t measure_stack_usage() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));  // Get current stack pointer

    uint32_t *p = (uint32_t*)__StackLimit;      // Start from stack limit
    while (p < sp) {
        if (*p != 0xAAAAAAAA) {                 // Stop at the first non-pattern value
            break;
        }
        p++;
    }

    return ((uint32_t)sp - (uint32_t)p);        // Stack usage in bytes
}

// Functions to reset and read cycle counts (RISC-V processor)
void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

unsigned int read_cycle_count() {
    return read_csr(NDS_MCYCLE);
}

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    // Get stack top and calculate stack limit
    uint32_t stack_top = (uint32_t)&_STACK_TOP;  // Get stack top from linker script
    __StackLimit = stack_top - STACK_SIZE;      // Calculate stack limit based on 4096-byte stack size

    // Validate stack_top and __StackLimit
    if (__StackLimit >= stack_top) {
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

		// Fill stack with a known pattern
		fill_stack_pattern_to_sp();

        // Reset cycle count
        reset_cycle_count();

        // Start cycle count
        unsigned int start_cycles = read_cycle_count();

        // Perform FFT using Q15
        riscv_cfft_q15(&fft_instance, input, 0, 1);

        // End cycle count
        unsigned int end_cycles = read_cycle_count();

        // Measure stack usage
        uint32_t stack_used = measure_stack_usage();

        // Calculate the total cycle count
        unsigned int cycle_count = end_cycles - start_cycles;

        // Calculate the magnitude of the FFT output using riscv_cmplx_mag_q15()
        riscv_cmplx_mag_q15(input, output, N);

        // Print results
		printf("  Stack Used: %lu bytes\n\r", (unsigned long)stack_used);
		printf("  Cycle Count: %lu\n\r", (unsigned long)cycle_count);
		printf("\n");

		free(input);
		free(output);
	}

    return 0;
}
