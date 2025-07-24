#include "main.h"

const int FIR_SIZES[] = {32, 64, 128, 256, 512, 1024};

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

const float32_t firCoeffs32[NUM_TAPS] = {
    -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f,
    +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
    -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f,
    +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
    +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f,
    -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
    +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f,
    -0.0018225230f
};


const q15_t firCoeffsQ15[NUM_TAPS_q15] = {
		2411, 4172, 5626, 6446, 6446, 5626, 4172, 2411 // Coefficients padded to make NUM_TAPS even
};
