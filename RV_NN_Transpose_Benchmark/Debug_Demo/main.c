#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include "riscv_math.h"
#include "riscv_nnfunctions.h"

#include "validate.h"


void transpose_default_riscv_transpose_s8();
void transpose_3dim2_riscv_transpose_s8();
void transpose_matrix_riscv_transpose_s8();
void transpose_conv_1_riscv_transpose_conv_s8();
void transpose_conv_2_riscv_transpose_conv_s8();
void transpose_conv_3_riscv_transpose_conv_s8();


extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

	printf("\n\r");
    printf("-----Starting NMSIS-Transpose Functions benchmark-----\n\r");
    printf("\n\r");
    printf("*****riscv Transpose S8*****\n\r");
    transpose_default_riscv_transpose_s8();
    transpose_3dim2_riscv_transpose_s8();
    transpose_matrix_riscv_transpose_s8();
    printf("\n\r");
    printf("*****riscv Transpose Convolution S8*****\n\r");
    transpose_conv_1_riscv_transpose_conv_s8();
    transpose_conv_2_riscv_transpose_conv_s8();
    transpose_conv_3_riscv_transpose_conv_s8();
    printf("All tests are passed.\n\r");
    delay_ms(20);
	printf("Finish Transpose Functions benchmark\n\r");
	delay_ms(20);
	return 0;
}
