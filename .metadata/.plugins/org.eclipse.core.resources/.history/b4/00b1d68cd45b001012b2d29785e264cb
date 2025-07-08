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

// Constants
#define SINE_FREQ        50
#define SAMPLING_FREQ    256
#define FFT_SIZES_COUNT  6
#define Q15_SCALE        32768
#define NUM_EXECUTIONS   10
#define M_PI 3.14159265358979323846

extern void user_init(void);

// Clock Frequency
extern uint32_t clkFastfreq;

// Stack Parameters (constants)
extern const uint32_t stack_top;
extern const uint32_t stack_limit;
extern const uint32_t STACK_SIZE;

// FFT sizes to benchmark
extern const int FFT_SIZES[FFT_SIZES_COUNT];

// Utility Function Declarations
uint32_t get_clk_fast_freq(void);
void reset_counters(void);
void read_perf_counters(unsigned int *cycles, unsigned int *instructions);
void fill_stack_pattern_to_sp(void);
uint32_t measure_stack_usage(void);
void calculate_averages(uint32_t* cycle_counts, uint32_t* instr_counts, float* exec_time_us, uint32_t* stack_usages, int num_executions);

#endif // MAIN_H
