#include "main.h"

void transpose_default_riscv_transpose_s8();
void transpose_3dim2_riscv_transpose_s8();
void transpose_matrix_riscv_transpose_s8();
void transpose_conv_1_riscv_transpose_conv_s8();
void transpose_conv_2_riscv_transpose_conv_s8();
void transpose_conv_3_riscv_transpose_conv_s8();

uint32_t clkFastfreq = 0;;

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);
    core_interrupt_disable();

    // Get the clock frequency
    clkFastfreq = get_clk_fast_freq();

    printf("\n\r");
    printf("-----Starting NMSIS-Transpose Functions benchmark-----\n\r");
    printf("\n\r");
    printf("CPU Clock Frequency: %lu Hz\n\r", clkFastfreq);
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
