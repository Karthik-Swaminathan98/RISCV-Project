#include "main.h"

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
