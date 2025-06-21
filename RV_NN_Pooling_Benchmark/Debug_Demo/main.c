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


void avgpooling_riscv_avgpool_s8();
void avgpooling_1_riscv_avgpool_s8();
void avgpooling_2_riscv_avgpool_s8();
void avgpooling_int16_riscv_avgpool_s16();
void avgpooling_int16_1_riscv_avgpool_s16();
void avgpooling_int16_2_riscv_avgpool_s16();


extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

	printf("\n\r");
    printf("-----Starting NMSIS-Pooling Functions benchmark-----\n\r");
    printf("\n\r");
    printf("*****RISCV Average Pooling S8*****\n\r");
    avgpooling_riscv_avgpool_s8();
    avgpooling_1_riscv_avgpool_s8();
    avgpooling_2_riscv_avgpool_s8();
    printf("\n\r");
    printf("*****RISCV Average Pooling S16*****\n\r");
    avgpooling_int16_riscv_avgpool_s16();
    avgpooling_int16_1_riscv_avgpool_s16();
    avgpooling_int16_2_riscv_avgpool_s16();
    printf("All tests are passed.\n\r");
    delay_ms(20);
	printf("Finish Pooling Functions benchmark\n\r");
	delay_ms(20);
	return 0;
}
