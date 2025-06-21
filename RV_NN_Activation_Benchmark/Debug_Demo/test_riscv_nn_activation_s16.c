#include <stdlib.h>
#include <riscv_nnfunctions.h>
#include "validate.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"

//#define ACTIVATION_SIZE 128
#define LEFT_SHIFT 8
#define ACTIVATION_FUNC RISCV_SIGMOID
//static int16_t s16_input[ACTIVATION_SIZE] = {};


static void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

static unsigned int read_cycle_counter() {
    return read_csr(NDS_MCYCLE);
}

#define STACK_SIZE 0x1000
extern uint32_t _STACK_TOP;
uint32_t stack_limit;
uint32_t stack_top;

static void fill_stack_pattern_to_sp() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));

    uint32_t *p = (uint32_t*)stack_limit;
    while (p < sp) {
        *p++ = 0xAAAAAAAA;
    }
}

static uint32_t measure_stack_usage() {
    uint32_t *sp;
    __asm__ volatile ("mv %0, sp" : "=r" (sp));

    uint32_t *p = (uint32_t*)stack_limit;
    while (p < sp) {
        if (*p != 0xAAAAAAAA) {
            break;
        }
        p++;
    }

    return ((uint32_t)sp - (uint32_t)p);
}

void activ_riscv_nn_activation_s16(void) {
    const int input_sizes[] = {512, 1024, 2048};
    const int num_sizes = sizeof(input_sizes) / sizeof(input_sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int size = input_sizes[i];

        // Allocate buffers
        int16_t *input = (int16_t *)malloc(size * sizeof(int16_t));
        int16_t *output = (int16_t *)malloc(size * sizeof(int16_t));

        if (!input || !output) {
            printf("Memory allocation failed for input size %d\n\r", size);
            free(input);
            free(output);
            continue;
        }

        generate_rand_s16(input, size);
        memset(output, 0, size * sizeof(int16_t));

        // Get stack top and calculate stack limit
        stack_top = (uint32_t)&_STACK_TOP;
        stack_limit = stack_top - STACK_SIZE;

        reset_cycle_count();
        fill_stack_pattern_to_sp();

        uint32_t start_cycles = read_cycle_counter();

        riscv_nn_activation_s16(input, output, size, LEFT_SHIFT, ACTIVATION_FUNC);

        uint32_t end_cycles = read_cycle_counter();

        uint32_t cycle_count = end_cycles - start_cycles;
        uint32_t stack_used = measure_stack_usage();

        printf("Input Size: %d\n\r", size);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);

        free(input);
        free(output);
    }
}
