#include "main.h"
#include "../Include_ARM/fully_connected_int16/test_data.h"
#include "../Include_ARM/fc_int16_slow/test_data.h"
#include "../Include_ARM/fully_connected_int16_big/test_data.h"

void fully_connected_int16_riscv_fully_connected_s16(void)
{
    //const riscv_nmsis_nn_status expected = riscv_nmsis_NN_SUCCESS;
    int16_t output[FULLY_CONNECTED_INT16_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_fc_params fc_params;
    nmsis_nn_per_tensor_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const int64_t *bias_data = fully_connected_int16_biases;
    const int8_t *kernel_data = fully_connected_int16_weights;
    const int16_t *input_data = fully_connected_int16_input;
    const int16_t *output_ref = fully_connected_int16_output_ref;
    const int32_t output_ref_size = FULLY_CONNECTED_INT16_DST_SIZE;

    input_dims.n = FULLY_CONNECTED_INT16_INPUT_BATCHES;
    input_dims.w = FULLY_CONNECTED_INT16_INPUT_W;
    input_dims.h = FULLY_CONNECTED_INT16_INPUT_H;
    input_dims.c = FULLY_CONNECTED_INT16_IN_CH;
    filter_dims.n = FULLY_CONNECTED_INT16_ACCUMULATION_DEPTH;
    filter_dims.c = FULLY_CONNECTED_INT16_OUT_CH;
    filter_dims.h = FULLY_CONNECTED_INT16_INPUT_H;
    filter_dims.w = FULLY_CONNECTED_INT16_INPUT_W;
    output_dims.n = FULLY_CONNECTED_INT16_INPUT_BATCHES;
    output_dims.c = FULLY_CONNECTED_INT16_OUT_CH;

    fc_params.input_offset = 0;
    fc_params.filter_offset = 0;
    fc_params.output_offset = 0;
    fc_params.activation.min = FULLY_CONNECTED_INT16_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FULLY_CONNECTED_INT16_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FULLY_CONNECTED_INT16_OUTPUT_MULTIPLIER;
    quant_params.shift = FULLY_CONNECTED_INT16_OUTPUT_SHIFT;

    int32_t buf_size = riscv_fully_connected_s16_get_buffer_size(&filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_fully_connected_s16(&ctx,
							 &fc_params,
							 &quant_params,
							 &input_dims,
							 input_data,
							 &filter_dims,
							 kernel_data,
							 &bias_dims,
							 bias_data,
							 &output_dims,
							 output);

    read_perf_counters(&end_cycles, &end_inst);
    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t instr_count = end_inst - start_inst;
    uint32_t stack_used = measure_stack_usage();

    float time_sec = (float)cycle_count / clkFastfreq;
    float time_us = time_sec * 1e6f;

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    printf("\n\r");
	if (validate_s16(output, output_ref, output_ref_size)) {
		printf("riscv_fully_connected_s16 output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
	} else {
		printf("riscv_fully_connected_s16 output validation FAILED\n\r");
	}
}

void fully_connected_int16_big_riscv_fully_connected_s16(void)
{
    int16_t output[FULLY_CONNECTED_INT16_BIG_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_fc_params fc_params;
    nmsis_nn_per_tensor_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const int64_t *bias_data = fully_connected_int16_big_biases;
    const int8_t *kernel_data = fully_connected_int16_big_weights;
    const int16_t *input_data = fully_connected_int16_big_input;
    const int16_t *output_ref = fully_connected_int16_big_output_ref;
    const int32_t output_ref_size = FULLY_CONNECTED_INT16_BIG_DST_SIZE;

    input_dims.n = FULLY_CONNECTED_INT16_BIG_INPUT_BATCHES;
    input_dims.w = FULLY_CONNECTED_INT16_BIG_INPUT_W;
    input_dims.h = FULLY_CONNECTED_INT16_BIG_INPUT_H;
    input_dims.c = FULLY_CONNECTED_INT16_BIG_IN_CH;
    filter_dims.n = FULLY_CONNECTED_INT16_BIG_ACCUMULATION_DEPTH;
    filter_dims.c = FULLY_CONNECTED_INT16_BIG_OUT_CH;
    filter_dims.h = FULLY_CONNECTED_INT16_BIG_INPUT_H;
    filter_dims.w = FULLY_CONNECTED_INT16_BIG_INPUT_W;
    output_dims.n = FULLY_CONNECTED_INT16_BIG_INPUT_BATCHES;
    output_dims.c = FULLY_CONNECTED_INT16_BIG_OUT_CH;

    fc_params.input_offset = 0;
    fc_params.filter_offset = 0;
    fc_params.output_offset = 0;
    fc_params.activation.min = FULLY_CONNECTED_INT16_BIG_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FULLY_CONNECTED_INT16_BIG_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FULLY_CONNECTED_INT16_BIG_OUTPUT_MULTIPLIER;
    quant_params.shift = FULLY_CONNECTED_INT16_BIG_OUTPUT_SHIFT;

    int32_t buf_size = riscv_fully_connected_s16_get_buffer_size(&filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_fully_connected_s16(&ctx,
                             &fc_params,
                             &quant_params,
                             &input_dims,
                             input_data,
                             &filter_dims,
                             kernel_data,
                             &bias_dims,
                             bias_data,
                             &output_dims,
                             output);

    read_perf_counters(&end_cycles, &end_inst);
    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t instr_count = end_inst - start_inst;
    uint32_t stack_used = measure_stack_usage();

    float time_sec = (float)cycle_count / clkFastfreq;
    float time_us = time_sec * 1e6f;

    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    printf("\n\r");
    if (validate_s16(output, output_ref, output_ref_size)) {
        printf("riscv_fully_connected_s16 (BIG) output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
    } else {
        printf("riscv_fully_connected_s16 (BIG) output validation FAILED\n\r");
    }
}

void fc_int16_slow_riscv_fully_connected_s16(void)
{
    int16_t output[FC_INT16_SLOW_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_fc_params fc_params;
    nmsis_nn_per_tensor_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const int64_t *bias_data = fc_int16_slow_biases;
    const int8_t *kernel_data = fc_int16_slow_weights;
    const int16_t *input_data = fc_int16_slow_input;
    const int16_t *output_ref = fc_int16_slow_output_ref;
    const int32_t output_ref_size = FC_INT16_SLOW_DST_SIZE;

    input_dims.n = FC_INT16_SLOW_INPUT_BATCHES;
    input_dims.w = FC_INT16_SLOW_INPUT_W;
    input_dims.h = FC_INT16_SLOW_INPUT_H;
    input_dims.c = FC_INT16_SLOW_IN_CH;
    filter_dims.n = FC_INT16_SLOW_ACCUMULATION_DEPTH;
    filter_dims.c = FC_INT16_SLOW_OUT_CH;
    filter_dims.h = FC_INT16_SLOW_INPUT_H;
    filter_dims.w = FC_INT16_SLOW_INPUT_W;
    output_dims.n = FC_INT16_SLOW_INPUT_BATCHES;
    output_dims.c = FC_INT16_SLOW_OUT_CH;

    fc_params.input_offset = 0;
    fc_params.filter_offset = 0;
    fc_params.output_offset = 0;
    fc_params.activation.min = FC_INT16_SLOW_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FC_INT16_SLOW_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FC_INT16_SLOW_OUTPUT_MULTIPLIER;
    quant_params.shift = FC_INT16_SLOW_OUTPUT_SHIFT;

    int32_t buf_size = riscv_fully_connected_s16_get_buffer_size(&filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_fully_connected_s16(&ctx,
                             &fc_params,
                             &quant_params,
                             &input_dims,
                             input_data,
                             &filter_dims,
                             kernel_data,
                             &bias_dims,
                             bias_data,
                             &output_dims,
                             output);

    read_perf_counters(&end_cycles, &end_inst);
    uint32_t cycle_count = end_cycles - start_cycles;
    uint32_t instr_count = end_inst - start_inst;
    uint32_t stack_used = measure_stack_usage();

    float time_sec = (float)cycle_count / clkFastfreq;
    float time_us = time_sec * 1e6f;

    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    printf("\n\r");
    if (validate_s16(output, output_ref, output_ref_size)) {
        printf("riscv_fully_connected_s16 (SLOW) output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
    } else {
        printf("riscv_fully_connected_s16 (SLOW) output validation FAILED\n\r");
    }
}

