#include <stdlib.h>
#include <riscv_nnfunctions.h>
//#include "../Include_ARM/dw_int16xint8/test_data.h"
#include "validate.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "stimer.h"
#include "../TestData_ARM/softmax_s8_s16/test_data.h"

static void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

static unsigned int read_cycle_counter() {
    return read_csr(NDS_MCYCLE);
}
#define REPEAT_NUM (2)

void softmax_s8_s16_riscv_softmax_s8_s16(void)
{
    const int32_t num_rows = SOFTMAX_S8_S16_NUM_ROWS;
    const int32_t row_size = SOFTMAX_S8_S16_ROW_SIZE;
    const int32_t mult = SOFTMAX_S8_S16_INPUT_MULT;
    const int32_t shift = SOFTMAX_S8_S16_INPUT_LEFT_SHIFT;
    const int32_t diff_min = SOFTMAX_S8_S16_DIFF_MIN;
    const int8_t *input_data = softmax_s8_s16_input;
    int16_t output[SOFTMAX_S8_S16_DST_SIZE];

    reset_cycle_count();
    // Measure cycles
    uint32_t start_cycles_s8_s16 = read_cycle_counter();

    riscv_softmax_s8_s16(input_data, num_rows, row_size, mult, shift, diff_min, output);

    uint32_t end_cycles_s8_s16 = read_cycle_counter();

	// Calculate cycle count
	uint32_t cycle_count_s8_s16 = end_cycles_s8_s16 - start_cycles_s8_s16;
	if (validate_s16(output, softmax_s8_s16_output_ref, SOFTMAX_S8_S16_DST_SIZE)) {
		printf("riscv_softmax_s8_s16 output validation PASSED\n\r");
		printf("Cycle Count for riscv_softmax_s8_s16: %lu\n\r", (unsigned long)cycle_count_s8_s16);
	} else {
		printf("riscv_softmax_s8_s16 output validation FAILED\n\r");
	}

}
