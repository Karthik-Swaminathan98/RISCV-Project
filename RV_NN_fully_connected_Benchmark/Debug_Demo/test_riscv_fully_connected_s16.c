#include <stdlib.h>
#include <riscv_nnfunctions.h>
#include "../Include_ARM/fully_connected_int16/test_data.h"
#include "../Include_ARM/fc_int16_slow/test_data.h"
#include "../Include_ARM/fully_connected_int16_big/test_data.h"
#include "validate.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"

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

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();

	// Fill stack with a known pattern
	fill_stack_pattern_to_sp();

	// Measure cycles
	uint32_t start_cycles_s16 = read_cycle_counter();
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
	// Measure cycles
	uint32_t end_cycles_s16 = read_cycle_counter();

    // Measure stack usage
    uint32_t stack_used_s16 = measure_stack_usage();

	// Calculate cycle count
	uint32_t cycle_count_s16 = end_cycles_s16 - start_cycles_s16;

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    printf("\n\r");
	if (validate_s16(output, output_ref, output_ref_size)) {
		printf("riscv_fully_connected_s16 output validation PASSED\n\r");
		printf("Stack Used for riscv_fully_connected_s16: %lu bytes\n\r", (unsigned long)stack_used_s16);
		printf("Cycle Count for riscv_fully_connected_s16: %lu\n\r", (unsigned long)cycle_count_s16);
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

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();
    fill_stack_pattern_to_sp();

    uint32_t start_cycles = read_cycle_counter();
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
    uint32_t end_cycles = read_cycle_counter();
    uint32_t stack_used = measure_stack_usage();
    uint32_t cycle_count = end_cycles - start_cycles;

    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    printf("\n\r");
    if (validate_s16(output, output_ref, output_ref_size)) {
        printf("riscv_fully_connected_s16 (BIG) output validation PASSED\n\r");
        printf("Stack Used for riscv_fully_connected_s16 (BIG): %lu bytes\n\r", (unsigned long)stack_used);
        printf("Cycle Count for riscv_fully_connected_s16 (BIG): %lu\n\r", (unsigned long)cycle_count);
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

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();
    fill_stack_pattern_to_sp();

    uint32_t start_cycles = read_cycle_counter();
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
    uint32_t end_cycles = read_cycle_counter();
    uint32_t stack_used = measure_stack_usage();
    uint32_t cycle_count = end_cycles - start_cycles;

    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    printf("\n\r");
    if (validate_s16(output, output_ref, output_ref_size)) {
        printf("riscv_fully_connected_s16 (SLOW) output validation PASSED\n\r");
        printf("Stack Used for riscv_fully_connected_s16 (SLOW): %lu bytes\n\r", (unsigned long)stack_used);
        printf("Cycle Count for riscv_fully_connected_s16 (SLOW): %lu\n\r", (unsigned long)cycle_count);
    } else {
        printf("riscv_fully_connected_s16 (SLOW) output validation FAILED\n\r");
    }
}

