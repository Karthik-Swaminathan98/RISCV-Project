#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_complex_math.h"

#define SAMPLING_FREQ 256   // Sampling frequency in Hz
#define SINE_FREQ 50        // Frequency of the sine wave in Hz
#define M_PI 3.14159265358979323846
#define STACK_SIZE 0x1000   // Define stack size as 4096 bytes

extern uint32_t _STACK_TOP;
uint32_t __StackLimit;       // To store the calculated stack limit

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

// Function to generate a sine wave in F32 format
void generate_sine_wave_f32(float32_t* input, int N, float signal_freq, float sampling_freq) {
    for (int i = 0; i < N; i++) {
        float32_t value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        input[2 * i] = value;  // Real part
        input[2 * i + 1] = 0.0f;  // Imaginary part (set to 0)
    }
}

// Function to reset cycle count (RISC-V processor)
void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

// Function to read cycle count (32-bit)
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

    // FFT sizes to benchmark
    const int fft_sizes[] = {32, 64, 128, 256, 512, 1024};
    const int num_fft_sizes = sizeof(fft_sizes) / sizeof(fft_sizes[0]);

    // Benchmark for each FFT size
    printf("Benchmarking FFT sizes...\n\r");
    for (int size_idx = 0; size_idx < num_fft_sizes; size_idx++) {
        int fft_size = fft_sizes[size_idx];
        int log2_fft_size = (int)(log2((double)fft_size));

        // Allocate input arrays dynamically for the current FFT size
        float32_t *input = (float32_t *)malloc(2 * fft_size * sizeof(float32_t));  // Interleaved input (real + imag)
        float32_t *magnitude = (float32_t *)malloc(fft_size * sizeof(float32_t));  // Magnitude array

        if (!input || !magnitude) {
            printf("Memory allocation failed for FFT size %d\n", fft_size);
            if (input) free(input);
            if (magnitude) free(magnitude);
            continue;
        }

        // Generate a sine wave signal in F32 format
        generate_sine_wave_f32(input, fft_size, SINE_FREQ, SAMPLING_FREQ);

        // Fill the stack with a pattern
        fill_stack_pattern_to_sp();

        // Reset cycle count
        reset_cycle_count();

        // Start cycle count
        unsigned int start_cycles = read_cycle_count();

        // Perform FFT using the Andes DSP library
        riscv_dsp_cfft_f32(input, log2_fft_size);

        // End cycle count
        unsigned int end_cycles = read_cycle_count();

        // Measure stack usage
        uint32_t stack_usage = measure_stack_usage();

        // Calculate the total cycle count
        unsigned int cycle_count = end_cycles - start_cycles;

        // Perform magnitude calculation
        riscv_dsp_cmag_f32(input, magnitude, fft_size);

        // Print results for the current FFT size
        printf("\nFFT Size: %d\n\r", fft_size);
        printf("Cycle Count for D25F: %u\n\r", cycle_count);
        printf("Stack Usage: %lu bytes\n\r", stack_usage);

        // Free dynamically allocated memory
        free(input);
        free(magnitude);
    }

    // Print stack information
    uint32_t stack_size = stack_top - __StackLimit;
    printf("\nStack Top:    0x%08lX\n\r", stack_top);
    printf("Stack Limit:  0x%08lX\n\r", __StackLimit);
    printf("Stack Size:   %lu bytes\n\r", stack_size);

    return 0;
}
