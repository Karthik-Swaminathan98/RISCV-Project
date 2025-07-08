#include "main.h"
#include "../TestData_ARM/softmax/test_data.h"

#define REPEAT_NUM (2)

void softmax_riscv_softmax_s8(void)
{
    const int32_t num_rows = SOFTMAX_NUM_ROWS;
    const int32_t row_size = SOFTMAX_ROW_SIZE;
    const int32_t mult = SOFTMAX_INPUT_MULT;
    const int32_t shift = SOFTMAX_INPUT_LEFT_SHIFT;
    const int32_t diff_min = SOFTMAX_DIFF_MIN;
    const int8_t *input_data = softmax_input;
    int8_t output[SOFTMAX_DST_SIZE];

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_softmax_s8(input_data, num_rows, row_size, mult, shift, diff_min, output);

    read_perf_counters(&end_cycles, &end_inst);
    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t instr_count = end_inst - start_inst;
    uint32_t stack_used = measure_stack_usage();

    float time_sec = (float)cycle_count / clkFastfreq;
    float time_us = time_sec * 1e6f;

    printf("\n\r");
    if (validate(output, softmax_output_ref, SOFTMAX_DST_SIZE)) {
		printf("riscv_softmax_s8 output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
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

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_softmax_s8(input_data, num_rows, row_size, mult, shift, diff_min, output);

    read_perf_counters(&end_cycles, &end_inst);
    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t instr_count = end_inst - start_inst;
    uint32_t stack_used = measure_stack_usage();

    float time_sec = (float)cycle_count / clkFastfreq;
    float time_us = time_sec * 1e6f;

    printf("\n\r");
    if (validate(output, softmax_expect_invalid_output, SOFTMAX_DST_SIZE)) {
        printf("riscv_softmax_s8 with invalid diff_min output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
    } else {
        printf("riscv_softmax_s8 with invalid diff_min output validation FAILED\n\r");
    }

    free(softmax_expect_invalid_output);
}

