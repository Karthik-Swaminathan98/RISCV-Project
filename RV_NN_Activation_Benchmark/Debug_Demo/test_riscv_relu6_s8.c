#include <stdlib.h>
#include <riscv_nnfunctions.h>
#include "validate.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"

#define RELU6_INPUT_SIZE 1024
//static int8_t relu6_s8_input[RELU6_INPUT_SIZE] = {};

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

void relu6_riscv_relu6_s8(void)
{
    const uint16_t input_sizes[] = {512, 1024, 2048};
    const int num_sizes = sizeof(input_sizes) / sizeof(input_sizes[0]);

    for (int i = 0; i < num_sizes; i++)
    {
        uint16_t size = input_sizes[i];

        // Allocate and initialize input buffer
        int8_t *input_data = (int8_t *)malloc(size * sizeof(int8_t));
        if (!input_data)
        {
            printf("Memory allocation failed for size %u\n\r", size);
            continue;
        }

        generate_rand_s8(input_data, size);

        // Get stack top and calculate stack limit
        stack_top = (uint32_t)&_STACK_TOP;
        stack_limit = stack_top - STACK_SIZE;

        reset_cycle_count();
        fill_stack_pattern_to_sp();

        uint32_t start_cycles = read_cycle_counter();
        riscv_relu6_s8(input_data, size);
        uint32_t end_cycles = read_cycle_counter();

        uint32_t cycle_count = end_cycles - start_cycles;
        uint32_t stack_used = measure_stack_usage();

        printf("Input Size: %u\n\r", size);
        printf("Cycle Count for riscv_relu6_s8: %lu\n\r", (unsigned long)cycle_count);
        printf("Stack Used for riscv_relu6_s8: %lu bytes\n\r\n", (unsigned long)stack_used);

        free(input_data);
    }
}
