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

void basic_riscv_convolve_s8();
void conv_2x2_dilation_riscv_convolve_s8();
void conv_3x3_dilation_5x5_input_riscv_convolve_s8();;
void basic_riscv_convolve_s16();
void int16xint8_dilation_1_riscv_convolve_s16();
void int16xint8xint32_1_riscv_convolve_s16();
void depthwise_2_riscv_depthwise_conv_s8();
void depthwise_mult_batches_riscv_depthwise_conv_s8();
void depthwise_dilation_riscv_depthwise_conv_s8();
void dw_int16xint8_riscv_depthwise_conv_s16();
void dw_int16xint8_mult4_riscv_depthwise_conv_s16();
void dw_int16xint8_dilation_riscv_depthwise_conv_s16();

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);
	printf("\n\r");
    printf("-----Starting NMSIS-ConvolutionFunctions benchmark-----\n\r");
    printf("\n\r");
    printf("*****RISCV CONVOLVE WRAPPER S8*****\n\r");
    basic_riscv_convolve_s8();
    conv_2x2_dilation_riscv_convolve_s8();
    conv_3x3_dilation_5x5_input_riscv_convolve_s8();
    printf("\n\r");
    printf("*****RISCV CONVOLVE WRAPPER S16*****\n\r");
    basic_riscv_convolve_s16();
    int16xint8_dilation_1_riscv_convolve_s16();
    int16xint8xint32_1_riscv_convolve_s16();
    printf("\n\r");
    printf("*****RISCV DEPTHWISE CONVOLVE WRAPPER S8*****\n\r");
    depthwise_2_riscv_depthwise_conv_s8();
    depthwise_mult_batches_riscv_depthwise_conv_s8();
    depthwise_dilation_riscv_depthwise_conv_s8();
    printf("\n\r");
    printf("*****RISCV DEPTHWISE CONVOLVE WRAPPER S16*****\n\r");
    dw_int16xint8_riscv_depthwise_conv_s16();
    dw_int16xint8_mult4_riscv_depthwise_conv_s16();
    dw_int16xint8_dilation_riscv_depthwise_conv_s16();
	printf("Finish ConvolutionFunctions benchmark\n\r");
	delay_ms(20);
	return 0;
}
