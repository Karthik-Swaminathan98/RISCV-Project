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


void fully_connected_riscv_fully_connected_s8();
void fully_connected_int16_riscv_fully_connected_s16();

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

	printf("\n\r");
    printf("Starting NMSIS-Fully Connected Functions benchmark\n\r");
    delay_ms(20);
    fully_connected_riscv_fully_connected_s8();
	fully_connected_int16_riscv_fully_connected_s16();;
    printf("All tests are passed.\n\r");
    delay_ms(20);
	printf("Finish Fully Connected Functions benchmark\n\r");
	delay_ms(20);
	return 0;
}
