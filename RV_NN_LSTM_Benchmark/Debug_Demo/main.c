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

void lstm_1();
void lstm_2();
void lstm_one_time_step();
void lstm_1_s16();
void lstm_2_s16();
void lstm_one_time_step_s16();


extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

	printf("\n\r");
    printf("-----Starting NMSIS-LSTM Functions benchmark-----\n\r");
    printf("\n\r");
    printf("*****RISCV LSTM S8*****\n\r");;
    lstm_1();
    lstm_2();
    lstm_one_time_step();
    printf("\n\r");
    printf("*****RISCV LSTM S16*****\n\r");
    lstm_1_s16();
    lstm_2_s16();
    lstm_one_time_step_s16();
    printf("All tests are passed.\n\r");
    delay_ms(20);
	printf("Finish LSTM Functions benchmark\n\r");
	delay_ms(20);
	return 0;
}
