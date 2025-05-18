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
void basic_riscv_convolve_s16();
void depthwise_2_riscv_depthwise_conv_s8();
void dw_int16xint8_riscv_depthwise_conv_s16();

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

	printf("\n\r");
    printf("Starting NMSIS-ConvolutionFunctions benchmark\n\r");
    delay_ms(20);
    basic_riscv_convolve_s8();
    basic_riscv_convolve_s16();
    depthwise_2_riscv_depthwise_conv_s8();
    dw_int16xint8_riscv_depthwise_conv_s16();
    printf("All tests are passed.\n\r");
    delay_ms(20);
	printf("Finish ConvolutionFunctions benchmark\n\r");
	delay_ms(20);
	return 0;
}
