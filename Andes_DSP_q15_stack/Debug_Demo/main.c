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
#define STACK_SIZE 0x1000   // Define stack size as 4096 bytes

extern uint32_t _STACK_TOP;
uint32_t __StackLimit;       // To store the calculated stack limit

// Allocate input array (real + imaginary parts interleaved)
q15_t input[2 * N];          // Interleaved input (real + imag)
q15_t magnitude[N];          // Magnitude array
float32_t frequency_bin[N];  // Frequency bin array

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

    // Generate a 50Hz sine wave signal in Q15 format
    printf("Input Sine Wave Values:\n");
    for (int i = 0; i < N; i++) {
        input[2 * i] = (q15_t)(sinf(2 * M_PI * SINE_FREQ * i / SAMPLING_FREQ) * Q15_SCALE); // Real part in Q15 format
        input[2 * i + 1] = 0; // Imaginary part (zero)
    }
    delay_ms(10);
    printf("Sine Wave generated\n");
    printf("START DSP PROCESS\n");
    delay_ms(10);

    // Fill the stack with a pattern
    fill_stack_pattern_to_sp();

    // Reset cycle count
    reset_cycle_count();

    // Start cycle count
    unsigned int start_cycles = read_cycle_count();

    // Perform FFT using the Andes DSP library
    riscv_dsp_cfft_q15(input, M);

    // End cycle count
    unsigned int end_cycles = read_cycle_count();

    // Measure stack usage
    uint32_t stack_usage = measure_stack_usage();

    // Calculate the total cycle count
    unsigned int cycle_count = end_cycles - start_cycles;

    // Perform magnitude calculation
    riscv_dsp_cmag_q15(input, magnitude, N);

    // Calculate magnitude and frequency bins
    for (int i = 0; i < N; i++) {
        frequency_bin[i] = i * ((float32_t)SAMPLING_FREQ / N);
        printf("%.2f\t%.2f\n\r", frequency_bin[i], magnitude[i]/ (float32_t)Q15_SCALE);
        delay_ms(10);
    }

    // Print results
    printf("Cycle Count for D25F: %u\n\r", cycle_count);
    printf("Stack Usage by riscv_dsp_cfft_q15(): %lu bytes\n\r", stack_usage);
    uint32_t stack_size = stack_top - __StackLimit;
    printf("Stack Top:    0x%08lX\n\r", stack_top);
    printf("Stack Limit:  0x%08lX\n\r", __StackLimit);
    printf("Stack Size:   %lu bytes\n\r", stack_size);

    return 0;
}
