/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for B91m
 *
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************************************/
#include "app_config.h"
#include "core.h" // Include core.h to use rdmcycle
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

// Allocate input array (real + imaginary parts interleaved)
float32_t input[2 * N];   // Interleaved input (real + imag)
float32_t magnitude[N];   // Magnitude array
float32_t frequency_bin[N]; // Frequency bin array

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

    printf("Hello Karthik!\n");

    // Toggle LEDs
    gpio_toggle(LED1);
    gpio_toggle(LED2);
    gpio_toggle(LED3);
    gpio_toggle(LED4);
    delay_ms(500);

    // Reset cycle count
    reset_cycle_count();

    // Generate a 50Hz sine wave signal
    for (int i = 0; i < N; i++) {
        input[2 * i] = sinf(2 * M_PI * SINE_FREQ * i / SAMPLING_FREQ); // Real part
        input[2 * i + 1] = 0.0f; // Imaginary part (zero)
    }
    printf("Sine Wave generated\n");
    printf("START DSP PROCESS\n");

    // Start cycle count
    unsigned int start_cycles = read_cycle_count();

    // Perform FFT using the Andes DSP library (in-place transform)
    riscv_dsp_cfft_f32(input, M);

    // End cycle count
    unsigned int end_cycles = read_cycle_count();

    // Calculate the total cycle count
    unsigned int cycle_count = end_cycles - start_cycles;

    riscv_dsp_cmag_f32(input, magnitude, N);

	// Print frequency bins and magnitudes
	printf("Frequency (Hz) \tMagnitude\n");
	for (int i = 0; i < N; i++) {
		frequency_bin[i] = i * SAMPLING_FREQ / N;
		printf("%f \t%f\n", frequency_bin[i], magnitude[i]);
		delay_ms(20);
	}

    // Print results: cycle count
    printf("Cycle Count for D25F: %u\n", cycle_count);

    return 0;
}
