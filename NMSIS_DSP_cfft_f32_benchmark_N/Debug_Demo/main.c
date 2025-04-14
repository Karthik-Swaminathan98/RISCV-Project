#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include "riscv_math.h"

#define SAMPLING_FREQ  256   // Sampling frequency in Hz
#define SIGNAL_FREQ    50    // Frequency of the sine wave in Hz
#define M_PI           3.14159265358979323846
#define NUM_EXECUTIONS 10    // Number of FFT executions for benchmarking

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

    // Array of different FFT sizes to test
    const int FFT_SIZES[] = {1024, 512, 256, 128, 64, 32};
    const int num_fft_sizes = sizeof(FFT_SIZES) / sizeof(FFT_SIZES[0]);

    // Loop over each FFT size
    for (int fft_idx = 0; fft_idx < num_fft_sizes; fft_idx++) {
        int FFT_SIZE = FFT_SIZES[fft_idx];

        // Allocate memory for input and output arrays dynamically based on FFT_SIZE
        float32_t* input = (float32_t*)malloc(2 * FFT_SIZE * sizeof(float32_t));           // Interleaved input (real + imag)
        float32_t* original_input = (float32_t*)malloc(2 * FFT_SIZE * sizeof(float32_t));  // Original input
        float32_t* magnitude_results = (float32_t*)malloc(FFT_SIZE * sizeof(float32_t));   // Magnitude results
        float32_t* magnitude_reference = (float32_t*)malloc(FFT_SIZE * sizeof(float32_t)); // Reference magnitude result
        unsigned int cycle_counts[NUM_EXECUTIONS];                                        // Array to store cycle counts

        if (input == NULL || original_input == NULL || magnitude_results == NULL || magnitude_reference == NULL) {
            printf("Memory allocation failed for FFT_SIZE = %d\n", FFT_SIZE);
            delay_ms(10);
            return -1;
        }

        // Generate a sine wave in F32 format
        for (int i = 0; i < FFT_SIZE; i++) {
            original_input[2 * i] = sinf(2 * M_PI * SIGNAL_FREQ * i / SAMPLING_FREQ); // Real part
            original_input[2 * i + 1] = 0; // Imaginary part
        }

        riscv_cfft_instance_f32 fft_instance;
        riscv_cfft_init_f32(&fft_instance, FFT_SIZE);

        printf("Testing FFT for FFT_SIZE = %d\n", FFT_SIZE);
        delay_ms(10);

        int consistent_magnitude = 1;

        // Execute the FFT process NUM_EXECUTIONS times
        for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
            // Copy the original input into the working input array
            memcpy(input, original_input, 2 * FFT_SIZE * sizeof(float32_t));

            // Reset cycle count
            reset_cycle_count();

            // Start cycle count
            unsigned int start_cycles = read_cycle_count();

            // Perform FFT using NMSIS DSP library
            riscv_cfft_f32(&fft_instance, input, 0, 1);

            // End cycle count
            unsigned int end_cycles = read_cycle_count();

            // Record the cycle count for this execution
            cycle_counts[execution] = end_cycles - start_cycles;

            // Calculate the magnitude of the FFT output
            riscv_cmplx_mag_f32(input, magnitude_results, FFT_SIZE);

            // For the first iteration, store the reference magnitude
            if (execution == 0) {
                memcpy(magnitude_reference, magnitude_results, FFT_SIZE * sizeof(float32_t));
            } else {
                // Compare current magnitude with the reference magnitude
                for (int i = 0; i < FFT_SIZE; i++) {
                    if (fabsf(magnitude_results[i] - magnitude_reference[i]) > 1e-5) { // Allowable tolerance for F32 format
                        consistent_magnitude = 0;
                        break;
                    }
                }
            }

            // If inconsistency is detected, stop further checks
            if (!consistent_magnitude) {
                printf("Inconsistent magnitude detected at execution %d for FFT_SIZE = %d\n", execution + 1, FFT_SIZE);
                delay_ms(10);
                break;
            }
        }

        if (consistent_magnitude) {
            printf("Magnitude results are consistent for FFT_SIZE = %d.\n", FFT_SIZE);
            delay_ms(10);
        }

        // Calculate and display the cycle counts and the average
        printf("Cycle Counts for each FFT execution (FFT_SIZE = %d):\n", FFT_SIZE);
        delay_ms(10);
        unsigned int total_cycle_count = 0;
        for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
            printf("Execution %d: %u cycles\n", execution + 1, cycle_counts[execution]);
            delay_ms(10);
            total_cycle_count += cycle_counts[execution];
        }
        unsigned int average_cycle_count = total_cycle_count / NUM_EXECUTIONS;
        printf("Average Cycle Count (FFT_SIZE = %d): %u\n", FFT_SIZE, average_cycle_count);
        delay_ms(10);
        printf("\n");

        // Free dynamically allocated memory
        free(input);
        free(original_input);
        free(magnitude_results);
        free(magnitude_reference);
    }

    return 0;
}
