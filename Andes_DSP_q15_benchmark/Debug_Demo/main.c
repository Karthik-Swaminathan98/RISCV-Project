#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_complex_math.h"

#define N 128               // Number of samples
#define SAMPLING_FREQ 256   // Sampling frequency in Hz
#define SINE_FREQ 50        // Frequency of the sine wave in Hz
#define M 7                 // Log2(N), since N = 512, M = 9
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

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    // Allocate input array (real + imaginary parts interleaved)
    q15_t input[2 * N];               // Interleaved input (real + imag)
    q15_t original_input[2 * N];      // Original input to be restored before each execution
    q15_t magnitude[NUM_EXECUTIONS][N]; // Magnitude array for each execution (in Q15 format)
    float32_t frequency_bin[N];       // Frequency bin array
    unsigned int cycle_counts[NUM_EXECUTIONS]; // Array to store cycle counts
    q15_t magnitude_temp[N];          // Temporary array to store magnitude results

    // Generate a 50Hz sine wave signal in Q15 format
    for (int i = 0; i < N; i++) {
            original_input[2 * i] = (q15_t)(sinf(2 * M_PI * SINE_FREQ * i / SAMPLING_FREQ) * Q15_SCALE); // Real part in Q15 format
            original_input[2 * i + 1] = 0; // Imaginary part (zero)
        }

    // Execute the FFT process ten times
    for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
    	// Copy original input to working input array
		for (int i = 0; i < 2 * N; i++) {
			input[i] = original_input[i];
		}
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

        // Calculate the magnitude for the current FFT result using riscv_dsp_cmag_q15 function
        riscv_dsp_cmag_q15(input, magnitude_temp, N);
        for (int i = 0; i < N; i++) {
            magnitude[execution][i] = magnitude_temp[i];
        }
    }

    // Calculate frequency bins
    for (int i = 0; i < N; i++) {
        frequency_bin[i] = i * ((float32_t)SAMPLING_FREQ / N);
    }

    // Display cycle counts for each execution
    printf("Cycle Counts for each FFT execution:\n");
    unsigned int total_cycle_count = 0;
    for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
        printf("Execution %d: %u cycles\n", execution + 1, cycle_counts[execution]);
        total_cycle_count += cycle_counts[execution];
        delay_ms(30);
    }
    delay_ms(30);
    // Calculate and display the average cycle count
    unsigned int average_cycle_count = total_cycle_count / NUM_EXECUTIONS;
    printf("Average Cycle Count: %u\n", average_cycle_count);
    delay_ms(30);

    // Check consistency of FFT outputs
    printf("Checking consistency of FFT outputs:\n");

    int consistent = 1;
    for (int i = 1; i < NUM_EXECUTIONS; i++) {
        for (int j = 0; j < N; j++) {
            if (abs(magnitude[0][j] - magnitude[i][j]) > 1) { // Tolerance of 1 for Q15 format
                consistent = 0;
                break;
            }
        }
        if (!consistent) {
            printf("Inconsistent FFT output detected at execution %d\n", i + 1);
            delay_ms(30);
            break;
        }
    }

    if (consistent) {
        printf("FFT outputs are consistent across all executions.\n");
        delay_ms(30);
    }

    // Print the final magnitude and frequency bins
    delay_ms(30);
    printf("Final Magnitude and Frequency Bins:\n");
    printf("Frequency\tMagnitude1\tMagnitude2\tMagnitude3\t...\tMagnitude10\n");
    delay_ms(30);
        for (int i = 0; i < N; i++) {
            printf("%.2f", frequency_bin[i]);
            for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
                printf("\t%.2f", magnitude[execution][i] / (float32_t)Q15_SCALE);
            }
            printf("\n");
            delay_ms(30);
        }
    return 0;
}
