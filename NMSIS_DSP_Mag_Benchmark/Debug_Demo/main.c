#include "main.h"

void benchmark_f32(void);
void benchmark_q15(void);

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
    printf("-----Starting NMSIS-Magnitude benchmark-----\n\r");
    printf("\n\r");
    printf("CPU Clock Frequency: %lu Hz\n\r", clkFastfreq);
    printf("\n\r");

    // Perform benchmarks
    printf("*****Benchmarking NMSIS Cmag F32*****\n\r");
    //benchmark_f32();
    printf("\n\r");

    printf("*****Benchmarking NMSIS Cmag Q15*****\n\r");
    benchmark_q15();
    printf("\n\r");

    printf("All tests are completed.\n\r");
    printf("Finish NMSIS Magnitude Benchmark\n\r");

    return 0;
}
