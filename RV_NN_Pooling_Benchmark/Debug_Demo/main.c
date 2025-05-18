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
void avgpooling_int16_riscv_avgpool_s16();


extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

	printf("\n\r");
    printf("Starting NMSIS-Pooling Functions benchmark\n\r");
    delay_ms(20);
    avgpooling_riscv_avgpool_s8();
    avgpooling_int16_riscv_avgpool_s16();
    printf("All tests are passed.\n\r");
    delay_ms(20);
	printf("Finish Pooling Functions benchmark\n\r");
	delay_ms(20);
	return 0;
}
