#include <stdlib.h>
#include <riscv_nnfunctions.h>
//#include "../Include_ARM/dw_int16xint8/test_data.h"
#include "validate.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"
#include "../TestData_ARM/softmax_s16/exp_lut_data.h"
#include "../TestData_ARM/softmax_s16/one_by_one_lut_data.h"
#include "../TestData_ARM/softmax_s16/test_data.h"

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

void softmax_s16_riscv_softmax_s16(void)
{
    const int32_t num_rows = SOFTMAX_S16_NUM_ROWS;
    const int32_t row_size = SOFTMAX_S16_ROW_SIZE;
    const int32_t mult = SOFTMAX_S16_INPUT_MULT;
    const int32_t shift = SOFTMAX_S16_INPUT_LEFT_SHIFT;
    const int16_t *input_data = softmax_s16_input;
    const nmsis_nn_softmax_lut_s16 softmax_params = {.exp_lut = softmax_s16_exp_lut,
                                                     .one_by_one_lut = softmax_s16_one_by_one_lut};
    int16_t output[SOFTMAX_S16_DST_SIZE];

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

	// Fill stack with a known pattern
	fill_stack_pattern_to_sp();

    reset_cycle_count();
	// Measure cycles
	uint32_t start_cycles_s16 = read_cycle_counter();

    riscv_softmax_s16(input_data, num_rows, row_size, mult, shift, &softmax_params, output);
	// Measure cycles
	uint32_t end_cycles_s16 = read_cycle_counter();

    // Measure stack usage
    uint32_t stack_used_s16 = measure_stack_usage();

	// Calculate cycle count
	uint32_t cycle_count_s16 = end_cycles_s16 - start_cycles_s16;

    if (validate_s16(output, softmax_s16_output_ref, SOFTMAX_S16_DST_SIZE)) {
		printf("riscv_softmax_s16 output validation PASSED\n\r");
		printf("Stack Used for riscv_softmax_s16: %lu bytes\n\r", (unsigned long)stack_used_s16);
		delay_ms(20);
		printf("Cycle for riscv_softmax_s16: %lu\n\r", (unsigned long)cycle_count_s16);
	} else {
		printf("riscv_softmax_s16 output validation FAILED\n\r");
	}
}
