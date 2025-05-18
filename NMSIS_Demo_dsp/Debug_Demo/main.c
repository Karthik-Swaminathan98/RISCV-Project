#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "riscv_math.h"
#include "nmsis_bench.h"

#define N       128             // Number of samples
#define SAMPLING_FREQ  256      // Sampling frequency in Hz
#define SIGNAL_FREQ    50       // Frequency of the sine wave in Hz
#define M_PI           3.14159265358979323846

// Input and result arrays
float32_t input[2 * N];              // FFT processing buffer
float32_t magnitude[N];              // Magnitude result
float32_t frequency_bin[N];          // Frequency bin array
unsigned int cycle_count = 0;        // Cycle count for single FFT

extern void user_init(void);

// Benchmark variable declaration
BENCH_DECLARE_VAR();

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);
    printf("\r\nNuclei RISC-V NMSIS-DSP Library Demonstration\r\n");
    delay_ms(20);
    #if (defined(__RISCV_FEATURE_DSP) && __RISCV_FEATURE_DSP == 1) || \
        (defined(__RISCV_FEATURE_VECTOR) && __RISCV_FEATURE_VECTOR == 1)
        printf("Using Nuclei RISC-V accelerated and optimized NMSIS-DSP library!\r\n");
        delay_ms(20);
    #if (defined(__RISCV_FEATURE_DSP) && __RISCV_FEATURE_DSP == 1)
        printf("Warning: Make sure Nuclei RISC-V DSP(P-ext) is present in your CPU core!\r\n");
        delay_ms(20);
    #endif
    #if (defined(__RISCV_FEATURE_VECTOR) && __RISCV_FEATURE_VECTOR == 1)
        printf("Warning: Make sure Nuclei RISC-V Vector is present in your CPU core!\r\n");
        delay_ms(20);
        // Enable Vector Unit
        __enable_vector();
    #endif
        printf("         Otherwise this example will trap to cpu core exception!\r\n\r\n");
        delay_ms(20);
    #else
        printf("Using c-only optimized NMSIS-DSP library!\r\n");
        delay_ms(20);
    #endif

    printf("Starting Single Execution FFT Benchmark\n");
    delay_ms(20);

    // Generate a sine wave signal
    for (int i = 0; i < N; i++) {
    	input[2 * i] = sinf(2 * M_PI * SIGNAL_FREQ * i / SAMPLING_FREQ);
    	input[2 * i + 1] = 0.0f; // Imaginary part
    }
    printf("F32 Sine Wave Generated\n");
    delay_ms(20);

    // Initialize FFT instance
    riscv_cfft_instance_f32 fft_instance;
    riscv_cfft_init_f32(&fft_instance, N);

    // Initialize benchmarking
    BENCH_INIT();
    BENCH_RESET(FFT);

    printf("Running FFT...\n");
    delay_ms(20);

    // Benchmark the FFT
    BENCH_START(FFT);
    riscv_cfft_f32(&fft_instance, input, 0, 1);
    BENCH_END(FFT);

    // Get cycle count
    cycle_count = (unsigned int)BENCH_GET_USECYC();
    printf("FFT executed in %u cycles.\n", cycle_count);
    delay_ms(30);

    // Compute magnitude
    riscv_cmplx_mag_f32(input, magnitude, N);

    // Generate frequency bins
    for (int i = 0; i < N; i++) {
        frequency_bin[i] = i * ((float32_t)SAMPLING_FREQ / N);
    }

    // Print results
    printf("Frequency (Hz)\tMagnitude\n");
    for (int i = 0; i < N; i++) {
        printf("%.2f\t\t%.5f\n", frequency_bin[i], magnitude[i]);
        delay_ms(10);
    }
    BENCH_STATUS(FFT);
    return 0;
}
