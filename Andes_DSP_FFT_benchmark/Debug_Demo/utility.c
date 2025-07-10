#include "main.h"

const int FFT_SIZES[FFT_SIZES_COUNT] = {32, 64, 128, 256, 512, 1024};

const uint32_t stack_top = 0x000A0000;
const uint32_t stack_limit = 0x0009F000;
const uint32_t STACK_SIZE = 4096;

uint32_t get_clk_fast_freq(void) {
    return sys_clk.cclk * 1e6;
}

void reset_counters() {
    write_csr(NDS_MCYCLE, 0);
    write_csr(NDS_MINSTRET, 0);
}

void read_perf_counters(unsigned int *cycles, unsigned int *instructions) {
    *cycles = read_csr(NDS_MCYCLE);
    *instructions = read_csr(NDS_MINSTRET);
}

void fill_stack_pattern_to_sp() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));

    uint32_t *p = (uint32_t *)stack_limit;
    while (p < sp) {
        *p++ = 0xAAAAAAAA;
    }
}

uint32_t measure_stack_usage() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));

    uint32_t *p = (uint32_t *)stack_limit;
    while (p < sp) {
        if (*p != 0xAAAAAAAA) {
            break;
        }
        p++;
    }

    return ((uint32_t)sp - (uint32_t)p);
}

void calculate_averages(uint32_t* cycle_counts, uint32_t* instr_counts, float* exec_time_us, uint32_t* stack_usages, int num_executions) {
    uint32_t total_cycles = 0, total_instr = 0, total_stack_used = 0;
    float total_exec_time_us = 0;

    for (int i = 0; i < num_executions; i++) {
        total_cycles += cycle_counts[i];
        total_instr += instr_counts[i];
        total_stack_used += stack_usages[i];
        total_exec_time_us += exec_time_us[i];
    }

    printf("\nAverages across %d executions:\n\r", num_executions);
    printf("Cycle Count = %lu\n\r", total_cycles / num_executions);
    printf("Estimated Instructions = %lu\n\r", total_instr / num_executions);
    printf("Execution Time = %.2f us\n\r", total_exec_time_us / num_executions);
    printf("Stack Used = %lu bytes\n\r", total_stack_used / num_executions);
}

void generate_sine_wave_f32(float32_t* input, int N, float signal_freq, float sampling_freq) {
    for (int i = 0; i < N; i++) {
        float value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        input[2 * i] = value;  // Real part
        input[2 * i + 1] = 0;  // Imaginary part
    }
}

void generate_sine_wave_q15(q15_t* input, int N, float signal_freq, float sampling_freq) {
    for (int i = 0; i < N; i++) {
        float value = sinf(2 * M_PI * signal_freq * i / sampling_freq);
        input[2 * i] = (q15_t)(value * Q15_SCALE);  // Real part
        input[2 * i + 1] = 0;                       // Imaginary part
    }
}
