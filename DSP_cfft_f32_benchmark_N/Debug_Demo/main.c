#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_complex_math.h"

#define SAMPLING_FREQ 256   // Sampling frequency in Hz
#define SINE_FREQ 50        // Frequency of the sine wave in Hz
#define M_PI 3.14159265358979323846
#define NUM_EXECUTIONS 10   // Number of FFT executions

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

    // Array of different N values to test
    const int N_values[] = {1024, 512, 256, 128, 64, 32};
    const int num_N_values = sizeof(N_values) / sizeof(N_values[0]);

    // Loop over each N value
    for (int n_idx = 0; n_idx < num_N_values; n_idx++) {
        int N = N_values[n_idx];
        int M = (int)(log2(N)); // Calculate M = log2(N)

        // Allocate input arrays dynamically based on N
        float32_t* input = (float32_t*)malloc(2 * N * sizeof(float32_t));               // Interleaved input (real + imag)
        float32_t* original_input = (float32_t*)malloc(2 * N * sizeof(float32_t));      // Original input
        float32_t* magnitude_results = (float32_t*)malloc(N * sizeof(float32_t));       // Temporary magnitude results
        float32_t* magnitude_reference = (float32_t*)malloc(N * sizeof(float32_t));     // Reference magnitude result
        unsigned int cycle_counts[NUM_EXECUTIONS];                                      // Array to store cycle counts

        if (input == NULL || original_input == NULL || magnitude_results == NULL || magnitude_reference == NULL) {
            printf("Memory allocation failed for N = %d\n", N);
            delay_ms(10);
            return -1;
        }

        // Generate a sine wave signal in floating-point format
        for (int i = 0; i < N; i++) {
            original_input[2 * i] = sinf(2 * M_PI * SINE_FREQ * i / SAMPLING_FREQ); // Real part
            original_input[2 * i + 1] = 0; // Imaginary part (zero)
        }

        printf("Testing FFT for N = %d\n", N);
        delay_ms(10);

        int consistent_magnitude = 1;

        // Execute the FFT process ten times
        for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
            // Use memcpy to copy original input to working input array
            memcpy(input, original_input, 2 * N * sizeof(float32_t));

            // Reset cycle count
            reset_cycle_count();

            // Start cycle count
            unsigned int start_cycles = read_cycle_count();

            // Perform FFT using the Andes DSP library
            riscv_dsp_cfft_f32(input, M);

            // End cycle count
            unsigned int end_cycles = read_cycle_count();

            // Record the cycle count for this execution
            cycle_counts[execution] = end_cycles - start_cycles;

            // Calculate the magnitude for the current FFT result
            riscv_dsp_cmag_f32(input, magnitude_results, N);

            // For the first iteration, store the reference magnitude
            if (execution == 0) {
                memcpy(magnitude_reference, magnitude_results, N * sizeof(float32_t));
            } else {
                // Compare current magnitude with the reference magnitude
                for (int i = 0; i < N; i++) {
                    if (fabsf(magnitude_results[i] - magnitude_reference[i]) > 1e-6) { // Tolerance for floating-point values
                        consistent_magnitude = 0;
                        break;
                    }
                }
            }

            // If inconsistency is detected, stop checking further
            if (!consistent_magnitude) {
                printf("Inconsistent magnitude detected at execution %d for N = %d\n", execution + 1, N);
                delay_ms(10);
                break;
            }
        }

        if (consistent_magnitude) {
            printf("Magnitude results are consistent for N = %d.\n", N);
            delay_ms(10);
        }

        // Calculate and display the cycle counts and average
        printf("Cycle Counts for each FFT execution (N = %d):\n", N);
        delay_ms(10);
        unsigned int total_cycle_count = 0;
        for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
            printf("Execution %d: %u cycles\n", execution + 1, cycle_counts[execution]);
            delay_ms(10);
            total_cycle_count += cycle_counts[execution];
        }
        unsigned int average_cycle_count = total_cycle_count / NUM_EXECUTIONS;
        printf("Average Cycle Count (N = %d): %u\n", N, average_cycle_count);
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
