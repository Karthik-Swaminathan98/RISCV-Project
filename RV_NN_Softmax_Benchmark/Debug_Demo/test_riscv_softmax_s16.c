#include "main.h"
#include "../TestData_ARM/softmax_s16/exp_lut_data.h"
#include "../TestData_ARM/softmax_s16/one_by_one_lut_data.h"
#include "../TestData_ARM/softmax_s16/test_data.h"

#define REPEAT_NUM (2)

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

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_softmax_s16(input_data, num_rows, row_size, mult, shift, &softmax_params, output);

    read_perf_counters(&end_cycles, &end_inst);
    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t instr_count = end_inst - start_inst;
    uint32_t stack_used = measure_stack_usage();

    float time_sec = (float)cycle_count / clkFastfreq;
    float time_us = time_sec * 1e6f;

    if (validate_s16(output, softmax_s16_output_ref, SOFTMAX_S16_DST_SIZE)) {
		printf("riscv_softmax_s16 output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
	} else {
		printf("riscv_softmax_s16 output validation FAILED\n\r");
	}
}
