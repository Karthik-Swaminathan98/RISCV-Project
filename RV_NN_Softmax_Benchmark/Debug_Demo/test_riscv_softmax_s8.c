#include <stdlib.h>
#include <riscv_nnfunctions.h>
//#include "../Include_riscv/dw_int16xint8/test_data.h"
#include "validate.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"
#include "../TestData_ARM/softmax/test_data.h"

static void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

static unsigned int read_cycle_counter() {
    return read_csr(NDS_MCYCLE);
}
#define REPEAT_NUM (2)
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

void softmax_riscv_softmax_s8(void)
{
    const int32_t num_rows = SOFTMAX_NUM_ROWS;
    const int32_t row_size = SOFTMAX_ROW_SIZE;
    const int32_t mult = SOFTMAX_INPUT_MULT;
    const int32_t shift = SOFTMAX_INPUT_LEFT_SHIFT;
    const int32_t diff_min = SOFTMAX_DIFF_MIN;
    const int8_t *input_data = softmax_input;
    int8_t output[SOFTMAX_DST_SIZE];

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();

	// Fill stack with a known pattern
	fill_stack_pattern_to_sp();

	// Measure cycles
	uint32_t start_cycles_s8 = read_cycle_counter();
    riscv_softmax_s8(input_data, num_rows, row_size, mult, shift, diff_min, output);
    // Measure cycles
    uint32_t end_cycles_s8 = read_cycle_counter();

    // Measure stack usage
    uint32_t stack_used_s8 = measure_stack_usage();

    // Calculate cycle count
    uint32_t cycle_count_s8 = end_cycles_s8 - start_cycles_s8;
    printf("\n\r");
    if (validate(output, softmax_output_ref, SOFTMAX_DST_SIZE)) {
		printf("riscv_softmax_s8 output validation PASSED\n\r");
		printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used_s8);
		printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count_s8);
	} else {
		printf("riscv_avgpriscv_softmax_s8ool_s8 output validation FAILED\n\r");
	}

}
void softmax_invalid_diff_min_riscv_softmax_s8(void)
{
    const int32_t num_rows = SOFTMAX_NUM_ROWS;
    const int32_t row_size = SOFTMAX_ROW_SIZE;
    const int32_t mult = SOFTMAX_INPUT_MULT;
    const int32_t shift = SOFTMAX_INPUT_LEFT_SHIFT;
    const int32_t diff_min = 0x7FFFFFFF;
    const int8_t *input_data = softmax_input;
    int8_t output[SOFTMAX_DST_SIZE];

    int8_t *softmax_expect_invalid_output = malloc(SOFTMAX_DST_SIZE);
    for (int i = 0; i < SOFTMAX_DST_SIZE; i++)
    {
        softmax_expect_invalid_output[i] = -128;
    }

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();

    // Fill stack with a known pattern
    fill_stack_pattern_to_sp();

    // Measure cycles
    uint32_t start_cycles = read_cycle_counter();
    riscv_softmax_s8(input_data, num_rows, row_size, mult, shift, diff_min, output);
    uint32_t end_cycles = read_cycle_counter();

    // Measure stack usage
    uint32_t stack_used = measure_stack_usage();

    // Calculate cycle count
    uint32_t cycle_count = end_cycles - start_cycles;
    printf("\n\r");
    if (validate(output, softmax_expect_invalid_output, SOFTMAX_DST_SIZE)) {
        printf("riscv_softmax_s8 with invalid diff_min output validation PASSED\n\r");
        printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
    } else {
        printf("riscv_softmax_s8 with invalid diff_min output validation FAILED\n\r");
    }

    free(softmax_expect_invalid_output);
}

