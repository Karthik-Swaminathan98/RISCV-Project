#include <stdlib.h>
#include <riscv_nnfunctions.h>
//#include "../Include_riscv/dw_int16xint8/test_data.h"
#include "validate.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"
#include "../TestData_ARM/avgpooling/test_data.h"
#include "../TestData_ARM/avgpooling_1/test_data.h"
#include "../TestData_ARM/avgpooling_2/test_data.h"

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

void avgpooling_riscv_avgpool_s8(void)
{
    //const riscv_nmsis_nn_status expected = riscv_nmsis_NN_SUCCESS;
    int8_t output[AVGPOOLING_OUTPUT_W * AVGPOOLING_OUTPUT_H * AVGPOOLING_BATCH_SIZE * AVGPOOLING_OUTPUT_C] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_pool_params pool_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_data = avgpooling_input_tensor;

    input_dims.n = AVGPOOLING_BATCH_SIZE;
    input_dims.w = AVGPOOLING_INPUT_W;
    input_dims.h = AVGPOOLING_INPUT_H;
    input_dims.c = AVGPOOLING_INPUT_C;
    filter_dims.w = AVGPOOLING_FILTER_W;
    filter_dims.h = AVGPOOLING_FILTER_H;
    output_dims.w = AVGPOOLING_OUTPUT_W;
    output_dims.h = AVGPOOLING_OUTPUT_H;
    output_dims.c = AVGPOOLING_OUTPUT_C;

    pool_params.padding.w = AVGPOOLING_PADDING_W;
    pool_params.padding.h = AVGPOOLING_PADDING_H;
    pool_params.stride.w = AVGPOOLING_STRIDE_W;
    pool_params.stride.h = AVGPOOLING_STRIDE_H;

    pool_params.activation.min = AVGPOOLING_ACTIVATION_MIN;
    pool_params.activation.max = AVGPOOLING_ACTIVATION_MAX;

    ctx.size = riscv_avgpool_s8_get_buffer_size(AVGPOOLING_OUTPUT_W, AVGPOOLING_INPUT_C);
    ctx.buf = malloc(ctx.size);

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();

	// Fill stack with a known pattern
	fill_stack_pattern_to_sp();

    // Measure cycles
    uint32_t start_cycles_s8 = read_cycle_counter();
    riscv_avgpool_s8(&ctx, &pool_params, &input_dims, input_data, &filter_dims, &output_dims, output);
    // Measure cycles
    uint32_t end_cycles_s8 = read_cycle_counter();

    // Measure stack usage
    uint32_t stack_used_s8 = measure_stack_usage();

    // Calculate cycle count
    uint32_t cycle_count_s8 = end_cycles_s8 - start_cycles_s8;
    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    printf("\n\r");
	if (validate(output,
            avgpooling_output,
            AVGPOOLING_OUTPUT_W * AVGPOOLING_OUTPUT_H * AVGPOOLING_BATCH_SIZE * AVGPOOLING_OUTPUT_C)) {
		printf("riscv_avgpool_s8 output validation PASSED\n\r");
		printf("Stack Used for riscv_avgpool_s8: %lu bytes\n\r", (unsigned long)stack_used_s8);
		printf("Cycle Count for riscv_avgpool_s8: %lu\n\r", (unsigned long)cycle_count_s8);
	} else {
		printf("riscv_avgpool_s8 output validation FAILED\n\r");
	}
}

void avgpooling_1_riscv_avgpool_s8(void)
{
    int8_t output[AVGPOOLING_1_OUTPUT_W * AVGPOOLING_1_OUTPUT_H * AVGPOOLING_1_BATCH_SIZE * AVGPOOLING_1_OUTPUT_C] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_pool_params pool_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_data = avgpooling_1_input_tensor;

    input_dims.n = AVGPOOLING_1_BATCH_SIZE;
    input_dims.w = AVGPOOLING_1_INPUT_W;
    input_dims.h = AVGPOOLING_1_INPUT_H;
    input_dims.c = AVGPOOLING_1_INPUT_C;
    filter_dims.w = AVGPOOLING_1_FILTER_W;
    filter_dims.h = AVGPOOLING_1_FILTER_H;
    output_dims.w = AVGPOOLING_1_OUTPUT_W;
    output_dims.h = AVGPOOLING_1_OUTPUT_H;
    output_dims.c = AVGPOOLING_1_OUTPUT_C;

    pool_params.padding.w = AVGPOOLING_1_PADDING_W;
    pool_params.padding.h = AVGPOOLING_1_PADDING_H;
    pool_params.stride.w = AVGPOOLING_1_STRIDE_W;
    pool_params.stride.h = AVGPOOLING_1_STRIDE_H;

    pool_params.activation.min = AVGPOOLING_1_ACTIVATION_MIN;
    pool_params.activation.max = AVGPOOLING_1_ACTIVATION_MAX;

    ctx.size = riscv_avgpool_s8_get_buffer_size(AVGPOOLING_1_OUTPUT_W, AVGPOOLING_1_INPUT_C);
    ctx.buf = malloc(ctx.size);

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();
    fill_stack_pattern_to_sp();

    uint32_t start_cycles = read_cycle_counter();
    riscv_nmsis_nn_status result = riscv_avgpool_s8(&ctx, &pool_params, &input_dims, input_data,
                                                &filter_dims, &output_dims, output);
    uint32_t end_cycles = read_cycle_counter();
    uint32_t stack_used = measure_stack_usage();
    uint32_t cycle_count = end_cycles - start_cycles;

    if (ctx.buf) {
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    printf("\n\r");
    if (result == RISCV_NMSIS_NN_SUCCESS && validate(output, avgpooling_1_output,
        AVGPOOLING_1_OUTPUT_W * AVGPOOLING_1_OUTPUT_H * AVGPOOLING_1_BATCH_SIZE * AVGPOOLING_1_OUTPUT_C)) {
        printf("avgpooling_1_riscv_avgpool_s8 output validation PASSED\n\r");
        printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
    } else {
        printf("avgpooling_1_riscv_avgpool_s8 output validation FAILED\n\r");
    }
}

void avgpooling_2_riscv_avgpool_s8(void)
{
    int8_t output[AVGPOOLING_2_OUTPUT_W * AVGPOOLING_2_OUTPUT_H * AVGPOOLING_2_BATCH_SIZE * AVGPOOLING_2_OUTPUT_C] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_pool_params pool_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_data = avgpooling_2_input_tensor;

    input_dims.n = AVGPOOLING_2_BATCH_SIZE;
    input_dims.w = AVGPOOLING_2_INPUT_W;
    input_dims.h = AVGPOOLING_2_INPUT_H;
    input_dims.c = AVGPOOLING_2_INPUT_C;
    filter_dims.w = AVGPOOLING_2_FILTER_W;
    filter_dims.h = AVGPOOLING_2_FILTER_H;
    output_dims.w = AVGPOOLING_2_OUTPUT_W;
    output_dims.h = AVGPOOLING_2_OUTPUT_H;
    output_dims.c = AVGPOOLING_2_OUTPUT_C;

    pool_params.padding.w = AVGPOOLING_2_PADDING_W;
    pool_params.padding.h = AVGPOOLING_2_PADDING_H;
    pool_params.stride.w = AVGPOOLING_2_STRIDE_W;
    pool_params.stride.h = AVGPOOLING_2_STRIDE_H;

    pool_params.activation.min = AVGPOOLING_2_ACTIVATION_MIN;
    pool_params.activation.max = AVGPOOLING_2_ACTIVATION_MAX;

    ctx.size = riscv_avgpool_s8_get_buffer_size(AVGPOOLING_2_OUTPUT_W, AVGPOOLING_2_INPUT_C);
    ctx.buf = malloc(ctx.size);

    // Get stack top and calculate stack limit
    stack_top = (uint32_t)&_STACK_TOP;
    stack_limit = stack_top - STACK_SIZE;

    reset_cycle_count();
    fill_stack_pattern_to_sp();

    uint32_t start_cycles = read_cycle_counter();
    riscv_nmsis_nn_status result = riscv_avgpool_s8(&ctx, &pool_params, &input_dims, input_data,
                                                &filter_dims, &output_dims, output);
    uint32_t end_cycles = read_cycle_counter();
    uint32_t stack_used = measure_stack_usage();
    uint32_t cycle_count = end_cycles - start_cycles;

    if (ctx.buf) {
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    printf("\n\r");
    if (result == RISCV_NMSIS_NN_SUCCESS && validate(output, avgpooling_2_output,
        AVGPOOLING_2_OUTPUT_W * AVGPOOLING_2_OUTPUT_H * AVGPOOLING_2_BATCH_SIZE * AVGPOOLING_2_OUTPUT_C)) {
        printf("avgpooling_2_riscv_avgpool_s8 output validation PASSED\n\r");
        printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
    } else {
        printf("avgpooling_2_riscv_avgpool_s8 output validation FAILED\n\r");
    }
}
