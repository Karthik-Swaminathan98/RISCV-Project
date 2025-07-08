#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_complex_math.h"

#define N 256               // Number of samples
#define SAMPLING_FREQ 256   // Sampling frequency in Hz
#define SINE_FREQ 50        // Frequency of the sine wave in Hz
#define M 8                 // Log2(N), since N = 256, M = 8
#define M_PI 3.14159265358979323846
#define Q15_SCALE (32768)   // Scaling factor for Q15 format

// Define stack-related parameters
const uint32_t stack_top = 0x000A0000;       // Explicitly defined stack top
const uint32_t stack_limit = 0x0009F000;    // Explicitly defined stack limit
const uint32_t STACK_SIZE = 4096;           // Explicitly defined stack size (bytes)

// Global variables for stack tracking
//uint32_t stack_top = STACK_TOP;
//uint32_t stack_limit = STACK_LIMIT;

q15_t input[2 * N];
q15_t magnitude[N];
float32_t frequency_bin[N];

// Fill stack pattern
void fill_stack_pattern_to_sp() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));
    uint32_t *p = (uint32_t*)stack_limit;
    while (p < sp) {
        *p++ = 0xAAAAAAAA;
    }
}

// Measure stack usage
uint32_t measure_stack_usage() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));
    uint32_t *p = (uint32_t*)stack_limit;
    while (p < sp) {
        if (*p != 0xAAAAAAAA) break;
        p++;
    }
    return ((uint32_t)sp - (uint32_t)p);
}

// Reset performance counters
void reset_counters() {
    write_csr(NDS_MCYCLE, 0);
    write_csr(NDS_MINSTRET, 0);
}

// Read both cycle and instruction counts
void read_perf_counters(unsigned int *cycles, unsigned int *instructions) {
    *cycles = read_csr(NDS_MCYCLE);
    *instructions = read_csr(NDS_MINSTRET);
}

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    if (stack_limit >= stack_top) {
        printf("Error: Invalid stack limit.\n");
        return -1;
    }

    // Generate 50Hz sine wave
    printf("Input Sine Wave Values:\n");
    for (int i = 0; i < N; i++) {
        input[2 * i] = (q15_t)(sinf(2 * M_PI * SINE_FREQ * i / SAMPLING_FREQ) * Q15_SCALE);
        input[2 * i + 1] = 0;
    }
    delay_ms(10);
    printf("Sine Wave generated\n");
    printf("START DSP PROCESS\n");
    delay_ms(10);

    fill_stack_pattern_to_sp();
    reset_counters();

    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_dsp_cfft_q15(input, M);

    read_perf_counters(&end_cycles, &end_inst);
    uint32_t stack_usage = measure_stack_usage();

    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t inst_count = end_inst - start_inst;


    // Compute magnitudes
    riscv_dsp_cmag_q15(input, magnitude, N);

//    for (int i = 0; i < N; i++) {
//        frequency_bin[i] = i * ((float32_t)SAMPLING_FREQ / N);
//        printf("%.2f\t%.2f\n\r", frequency_bin[i], magnitude[i] / (float32_t)Q15_SCALE);
//        delay_ms(10);
//    }

    printf("Cycle Count for riscv_dsp_cfft_q15(): %lu\n\r", cycle_count);
    printf("Instruction Count for riscv_dsp_cfft_q15(): %lu\n\r", inst_count);
    printf("Stack Usage: %lu bytes\n\r", stack_usage);
    printf("Stack Top:    0x%08lX\n\r", stack_top);
    printf("Stack Limit:  0x%08lX\n\r", stack_limit);
    printf("Stack Size:   %lu bytes\n\r", STACK_SIZE);

    return 0;
}
