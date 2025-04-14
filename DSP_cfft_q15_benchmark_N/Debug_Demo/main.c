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
#define Q15_SCALE (32768)   // Scaling factor for Q15 format
#define NUM_EXECUTIONS 10   // Number of FFT executions

// Function to reset cycle count (RISC-V processor)
void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

// Function to read cycle count (32-bit)
unsigned int read_cycle_count() {
    return read_csr(NDS_MCYCLE);
}

void generate_sine_wave_q15(q15_t* input, int N, float signal_freq, float sampling_freq) {
    for (int i = 0; i < N; i++) {
        // Generate sine wave in float format
        float32_t value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        // Convert to Q15 format
        input[2 * i] = (q15_t)(value * Q15_SCALE); // Real part
        input[2 * i + 1] = 0;                     // Imaginary part (set to 0)
    }
}

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

	printf("****************** \n");
	printf("Starting FFT Benchmark Program\n");
    // Array of different N values to test
    const int N_values[] = {1024, 512, 256, 128, 64, 32};
    const int num_N_values = sizeof(N_values) / sizeof(N_values[0]);

    // Loop over each N value
    for (int n_idx = 0; n_idx < num_N_values; n_idx++) {
        int N = N_values[n_idx];
        int M = (int)(log2(N)); // Calculate M = log2(N)

        // Allocate input arrays dynamically based on N
        q15_t* input = (q15_t*)malloc(2 * N * sizeof(q15_t));               // Interleaved input (real + imag)
        q15_t* original_input = (q15_t*)malloc(2 * N * sizeof(q15_t));      // Original input
        q15_t* magnitude_results = (q15_t*)malloc(N * sizeof(q15_t));       // Temporary magnitude results
        q15_t* magnitude_reference = (q15_t*)malloc(N * sizeof(q15_t));     // Reference magnitude result
        unsigned int cycle_counts[NUM_EXECUTIONS];                          // Array to store cycle counts

        if (input == NULL || original_input == NULL || magnitude_results == NULL || magnitude_reference == NULL) {
            printf("Memory allocation failed for N = %d\n", N);
            delay_ms(30);
            return -1;
        }

        // Generate a sine wave signal in Q15 format
		generate_sine_wave_q15(original_input, N, SINE_FREQ, SAMPLING_FREQ);

        printf("Testing FFT for N = %d\n", N);
        delay_ms(30);

        int consistent_magnitude = 1;

        // Execute the FFT process ten times
        for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
            // Use memcpy to copy original input to working input array
            memcpy(input, original_input, 2 * N * sizeof(q15_t));

            // Reset cycle count
            reset_cycle_count();

            // Start cycle count
            unsigned int start_cycles = read_cycle_count();

            // Perform FFT using the Andes DSP library
            riscv_dsp_cfft_q15(input, M);

            // End cycle count
            unsigned int end_cycles = read_cycle_count();

            // Record the cycle count for this execution
            cycle_counts[execution] = end_cycles - start_cycles;

            // Calculate the magnitude for the current FFT result
            riscv_dsp_cmag_q15(input, magnitude_results, N);

            // For the first iteration, store the reference magnitude
            if (execution == 0) {
                memcpy(magnitude_reference, magnitude_results, N * sizeof(q15_t));
            } else {
                // Compare current magnitude with the reference magnitude
                for (int i = 0; i < N; i++) {
                    if (abs(magnitude_results[i] - magnitude_reference[i]) > 1) { // Tolerance of 1 for Q15 format
                        consistent_magnitude = 0;
                        break;
                    }
                }
            }
        }

//        for (int i = 0; i < N; i++) {
//			float frequency_bin = i * ((float32_t)SAMPLING_FREQ / N);
//			printf("%.2f", frequency_bin);
//			delay_ms(10);
//			for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
//				printf("\t%.2f", magnitude_results[i] / (float32_t)Q15_SCALE);
//				delay_ms(10);
//			}
//			printf("\n");
//			delay_ms(10);
//		}

		if (consistent_magnitude) {
			printf("Magnitude results are consistent for FFT_SIZE = %d.\n", N);
			delay_ms(30);
		}else {
			printf("Inconsistent magnitudes detected for FFT size N = %d\n", N);
			delay_ms(30);
		}

        // Calculate and display the cycle counts and average
        printf("Cycle Counts for each FFT execution (N = %d):\n", N);
        delay_ms(30);
        unsigned int total_cycle_count = 0;
        for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
            printf("Execution %d: %u cycles\n", execution + 1, cycle_counts[execution]);
            delay_ms(30);
            total_cycle_count += cycle_counts[execution];
        }
        unsigned int average_cycle_count = total_cycle_count / NUM_EXECUTIONS;
        printf("Average Cycle Count (N = %d): %u\n", N, average_cycle_count);
        delay_ms(30);
        printf("\n");

        // Free dynamically allocated memory
        free(input);
        free(original_input);
        free(magnitude_results);
        free(magnitude_reference);
    }

    return 0;
}
