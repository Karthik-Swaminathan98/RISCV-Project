#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_complex_math.h"

#define SAMPLING_FREQ 256   // Hz
#define SINE_FREQ 50        // Hz
#define M_PI 3.14159265358979323846
#define Q15_SCALE (32768)   // For Q15 conversion
#define STACK_SIZE 0x1000   // 4096 bytes stack
//#define MCYCLE_FREQ_HZ 192000000UL  // mcycle clock frequency (adjust based on system)

extern uint32_t _STACK_TOP;
uint32_t __StackLimit;

// Fill pattern into stack
void fill_stack_pattern_to_sp() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));
    uint32_t *p = (uint32_t*)__StackLimit;
    while (p < sp) {
        *p++ = 0xAAAAAAAA;
    }
}

// Measure used stack
uint32_t measure_stack_usage() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));
    uint32_t *p = (uint32_t*)__StackLimit;
    while (p < sp) {
        if (*p != 0xAAAAAAAA) break;
        p++;
    }
    return ((uint32_t)sp - (uint32_t)p);
}

// Generate sine wave
void generate_sine_wave_q15(q15_t* input, int N, float signal_freq, float sampling_freq) {
    for (int i = 0; i < N; i++) {
        float32_t value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        input[2 * i] = (q15_t)(value * Q15_SCALE);
        input[2 * i + 1] = 0;
    }
}

// Reset and read mcycle
void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}
unsigned int read_cycle_count() {
    return read_csr(NDS_MCYCLE);
}

extern void user_init(void);

// Entry point
int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    uint32_t stack_top = (uint32_t)&_STACK_TOP;
    __StackLimit = stack_top - STACK_SIZE;

    if (__StackLimit >= stack_top) {
        printf("Error: Invalid stack range.\n");
        return -1;
    }

    const int fft_sizes[] = {32, 64, 128, 256, 512, 1024};
    const int num_fft_sizes = sizeof(fft_sizes) / sizeof(fft_sizes[0]);

    printf("Benchmarking FFT sizes...\n\r");
    printf("  CPU Clock  : %u MHz (CCLK)\n\r", sys_clk.cclk);
    //printf("  mcycle clock freq: %lu Hz\n\r", (unsigned long)MCYCLE_FREQ_HZ);

    for (int size_idx = 0; size_idx < num_fft_sizes; size_idx++) {
        int fft_size = fft_sizes[size_idx];
        int log2_fft_size = (int)(log2((double)fft_size));

        q15_t *input = (q15_t *)malloc(2 * fft_size * sizeof(q15_t));
        q15_t *magnitude = (q15_t *)malloc(fft_size * sizeof(q15_t));

        if (!input || !magnitude) {
            printf("Memory allocation failed for FFT size %d\n", fft_size);
            if (input) free(input);
            if (magnitude) free(magnitude);
            continue;
        }

        generate_sine_wave_q15(input, fft_size, SINE_FREQ, SAMPLING_FREQ);
        fill_stack_pattern_to_sp();

        reset_cycle_count();
        unsigned int start_cycles = read_cycle_count();

        riscv_dsp_cfft_q15(input, log2_fft_size);

        unsigned int end_cycles = read_cycle_count();
        unsigned int cycle_count = end_cycles - start_cycles;

        uint32_t stack_usage = measure_stack_usage();
//        float time_sec = (float)cycle_count / (float)sys_clk.cclk * 1e6f;
//        float time_us = time_sec * 1e6f;
        float mcycle_freq_hz = (float)sys_clk.cclk * 1e6f;  // Clock in Hz
        float time_sec = (float)cycle_count / mcycle_freq_hz;
        float time_us = time_sec * 1e6f;


        riscv_dsp_cmag_q15(input, magnitude, fft_size);

        printf("\nFFT Size: %d\n\r", fft_size);
        printf("Cycle Count for D25F      : %u cycles\n\r", cycle_count);
        printf("Execution Time (approx)   : %.2f us\n\r", time_us);
        printf("Stack Usage               : %lu bytes\n\r", stack_usage);

        free(input);
        free(magnitude);
    }

    printf("\nStack Top:    0x%08lX\n\r", stack_top);
    printf("Stack Limit:  0x%08lX\n\r", __StackLimit);
    printf("Stack Size:   %lu bytes\n\r", stack_top - __StackLimit);

    return 0;
}
