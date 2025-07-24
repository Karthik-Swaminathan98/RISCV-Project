#include "main.h"

void benchmark_sqrt(void);
void benchmark_atan2(void);
void benchmark_sin_cos(void);

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
    printf("-----Starting ANDES-Math benchmark-----\n\r");
    printf("\n\r");
    printf("CPU Clock Frequency: %lu Hz\n\r", clkFastfreq);
    printf("\n\r");

    printf("*****Benchmarking ANDES SQRT *****\n\r");
    benchmark_sqrt();
    printf("\n\r");

    printf("*****Benchmarking ANDES ATAN2 *****\n\r");
    benchmark_atan2();
    printf("\n\r");

    printf("*****Benchmarking ANDES SIN/COS *****\n\r");
    benchmark_sin_cos();
    printf("\n\r");

    printf("All tests are completed.\n\r");
    printf("Finish ANDES Math Benchmark\n\r");

    return 0;
}
