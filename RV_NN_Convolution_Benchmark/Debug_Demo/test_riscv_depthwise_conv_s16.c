#include "main.h"
#include "../TestData/dw_int16xint8/test_data.h"
#include "../TestData/dw_int16xint8_dilation/test_data.h"
#include "../TestData/dw_int16xint8_mult4/test_data.h"

void dw_int16xint8_riscv_depthwise_conv_s16(void)
{
    int16_t output[DW_INT16XINT8_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_dw_conv_params dw_conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims = {};
    nmsis_nn_dims output_dims;

    const int64_t *bias_data = dw_int16xint8_biases;
    const int16_t *input_data = dw_int16xint8_input;
    const int8_t *kernel_data = dw_int16xint8_weights;
    const int16_t *output_ref = dw_int16xint8_output_ref;
    const int32_t output_ref_size = DW_INT16XINT8_DST_SIZE;

    input_dims.n = DW_INT16XINT8_INPUT_BATCHES;
    input_dims.w = DW_INT16XINT8_INPUT_W;
    input_dims.h = DW_INT16XINT8_INPUT_H;
    input_dims.c = DW_INT16XINT8_IN_CH;
    filter_dims.w = DW_INT16XINT8_FILTER_X;
    filter_dims.h = DW_INT16XINT8_FILTER_Y;
    output_dims.w = DW_INT16XINT8_OUTPUT_W;
    output_dims.h = DW_INT16XINT8_OUTPUT_H;
    output_dims.c = DW_INT16XINT8_OUT_CH;

    dw_conv_params.padding.w = DW_INT16XINT8_PAD_X;
    dw_conv_params.padding.h = DW_INT16XINT8_PAD_Y;
    dw_conv_params.stride.w = DW_INT16XINT8_STRIDE_X;
    dw_conv_params.stride.h = DW_INT16XINT8_STRIDE_Y;
    dw_conv_params.dilation.w = DW_INT16XINT8_DILATION_X;
    dw_conv_params.dilation.h = DW_INT16XINT8_DILATION_Y;

    dw_conv_params.ch_mult = DW_INT16XINT8_CH_MULT;
    dw_conv_params.input_offset = DW_INT16XINT8_INPUT_OFFSET;
    dw_conv_params.output_offset = DW_INT16XINT8_OUTPUT_OFFSET;
    dw_conv_params.activation.min = DW_INT16XINT8_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = DW_INT16XINT8_OUT_ACTIVATION_MAX;

    quant_params.multiplier = (int32_t *)dw_int16xint8_output_mult;
    quant_params.shift = (int32_t *)dw_int16xint8_output_shift;

    const int32_t buf_size =
        riscv_depthwise_conv_wrapper_s16_get_buffer_size(&dw_conv_params, &input_dims, &filter_dims, &output_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_depthwise_conv_wrapper_s16(&ctx,
                                    &dw_conv_params,
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
        printf("dw_int16xint8_riscv_depthwise_conv_s16 output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
    } else {
        printf("dw_int16xint8_riscv_depthwise_conv_s16 output validation FAILED\n\r");
    }
}

void dw_int16xint8_mult4_riscv_depthwise_conv_s16(void)
{
    int16_t output[DW_INT16XINT8_MULT4_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_dw_conv_params dw_conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims = {};
    nmsis_nn_dims output_dims;

    const int64_t *bias_data = dw_int16xint8_mult4_biases;
    const int16_t *input_data = dw_int16xint8_mult4_input;
    const int8_t *kernel_data = dw_int16xint8_mult4_weights;
    const int16_t *output_ref = dw_int16xint8_mult4_output_ref;
    const int32_t output_ref_size = DW_INT16XINT8_MULT4_DST_SIZE;

    input_dims.n = DW_INT16XINT8_MULT4_INPUT_BATCHES;
    input_dims.w = DW_INT16XINT8_MULT4_INPUT_W;
    input_dims.h = DW_INT16XINT8_MULT4_INPUT_H;
    input_dims.c = DW_INT16XINT8_MULT4_IN_CH;
    filter_dims.w = DW_INT16XINT8_MULT4_FILTER_X;
    filter_dims.h = DW_INT16XINT8_MULT4_FILTER_Y;
    output_dims.w = DW_INT16XINT8_MULT4_OUTPUT_W;
    output_dims.h = DW_INT16XINT8_MULT4_OUTPUT_H;
    output_dims.c = DW_INT16XINT8_MULT4_OUT_CH;

    dw_conv_params.padding.w = DW_INT16XINT8_MULT4_PAD_X;
    dw_conv_params.padding.h = DW_INT16XINT8_MULT4_PAD_Y;
    dw_conv_params.stride.w = DW_INT16XINT8_MULT4_STRIDE_X;
    dw_conv_params.stride.h = DW_INT16XINT8_MULT4_STRIDE_Y;
    dw_conv_params.dilation.w = DW_INT16XINT8_MULT4_DILATION_X;
    dw_conv_params.dilation.h = DW_INT16XINT8_MULT4_DILATION_Y;

    dw_conv_params.ch_mult = DW_INT16XINT8_MULT4_CH_MULT;
    dw_conv_params.input_offset = DW_INT16XINT8_MULT4_INPUT_OFFSET;
    dw_conv_params.output_offset = DW_INT16XINT8_MULT4_OUTPUT_OFFSET;
    dw_conv_params.activation.min = DW_INT16XINT8_MULT4_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = DW_INT16XINT8_MULT4_OUT_ACTIVATION_MAX;

    quant_params.multiplier = (int32_t *)dw_int16xint8_mult4_output_mult;
    quant_params.shift = (int32_t *)dw_int16xint8_mult4_output_shift;

    const int32_t buf_size =
        riscv_depthwise_conv_wrapper_s16_get_buffer_size(&dw_conv_params, &input_dims, &filter_dims, &output_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_depthwise_conv_wrapper_s16(&ctx,
                                    &dw_conv_params,
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
        printf("dw_int16xint8_mult4_riscv_depthwise_conv_s16 output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
    } else {
        printf("dw_int16xint8_mult4_riscv_depthwise_conv_s16 output validation FAILED\n\r");
    }
}

void dw_int16xint8_dilation_riscv_depthwise_conv_s16(void)
{
    int16_t output[DW_INT16XINT8_DILATION_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_dw_conv_params dw_conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims = {};
    nmsis_nn_dims output_dims;

    const int64_t *bias_data = dw_int16xint8_dilation_biases;
    const int16_t *input_data = dw_int16xint8_dilation_input;
    const int8_t *kernel_data = dw_int16xint8_dilation_weights;
    const int16_t *output_ref = dw_int16xint8_dilation_output_ref;
    const int32_t output_ref_size = DW_INT16XINT8_DILATION_DST_SIZE;

    input_dims.n = DW_INT16XINT8_DILATION_INPUT_BATCHES;
    input_dims.w = DW_INT16XINT8_DILATION_INPUT_W;
    input_dims.h = DW_INT16XINT8_DILATION_INPUT_H;
    input_dims.c = DW_INT16XINT8_DILATION_IN_CH;
    filter_dims.w = DW_INT16XINT8_DILATION_FILTER_X;
    filter_dims.h = DW_INT16XINT8_DILATION_FILTER_Y;
    output_dims.w = DW_INT16XINT8_DILATION_OUTPUT_W;
    output_dims.h = DW_INT16XINT8_DILATION_OUTPUT_H;
    output_dims.c = DW_INT16XINT8_DILATION_OUT_CH;

    dw_conv_params.padding.w = DW_INT16XINT8_DILATION_PAD_X;
    dw_conv_params.padding.h = DW_INT16XINT8_DILATION_PAD_Y;
    dw_conv_params.stride.w = DW_INT16XINT8_DILATION_STRIDE_X;
    dw_conv_params.stride.h = DW_INT16XINT8_DILATION_STRIDE_Y;
    dw_conv_params.dilation.w = DW_INT16XINT8_DILATION_DILATION_X;
    dw_conv_params.dilation.h = DW_INT16XINT8_DILATION_DILATION_Y;

    dw_conv_params.ch_mult = DW_INT16XINT8_DILATION_CH_MULT;
    dw_conv_params.input_offset = DW_INT16XINT8_DILATION_INPUT_OFFSET;
    dw_conv_params.output_offset = DW_INT16XINT8_DILATION_OUTPUT_OFFSET;
    dw_conv_params.activation.min = DW_INT16XINT8_DILATION_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = DW_INT16XINT8_DILATION_OUT_ACTIVATION_MAX;

    quant_params.multiplier = (int32_t *)dw_int16xint8_dilation_output_mult;
    quant_params.shift = (int32_t *)dw_int16xint8_dilation_output_shift;

    const int32_t buf_size =
        riscv_depthwise_conv_wrapper_s16_get_buffer_size(&dw_conv_params, &input_dims, &filter_dims, &output_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    reset_counters();
    fill_stack_pattern_to_sp();
    unsigned int start_cycles, start_inst, end_cycles, end_inst;
    read_perf_counters(&start_cycles, &start_inst);

    riscv_depthwise_conv_wrapper_s16(&ctx,
                                    &dw_conv_params,
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
        printf("dw_int16xint8_dilation_riscv_depthwise_conv_s16 output validation PASSED\n\r");
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us \n\r", time_us);
        printf("Stack Used: %lu bytes\n\r\n", (unsigned long)stack_used);
    } else {
        printf("dw_int16xint8_dilation_riscv_depthwise_conv_s16 output validation FAILED\n\r");
    }
}

