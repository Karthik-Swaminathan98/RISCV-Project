#include <stdio.h>
#include <math.h>
#include "riscv_math.h"

#define FFT_SIZE       512
#define SAMPLING_FREQ  256.0f
#define SIGNAL_FREQ    50.0f
#define M_PI           3.14159265358979323846

void generate_sine_wave_q15(q15_t *input, uint32_t signal_length, float32_t signal_freq, float32_t sampling_freq) {
    for (uint32_t i = 0; i < signal_length; i++) {
        float32_t sine_value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        input[2 * i] = (q15_t)(sine_value * 32767); // Real part (scaled to Q15)
        input[2 * i + 1] = 0;                      // Imaginary part (set to 0)
    }
}
// Function to read cycle count (assembly for a RISC-V processor)
uint64_t read_cycle_count() {
    uint64_t cycles;
    asm volatile("rdcycle %0" : "=r"(cycles)); // Read cycle counter
    return cycles;
}

int main(void) {

    // Input data array (complex, interleaved format)
    q15_t input[2 * FFT_SIZE];
    // Output magnitude array
    float32_t magnitude[FFT_SIZE];
    float32_t frequency_bin[FFT_SIZE]; // Frequency bin array

    // Generate the input signal (50 Hz sine wave)
    generate_sine_wave_q15(input, FFT_SIZE, SIGNAL_FREQ, SAMPLING_FREQ);

    printf("Q15 Sine Wave Generated\n");

    // Initialize FFT instance
    riscv_cfft_instance_q15 fft_instance;
    riscv_cfft_init_q15(&fft_instance, FFT_SIZE);

    printf("START DSP PROCESS\n");

    // Start cycle count
    unsigned int start_cycles = read_cycle_count();

    // Perform FFT using Q15
    riscv_cfft_q15(&fft_instance, input, 0, 1);

    // End cycle count
    unsigned int end_cycles = read_cycle_count();

    // Calculate the total cycle count
    unsigned int cycle_count = end_cycles - start_cycles;

    // Calculate magnitude and frequency bins
    for (int i = 0; i < FFT_SIZE; i++) {
        q15_t real = input[2 * i];
        q15_t imag = input[2 * i + 1];
        magnitude[i] = sqrtf((real / 32768.0f) * (real / 32768.0f) + (imag / 32768.0f) * (imag / 32768.0f));
        frequency_bin[i] = i * ((float32_t)SAMPLING_FREQ / FFT_SIZE);
        printf("%.2f\t%.2f\n", frequency_bin[i], magnitude[i]);
    }

    // Print results: cycle count
    printf("Cycle Count for Q15 FFT: %u\n", cycle_count);

    return 0;
}
