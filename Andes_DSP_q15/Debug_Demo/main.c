#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_complex_math.h"

#define N 1024               // Number of samples
#define SAMPLING_FREQ 256   // Sampling frequency in Hz
#define SINE_FREQ 50        // Frequency of the sine wave in Hz
#define M 10                 // Log2(N), since N = 512, M = 9
#define M_PI 3.14159265358979323846
#define Q15_SCALE (32768)   // Scaling factor for Q15 format

// Allocate input array (real + imaginary parts interleaved)
q15_t input[2 * N];          // Interleaved input (real + imag)
q15_t magnitude[N];      // Magnitude array
float32_t frequency_bin[N];  // Frequency bin array

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

    // Generate a 50Hz sine wave signal in Q15 format and print input values
    printf("Input Sine Wave Values:\n");
    for (int i = 0; i < N; i++) {
        input[2 * i] = (q15_t)(sinf(2 * M_PI * SINE_FREQ * i / SAMPLING_FREQ) * Q15_SCALE); // Real part in Q15 format
        input[2 * i + 1] = 0; // Imaginary part (zero)
    }
    delay_ms(10);
    printf("Sine Wave generated\n");
    printf("START DSP PROCESS\n");
    delay_ms(10);

    // Reset cycle count
    reset_cycle_count();

    // Start cycle count
    unsigned int start_cycles = read_cycle_count();

    // Perform FFT using the Andes DSP library
    riscv_dsp_cfft_q15(input, M);

    // End cycle count
    unsigned int end_cycles = read_cycle_count();

    // Calculate the total cycle count
    unsigned int cycle_count = end_cycles - start_cycles;

    riscv_dsp_cmag_q15(input, magnitude, N);

    // Calculate magnitude and frequency bins
    for (int i = 0; i < N; i++) {
        frequency_bin[i] = i * ((float32_t)SAMPLING_FREQ / N);
        printf("%.2f\t%.2f\n", frequency_bin[i], magnitude[i]/ (float32_t)Q15_SCALE);
        delay_ms(10);
    }

    // Print results: cycle count
    printf("Cycle Count for D25F: %u\n", cycle_count);

    return 0;
}
