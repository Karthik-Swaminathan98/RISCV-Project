#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "app_config.h"
#include "core.h"
#include "clock.h"
#include "stimer.h"
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_complex_math.h"
#include "riscv_dsp_filtering_math.h"

// Constants
#define TEST_LENGTH_SAMPLES  320
#define BLOCK_SIZE           32
#define NUM_TAPS_ARRAY_SIZE  29
#define NUM_TAPS             29
#define NUM_TAPS_q15             8
#define SNR_THRESHOLD_F32    75.0f

extern const int FIR_SIZES[];
#define FIR_SIZES_COUNT 6
#define Q15_SCALE 32767
extern const float32_t firCoeffs32[NUM_TAPS_ARRAY_SIZE];
extern const q15_t firCoeffsQ15[NUM_TAPS_q15];

// Sine, step, ramp, and noise signal frequency and amplitude for generation
#define SINE_FREQ            1000.0f
#define SAMPLING_FREQ        48000.0f

extern void user_init(void);

// Clock Frequency
extern uint32_t clkFastfreq;

// Stack Parameters (constants)
extern const uint32_t stack_top;
extern const uint32_t stack_limit;
extern const uint32_t STACK_SIZE;

// Utility Function Declarations
uint32_t get_clk_fast_freq(void);
void reset_counters(void);
void read_perf_counters(unsigned int *cycles, unsigned int *instructions);
void fill_stack_pattern_to_sp(void);
uint32_t measure_stack_usage(void);

#endif // MAIN_H
