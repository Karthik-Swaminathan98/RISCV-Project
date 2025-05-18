#include <stdlib.h>
#include <riscv_nnfunctions.h>
#include "../Include_ARM/fully_connected/test_data.h"
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

void fully_connected_riscv_fully_connected_s8(void)
{
    //const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[FULLY_CONNECTED_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_fc_params fc_params;
    nmsis_nn_per_tensor_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const int32_t *bias_data = fully_connected_biases;
    const int8_t *kernel_data = fully_connected_weights;
    const int8_t *input_data = fully_connected_input;
    const int8_t *output_ref = fully_connected_output_ref;
    const int32_t output_ref_size = FULLY_CONNECTED_DST_SIZE;

    input_dims.n = FULLY_CONNECTED_INPUT_BATCHES;
    input_dims.w = FULLY_CONNECTED_INPUT_W;
    input_dims.h = FULLY_CONNECTED_INPUT_H;
    input_dims.c = FULLY_CONNECTED_IN_CH;
    filter_dims.n = FULLY_CONNECTED_ACCUMULATION_DEPTH;
    filter_dims.c = FULLY_CONNECTED_OUT_CH;
    output_dims.n = FULLY_CONNECTED_INPUT_BATCHES;
    output_dims.c = FULLY_CONNECTED_OUT_CH;

    fc_params.input_offset = FULLY_CONNECTED_INPUT_OFFSET;
    fc_params.filter_offset = 0;
    fc_params.output_offset = FULLY_CONNECTED_OUTPUT_OFFSET;
    fc_params.activation.min = FULLY_CONNECTED_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FULLY_CONNECTED_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FULLY_CONNECTED_OUTPUT_MULTIPLIER;
    quant_params.shift = FULLY_CONNECTED_OUTPUT_SHIFT;

//    generate_rand_s8(fully_connected_input, FULLY_CONNECTED_INPUT_SIZE);
//    generate_rand_s8(fully_connected_weights, FULLY_CONNECTED_KERNEL_SIZE);

    const int32_t buf_size = riscv_fully_connected_s8_get_buffer_size(&filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    reset_cycle_count();
    // Measure cycles
    uint32_t start_cycles_s8 = read_cycle_counter();
    riscv_fully_connected_s8(&ctx,
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
    uint32_t end_cycles_s8 = read_cycle_counter();

    // Calculate cycle count
    uint32_t cycle_count_s8 = end_cycles_s8 - start_cycles_s8;

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
	if (validate(output, output_ref, output_ref_size)) {
		printf("riscv_fully_connected_s8 output validation PASSED\n\r");
		printf("Cycle Count for riscv_fully_connected_s8: %lu\n\r", (unsigned long)cycle_count_s8);
	} else {
		printf("riscv_fully_connected_s8 output validation FAILED\n\r");
	}
}
