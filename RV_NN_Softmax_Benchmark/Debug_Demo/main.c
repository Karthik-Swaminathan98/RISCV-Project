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


void softmax_riscv_softmax_s8();
void softmax_invalid_diff_min_riscv_softmax_s8();
void softmax_s16_riscv_softmax_s16();
void softmax_s8_s16_riscv_softmax_s8_s16();


extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

	printf("\n\r");
    printf("-----Starting NMSIS-Softmax Functions benchmark-----\n\r");
    printf("*****RISCV Softmax Functions S8*****\n\r");
    softmax_riscv_softmax_s8();
    softmax_invalid_diff_min_riscv_softmax_s8();
    printf("\n\r");
    printf("*****RISCV Softmax Functions S16*****\n\r");
    softmax_s16_riscv_softmax_s16();
    printf("\n\r");
    printf("*****RISCV Softmax Functions S8_S16*****\n\r");
    softmax_s8_s16_riscv_softmax_s8_s16();
    printf("All tests are passed.\n\r");
    delay_ms(20);
	printf("Finish Softmax Functions benchmark\n\r");
	delay_ms(20);
	return 0;
}
