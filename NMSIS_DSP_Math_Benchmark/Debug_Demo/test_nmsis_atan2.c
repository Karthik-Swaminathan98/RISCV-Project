#include "main.h"

void benchmark_atan2(void) {
    printf("=== ATAN2 Benchmark ===\n\r");
    for (int i = 0; i < FIR_SIZES_COUNT; i++) {
        int N = FIR_SIZES[i];

        float32_t *y_f32  = malloc(N*sizeof(float32_t));
        float32_t *x_f32  = malloc(N*sizeof(float32_t));
        float32_t *out_f32= malloc(N*sizeof(float32_t));
        q15_t     *y_q15  = malloc(N*sizeof(q15_t));
        q15_t     *x_q15  = malloc(N*sizeof(q15_t));
        q15_t     *out_q15= malloc(N*sizeof(q15_t));
        if (!y_f32||!x_f32||!out_f32||!y_q15||!x_q15||!out_q15) {
            printf("Mem alloc failed for ATAN2 N=%d\n\r", N);
            free(y_f32); free(x_f32); free(out_f32);
            free(y_q15); free(x_q15); free(out_q15);
            continue;
        }

        // Inputs: rotating vector (sin,cos)
        for (int j = 0; j < N; j++) {
            float32_t a = 2*M_PI * j / N;
            y_f32[j] =  sinf(a);
            x_f32[j] =  cosf(a);
            y_q15[j] = (q15_t)(sinf(a)*0x7FFF);
            x_q15[j] = (q15_t)(cosf(a)*0x7FFF);
        }

        // ---- f32 atan2 ----
    	fill_stack_pattern_to_sp();
    	reset_counters();
        unsigned int start_cycles, start_inst, end_cycles, end_inst;
        read_perf_counters(&start_cycles, &start_inst);

        for (int j = 0; j < N; j++) {
            riscv_atan2_f32(y_f32[j], x_f32[j], &out_f32[j]);
        }

        read_perf_counters(&end_cycles, &end_inst);
        uint32_t cycle_count = end_cycles - start_cycles;
        uint32_t instr_count = end_inst - start_inst;
        uint32_t stack_used = measure_stack_usage();
        float time_sec = (float)cycle_count / clkFastfreq;
        float time_us = time_sec * 1e6f;

        printf("\nATAN2 f32 N=%d\n\r", N);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Estimated Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us\n\r", time_us);
        printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used);

        // ---- Q15 atan2 ----
    	fill_stack_pattern_to_sp();
    	reset_counters();
        read_perf_counters(&start_cycles, &start_inst);

        for (int j = 0; j < N; j++) {
            riscv_atan2_q15(y_q15[j], x_q15[j], &out_q15[j]);
        }

        read_perf_counters(&end_cycles, &end_inst);
        cycle_count = end_cycles - start_cycles;
        instr_count = end_inst - start_inst;
        stack_used = measure_stack_usage();
        time_sec = (float)cycle_count / clkFastfreq;
        time_us = time_sec * 1e6f;

        printf("\nATAN2 Q15 N=%d\n\r", N);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Estimated Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us\n\r", time_us);
        printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used);

        free(y_f32); free(x_f32); free(out_f32);
        free(y_q15); free(x_q15); free(out_q15);
    }
    printf("=== ATAN2 Benchmark Done ===\n\r");
}
