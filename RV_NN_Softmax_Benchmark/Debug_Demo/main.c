#include "main.h"

void softmax_riscv_softmax_s8();
void softmax_invalid_diff_min_riscv_softmax_s8();
void softmax_s16_riscv_softmax_s16();
void softmax_s8_s16_riscv_softmax_s8_s16();

uint32_t clkFastfreq = 0;

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);
    core_interrupt_disable();

    // Get the clock frequency
    clkFastfreq = get_clk_fast_freq();

    printf("\n\r");
    printf("-----Starting NMSIS-Softmax Functions benchmark-----\n\r");
    printf("\n\r");
    printf("CPU Clock Frequency: %lu Hz\n\r", clkFastfreq);
    printf("\n\r");

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
