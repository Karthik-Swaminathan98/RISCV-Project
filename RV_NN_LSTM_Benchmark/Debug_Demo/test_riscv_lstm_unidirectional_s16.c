#include <stdlib.h>
#include <riscv_nnfunctions.h>
#include "../TestData_ARM/lstm_1_s16/test_data.h"
#include "../TestData_ARM/lstm_2_s16/test_data.h"
#include "../TestData_ARM/lstm_one_time_step_s16/test_data.h"
#include "validate.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"


#define LARGEST_BUFFER_SIZE LSTM_1_S16_HIDDEN_SIZE *LSTM_1_S16_BATCH_SIZE *LSTM_1_S16_TIME_STEPS

static int16_t buffer1[LARGEST_BUFFER_SIZE];
static int16_t buffer2[LARGEST_BUFFER_SIZE];
static int16_t buffer3[LARGEST_BUFFER_SIZE];


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

void lstm_1_s16(void)
{
    int16_t output[LSTM_1_S16_BATCH_SIZE * LSTM_1_S16_TIME_STEPS * LSTM_1_S16_HIDDEN_SIZE] = {0};
    //const riscv_nmsis_nn_status expected = riscv_nmsis_NN_SUCCESS;
    const int16_t *output_ref = &lstm_1_s16_output[0];
    const int32_t output_ref_size = LSTM_1_S16_BATCH_SIZE * LSTM_1_S16_TIME_STEPS * LSTM_1_S16_HIDDEN_SIZE;

    int64_t input_data_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];
    int64_t forget_data_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];
    int64_t cell_data_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];
    int64_t output_data_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];

    int64_t input_hidden_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];
    int64_t forget_hidden_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];
    int64_t cell_hidden_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];
    int64_t output_hidden_kernel_sum[LSTM_1_S16_HIDDEN_SIZE];

    riscv_vector_sum_s8_s64(&input_data_kernel_sum[0],
                          LSTM_1_S16_INPUT_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_input_gate_input_weights[0],
                          LSTM_1_S16_INPUT_ZERO_POINT,
                          &lstm_1_s16_input_gate_bias[0]);
    riscv_vector_sum_s8_s64(&forget_data_kernel_sum[0],
                          LSTM_1_S16_INPUT_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_forget_gate_input_weights[0],
                          LSTM_1_S16_INPUT_ZERO_POINT,
                          &lstm_1_s16_forget_gate_bias[0]);
    riscv_vector_sum_s8_s64(&cell_data_kernel_sum[0],
                          LSTM_1_S16_INPUT_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_cell_gate_input_weights[0],
                          LSTM_1_S16_INPUT_ZERO_POINT,
                          &lstm_1_s16_cell_gate_bias[0]);
    riscv_vector_sum_s8_s64(&output_data_kernel_sum[0],
                          LSTM_1_S16_INPUT_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_output_gate_input_weights[0],
                          LSTM_1_S16_INPUT_ZERO_POINT,
                          &lstm_1_s16_output_gate_bias[0]);

    riscv_vector_sum_s8_s64(&input_hidden_kernel_sum[0],
                          LSTM_1_S16_HIDDEN_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_input_gate_hidden_weights[0],
                          -LSTM_1_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&forget_hidden_kernel_sum[0],
                          LSTM_1_S16_HIDDEN_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_forget_gate_hidden_weights[0],
                          -LSTM_1_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&cell_hidden_kernel_sum[0],
                          LSTM_1_S16_HIDDEN_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_cell_gate_hidden_weights[0],
                          -LSTM_1_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&output_hidden_kernel_sum[0],
                          LSTM_1_S16_HIDDEN_SIZE,
                          LSTM_1_S16_HIDDEN_SIZE,
                          &lstm_1_s16_output_gate_hidden_weights[0],
                          -LSTM_1_S16_OUTPUT_ZERO_POINT,
                          NULL);

    // INPUT GATE
    const nmsis_nn_lstm_gate gate_input = {LSTM_1_S16_INPUT_GATE_INPUT_MULTIPLIER,
                                           LSTM_1_S16_INPUT_GATE_INPUT_SHIFT,
                                           &lstm_1_s16_input_gate_input_weights[0],
                                           &input_data_kernel_sum[0],
                                           LSTM_1_S16_INPUT_GATE_HIDDEN_MULTIPLIER,
                                           LSTM_1_S16_INPUT_GATE_HIDDEN_SHIFT,
                                           &lstm_1_s16_input_gate_hidden_weights[0],
                                           &input_hidden_kernel_sum[0],
                                           &lstm_1_s16_input_gate_bias[0],
                                           RISCV_SIGMOID};

    // FORGET GATE
    const nmsis_nn_lstm_gate gate_forget = {LSTM_1_S16_FORGET_GATE_INPUT_MULTIPLIER,
                                            LSTM_1_S16_FORGET_GATE_INPUT_SHIFT,
                                            &lstm_1_s16_forget_gate_input_weights[0],
                                            &forget_data_kernel_sum[0],
                                            LSTM_1_S16_FORGET_GATE_HIDDEN_MULTIPLIER,
                                            LSTM_1_S16_FORGET_GATE_HIDDEN_SHIFT,
                                            &lstm_1_s16_forget_gate_hidden_weights[0],
                                            &forget_hidden_kernel_sum[0],
                                            &lstm_1_s16_forget_gate_bias[0],
											RISCV_SIGMOID};

    // CELL GATE
    const nmsis_nn_lstm_gate gate_cell = {LSTM_1_S16_CELL_GATE_INPUT_MULTIPLIER,
                                          LSTM_1_S16_CELL_GATE_INPUT_SHIFT,
                                          &lstm_1_s16_cell_gate_input_weights[0],
                                          &cell_data_kernel_sum[0],
                                          LSTM_1_S16_CELL_GATE_HIDDEN_MULTIPLIER,
                                          LSTM_1_S16_CELL_GATE_HIDDEN_SHIFT,
                                          &lstm_1_s16_cell_gate_hidden_weights[0],
                                          &cell_hidden_kernel_sum[0],
                                          &lstm_1_s16_cell_gate_bias[0],
										  RISCV_TANH};

    // OUTPUT GATE
    const nmsis_nn_lstm_gate gate_output = {LSTM_1_S16_OUTPUT_GATE_INPUT_MULTIPLIER,
                                            LSTM_1_S16_OUTPUT_GATE_INPUT_SHIFT,
                                            &lstm_1_s16_output_gate_input_weights[0],
                                            &output_data_kernel_sum[0],
                                            LSTM_1_S16_OUTPUT_GATE_HIDDEN_MULTIPLIER,
                                            LSTM_1_S16_OUTPUT_GATE_HIDDEN_SHIFT,
                                            &lstm_1_s16_output_gate_hidden_weights[0],
                                            &output_hidden_kernel_sum[0],
                                            &lstm_1_s16_output_gate_bias[0],
											RISCV_SIGMOID};

    // LSTM DATA
    const nmsis_nn_lstm_params params = {LSTM_1_S16_TIME_MAJOR,
                                         LSTM_1_S16_BATCH_SIZE,
                                         LSTM_1_S16_TIME_STEPS,
                                         LSTM_1_S16_INPUT_SIZE,
                                         LSTM_1_S16_HIDDEN_SIZE,
                                         LSTM_1_S16_INPUT_ZERO_POINT,
                                         LSTM_1_S16_FORGET_TO_CELL_MULTIPLIER,
                                         LSTM_1_S16_FORGET_TO_CELL_SHIFT,
                                         LSTM_1_S16_INPUT_TO_CELL_MULTIPLIER,
                                         LSTM_1_S16_INPUT_TO_CELL_SHIFT,
                                         LSTM_1_S16_CELL_CLIP,
                                         LSTM_1_S16_CELL_SCALE_POWER,
                                         LSTM_1_S16_OUTPUT_MULTIPLIER,
                                         LSTM_1_S16_OUTPUT_SHIFT,
                                         LSTM_1_S16_OUTPUT_ZERO_POINT,
                                         gate_forget,
                                         gate_input,
                                         gate_cell,
                                         gate_output};

    nmsis_nn_lstm_context buffers;
    buffers.temp1 = buffer1;
    buffers.temp2 = buffer2;
    buffers.cell_state = buffer3;

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();

	// Fill stack with a known pattern
	fill_stack_pattern_to_sp();

	// Measure cycles
	uint32_t start_cycles_s16 = read_cycle_counter();

    riscv_lstm_unidirectional_s16(lstm_1_s16_input_tensor, output, &params, &buffers);

	// Measure cycles
	uint32_t end_cycles_s16 = read_cycle_counter();

    // Measure stack usage
    uint32_t stack_used_s16 = measure_stack_usage();

	// Calculate cycle count
	uint32_t cycle_count_s16 = end_cycles_s16 - start_cycles_s16;
	printf("\n\r");
    if (validate_s16(output, output_ref, output_ref_size)) {
		printf("LSTM_1_S16 output validation PASSED\n\r");
		printf("Stack Used for riscv_lstm_unidirectional_s16: %lu bytes\n\r", (unsigned long)stack_used_s16);
		printf("Cycle for riscv_lstm_unidirectional_s16: %lu\n\r", (unsigned long)cycle_count_s16);
	} else {
		printf("LSTM_1_S16 output validation FAILED\n\r");
	}
}

void lstm_2_s16(void)
{
    int16_t output[LSTM_2_S16_BATCH_SIZE * LSTM_2_S16_TIME_STEPS * LSTM_2_S16_HIDDEN_SIZE] = {0};
    const int16_t *output_ref = &lstm_2_s16_output[0];
    const int32_t output_ref_size = LSTM_2_S16_BATCH_SIZE * LSTM_2_S16_TIME_STEPS * LSTM_2_S16_HIDDEN_SIZE;

    int64_t input_data_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];
    int64_t forget_data_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];
    int64_t cell_data_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];
    int64_t output_data_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];

    int64_t input_hidden_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];
    int64_t forget_hidden_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];
    int64_t cell_hidden_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];
    int64_t output_hidden_kernel_sum[LSTM_2_S16_HIDDEN_SIZE];

    riscv_vector_sum_s8_s64(&input_data_kernel_sum[0],
                          LSTM_2_S16_INPUT_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_input_gate_input_weights[0],
                          LSTM_2_S16_INPUT_ZERO_POINT,
                          &lstm_2_s16_input_gate_bias[0]);
    riscv_vector_sum_s8_s64(&forget_data_kernel_sum[0],
                          LSTM_2_S16_INPUT_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_forget_gate_input_weights[0],
                          LSTM_2_S16_INPUT_ZERO_POINT,
                          &lstm_2_s16_forget_gate_bias[0]);
    riscv_vector_sum_s8_s64(&cell_data_kernel_sum[0],
                          LSTM_2_S16_INPUT_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_cell_gate_input_weights[0],
                          LSTM_2_S16_INPUT_ZERO_POINT,
                          &lstm_2_s16_cell_gate_bias[0]);
    riscv_vector_sum_s8_s64(&output_data_kernel_sum[0],
                          LSTM_2_S16_INPUT_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_output_gate_input_weights[0],
                          LSTM_2_S16_INPUT_ZERO_POINT,
                          &lstm_2_s16_output_gate_bias[0]);

    riscv_vector_sum_s8_s64(&input_hidden_kernel_sum[0],
                          LSTM_2_S16_HIDDEN_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_input_gate_hidden_weights[0],
                          -LSTM_2_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&forget_hidden_kernel_sum[0],
                          LSTM_2_S16_HIDDEN_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_forget_gate_hidden_weights[0],
                          -LSTM_2_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&cell_hidden_kernel_sum[0],
                          LSTM_2_S16_HIDDEN_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_cell_gate_hidden_weights[0],
                          -LSTM_2_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&output_hidden_kernel_sum[0],
                          LSTM_2_S16_HIDDEN_SIZE,
                          LSTM_2_S16_HIDDEN_SIZE,
                          &lstm_2_s16_output_gate_hidden_weights[0],
                          -LSTM_2_S16_OUTPUT_ZERO_POINT,
                          NULL);

    const nmsis_nn_lstm_gate gate_input = {
        LSTM_2_S16_INPUT_GATE_INPUT_MULTIPLIER,
        LSTM_2_S16_INPUT_GATE_INPUT_SHIFT,
        &lstm_2_s16_input_gate_input_weights[0],
        &input_data_kernel_sum[0],
        LSTM_2_S16_INPUT_GATE_HIDDEN_MULTIPLIER,
        LSTM_2_S16_INPUT_GATE_HIDDEN_SHIFT,
        &lstm_2_s16_input_gate_hidden_weights[0],
        &input_hidden_kernel_sum[0],
        &lstm_2_s16_input_gate_bias[0],
		RISCV_SIGMOID};

    const nmsis_nn_lstm_gate gate_forget = {
        LSTM_2_S16_FORGET_GATE_INPUT_MULTIPLIER,
        LSTM_2_S16_FORGET_GATE_INPUT_SHIFT,
        &lstm_2_s16_forget_gate_input_weights[0],
        &forget_data_kernel_sum[0],
        LSTM_2_S16_FORGET_GATE_HIDDEN_MULTIPLIER,
        LSTM_2_S16_FORGET_GATE_HIDDEN_SHIFT,
        &lstm_2_s16_forget_gate_hidden_weights[0],
        &forget_hidden_kernel_sum[0],
        &lstm_2_s16_forget_gate_bias[0],
		RISCV_SIGMOID};

    const nmsis_nn_lstm_gate gate_cell = {
        LSTM_2_S16_CELL_GATE_INPUT_MULTIPLIER,
        LSTM_2_S16_CELL_GATE_INPUT_SHIFT,
        &lstm_2_s16_cell_gate_input_weights[0],
        &cell_data_kernel_sum[0],
        LSTM_2_S16_CELL_GATE_HIDDEN_MULTIPLIER,
        LSTM_2_S16_CELL_GATE_HIDDEN_SHIFT,
        &lstm_2_s16_cell_gate_hidden_weights[0],
        &cell_hidden_kernel_sum[0],
        &lstm_2_s16_cell_gate_bias[0],
		RISCV_TANH};

    const nmsis_nn_lstm_gate gate_output = {
        LSTM_2_S16_OUTPUT_GATE_INPUT_MULTIPLIER,
        LSTM_2_S16_OUTPUT_GATE_INPUT_SHIFT,
        &lstm_2_s16_output_gate_input_weights[0],
        &output_data_kernel_sum[0],
        LSTM_2_S16_OUTPUT_GATE_HIDDEN_MULTIPLIER,
        LSTM_2_S16_OUTPUT_GATE_HIDDEN_SHIFT,
        &lstm_2_s16_output_gate_hidden_weights[0],
        &output_hidden_kernel_sum[0],
        &lstm_2_s16_output_gate_bias[0],
		RISCV_SIGMOID};

    const nmsis_nn_lstm_params params = {
        LSTM_2_S16_TIME_MAJOR,
        LSTM_2_S16_BATCH_SIZE,
        LSTM_2_S16_TIME_STEPS,
        LSTM_2_S16_INPUT_SIZE,
        LSTM_2_S16_HIDDEN_SIZE,
        LSTM_2_S16_INPUT_ZERO_POINT,
        LSTM_2_S16_FORGET_TO_CELL_MULTIPLIER,
        LSTM_2_S16_FORGET_TO_CELL_SHIFT,
        LSTM_2_S16_INPUT_TO_CELL_MULTIPLIER,
        LSTM_2_S16_INPUT_TO_CELL_SHIFT,
        LSTM_2_S16_CELL_CLIP,
        LSTM_2_S16_CELL_SCALE_POWER,
        LSTM_2_S16_OUTPUT_MULTIPLIER,
        LSTM_2_S16_OUTPUT_SHIFT,
        LSTM_2_S16_OUTPUT_ZERO_POINT,
        gate_forget,
        gate_input,
        gate_cell,
        gate_output};

    nmsis_nn_lstm_context buffers;
    buffers.temp1 = buffer1;
    buffers.temp2 = buffer2;
    buffers.cell_state = buffer3;

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();
    fill_stack_pattern_to_sp();

    uint32_t start_cycles_s16 = read_cycle_counter();

    riscv_lstm_unidirectional_s16(lstm_2_s16_input_tensor, output, &params, &buffers);

    uint32_t end_cycles_s16 = read_cycle_counter();
    uint32_t cycle_count_s16 = end_cycles_s16 - start_cycles_s16;
    uint32_t stack_used_s16 = measure_stack_usage();
    printf("\n\r");
    if (validate_s16(output, output_ref, output_ref_size)) {
        printf("LSTM_2_S16 output validation PASSED\n\r");
        printf("Stack Used for riscv_lstm_unidirectional_s16: %lu bytes\n\r", (unsigned long)stack_used_s16);
        printf("Cycle for riscv_lstm_unidirectional_s16: %lu\n\r", (unsigned long)cycle_count_s16);
    } else {
        printf("LSTM_2_S16 output validation FAILED\n\r");
    }
}

void lstm_one_time_step_s16(void)
{
    int16_t output[LSTM_ONE_TIME_STEP_S16_BATCH_SIZE * LSTM_ONE_TIME_STEP_S16_TIME_STEPS *
                   LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE] = {0};
    const int16_t *output_ref = &lstm_one_time_step_s16_output[0];
    const int32_t output_ref_size =
        LSTM_ONE_TIME_STEP_S16_BATCH_SIZE * LSTM_ONE_TIME_STEP_S16_TIME_STEPS * LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE;

    int64_t input_data_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];
    int64_t forget_data_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];
    int64_t cell_data_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];
    int64_t output_data_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];

    int64_t input_hidden_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];
    int64_t forget_hidden_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];
    int64_t cell_hidden_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];
    int64_t output_hidden_kernel_sum[LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE];

    riscv_vector_sum_s8_s64(&input_data_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_input_gate_input_weights[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_ZERO_POINT,
                          &lstm_one_time_step_s16_input_gate_bias[0]);
    riscv_vector_sum_s8_s64(&forget_data_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_forget_gate_input_weights[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_ZERO_POINT,
                          &lstm_one_time_step_s16_forget_gate_bias[0]);
    riscv_vector_sum_s8_s64(&cell_data_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_cell_gate_input_weights[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_ZERO_POINT,
                          &lstm_one_time_step_s16_cell_gate_bias[0]);
    riscv_vector_sum_s8_s64(&output_data_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_output_gate_input_weights[0],
                          LSTM_ONE_TIME_STEP_S16_INPUT_ZERO_POINT,
                          &lstm_one_time_step_s16_output_gate_bias[0]);

    riscv_vector_sum_s8_s64(&input_hidden_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_input_gate_hidden_weights[0],
                          -LSTM_ONE_TIME_STEP_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&forget_hidden_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_forget_gate_hidden_weights[0],
                          -LSTM_ONE_TIME_STEP_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&cell_hidden_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_cell_gate_hidden_weights[0],
                          -LSTM_ONE_TIME_STEP_S16_OUTPUT_ZERO_POINT,
                          NULL);
    riscv_vector_sum_s8_s64(&output_hidden_kernel_sum[0],
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
                          &lstm_one_time_step_s16_output_gate_hidden_weights[0],
                          -LSTM_ONE_TIME_STEP_S16_OUTPUT_ZERO_POINT,
                          NULL);

    const nmsis_nn_lstm_gate gate_input = {
        LSTM_ONE_TIME_STEP_S16_INPUT_GATE_INPUT_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_INPUT_GATE_INPUT_SHIFT,
        &lstm_one_time_step_s16_input_gate_input_weights[0],
        &input_data_kernel_sum[0],
        LSTM_ONE_TIME_STEP_S16_INPUT_GATE_HIDDEN_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_INPUT_GATE_HIDDEN_SHIFT,
        &lstm_one_time_step_s16_input_gate_hidden_weights[0],
        &input_hidden_kernel_sum[0],
        &lstm_one_time_step_s16_input_gate_bias[0],
		RISCV_SIGMOID};

    const nmsis_nn_lstm_gate gate_forget = {
        LSTM_ONE_TIME_STEP_S16_FORGET_GATE_INPUT_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_FORGET_GATE_INPUT_SHIFT,
        &lstm_one_time_step_s16_forget_gate_input_weights[0],
        &forget_data_kernel_sum[0],
        LSTM_ONE_TIME_STEP_S16_FORGET_GATE_HIDDEN_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_FORGET_GATE_HIDDEN_SHIFT,
        &lstm_one_time_step_s16_forget_gate_hidden_weights[0],
        &forget_hidden_kernel_sum[0],
        &lstm_one_time_step_s16_forget_gate_bias[0],
		RISCV_SIGMOID};

    const nmsis_nn_lstm_gate gate_cell = {
        LSTM_ONE_TIME_STEP_S16_CELL_GATE_INPUT_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_CELL_GATE_INPUT_SHIFT,
        &lstm_one_time_step_s16_cell_gate_input_weights[0],
        &cell_data_kernel_sum[0],
        LSTM_ONE_TIME_STEP_S16_CELL_GATE_HIDDEN_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_CELL_GATE_HIDDEN_SHIFT,
        &lstm_one_time_step_s16_cell_gate_hidden_weights[0],
        &cell_hidden_kernel_sum[0],
        &lstm_one_time_step_s16_cell_gate_bias[0],
		RISCV_TANH};

    const nmsis_nn_lstm_gate gate_output = {
        LSTM_ONE_TIME_STEP_S16_OUTPUT_GATE_INPUT_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_OUTPUT_GATE_INPUT_SHIFT,
        &lstm_one_time_step_s16_output_gate_input_weights[0],
        &output_data_kernel_sum[0],
        LSTM_ONE_TIME_STEP_S16_OUTPUT_GATE_HIDDEN_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_OUTPUT_GATE_HIDDEN_SHIFT,
        &lstm_one_time_step_s16_output_gate_hidden_weights[0],
        &output_hidden_kernel_sum[0],
        &lstm_one_time_step_s16_output_gate_bias[0],
		RISCV_SIGMOID};

    const nmsis_nn_lstm_params params = {
        LSTM_ONE_TIME_STEP_S16_TIME_MAJOR,
        LSTM_ONE_TIME_STEP_S16_BATCH_SIZE,
        LSTM_ONE_TIME_STEP_S16_TIME_STEPS,
        LSTM_ONE_TIME_STEP_S16_INPUT_SIZE,
        LSTM_ONE_TIME_STEP_S16_HIDDEN_SIZE,
        LSTM_ONE_TIME_STEP_S16_INPUT_ZERO_POINT,
        LSTM_ONE_TIME_STEP_S16_FORGET_TO_CELL_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_FORGET_TO_CELL_SHIFT,
        LSTM_ONE_TIME_STEP_S16_INPUT_TO_CELL_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_INPUT_TO_CELL_SHIFT,
        LSTM_ONE_TIME_STEP_S16_CELL_CLIP,
        LSTM_ONE_TIME_STEP_S16_CELL_SCALE_POWER,
        LSTM_ONE_TIME_STEP_S16_OUTPUT_MULTIPLIER,
        LSTM_ONE_TIME_STEP_S16_OUTPUT_SHIFT,
        LSTM_ONE_TIME_STEP_S16_OUTPUT_ZERO_POINT,
        gate_forget,
        gate_input,
        gate_cell,
        gate_output};

    nmsis_nn_lstm_context buffers;
    buffers.temp1 = buffer1;
    buffers.temp2 = buffer2;
    buffers.cell_state = buffer3;

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();
    fill_stack_pattern_to_sp();

    uint32_t start_cycles = read_cycle_counter();

    riscv_nmsis_nn_status result =
        riscv_lstm_unidirectional_s16(lstm_one_time_step_s16_input_tensor, output, &params, &buffers);

    uint32_t end_cycles = read_cycle_counter();
    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t stack_used = measure_stack_usage();
    printf("\n\r");
    if ((result == RISCV_NMSIS_NN_SUCCESS) && validate_s16(output, output_ref, output_ref_size)) {
        printf("LSTM_ONE_TIME_STEP_S16 output validation PASSED\n\r");
        printf("Stack Used for riscv_lstm_unidirectional_s16 (one time step): %lu bytes\n\r", (unsigned long)stack_used);
        printf("Cycle for riscv_lstm_unidirectional_s16 (one time step): %lu\n\r", (unsigned long)cycle_count);
    } else {
        printf("LSTM_ONE_TIME_STEP_S16 output validation FAILED\n\r");
    }
}
