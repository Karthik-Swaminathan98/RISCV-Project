#include "main.h"

void benchmark_sqrt(void) {
    printf("=== SQRT Benchmark ===\n\r");
    for (int i = 0; i < FIR_SIZES_COUNT; i++) {
        int N = FIR_SIZES[i];

        float32_t *in_f32  = malloc(N * sizeof(float32_t));
        float32_t *out_f32 = malloc(N * sizeof(float32_t));
        q15_t     *in_q15  = malloc(N * sizeof(q15_t));
        q15_t     *out_q15 = malloc(N * sizeof(q15_t));
        if (!in_f32||!out_f32||!in_q15||!out_q15) {
            printf("Mem alloc failed for SQRT N=%d\n\r", N);
            free(in_f32); free(out_f32);
            free(in_q15); free(out_q15);
            continue;
        }

        // Inputs: ramp [0..1]
        for (int j = 0; j < N; j++) {
            float32_t x   = (float32_t)j / (float32_t)(N - 1);
            in_f32[j]     = x;
            in_q15[j]     = (q15_t)(x * 0x7FFF);
        }

        // ---- f32 sqrt ----
    	fill_stack_pattern_to_sp();
    	reset_counters();
        unsigned int start_cycles, start_inst, end_cycles, end_inst;
        read_perf_counters(&start_cycles, &start_inst);

        for (int j = 0; j < N; j++) {
            riscv_sqrt_f32(in_f32[j], &out_f32[j]);
        }

        read_perf_counters(&end_cycles, &end_inst);
        uint32_t cycle_count = end_cycles - start_cycles;
        uint32_t instr_count = end_inst - start_inst;
        uint32_t stack_used = measure_stack_usage();
        float time_sec = (float)cycle_count / clkFastfreq;
        float time_us = time_sec * 1e6f;

        printf("\nSQRT f32 N=%d\n\r", N);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Estimated Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us\n\r", time_us);
        printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used);

        // ---- Q15 sqrt ----
    	fill_stack_pattern_to_sp();
    	reset_counters();
        read_perf_counters(&start_cycles, &start_inst);

        for (int j = 0; j < N; j++) {
            riscv_sqrt_q15(in_q15[j], &out_q15[j]);
        }

        read_perf_counters(&end_cycles, &end_inst);
        cycle_count = end_cycles - start_cycles;
        instr_count = end_inst - start_inst;
        stack_used = measure_stack_usage();
        time_sec = (float)cycle_count / clkFastfreq;
        time_us = time_sec * 1e6f;

        printf("\nSQRT Q15 N=%d\n\r", N);
        printf("Cycle Count: %lu\n\r", (unsigned long)cycle_count);
        printf("Estimated Instruction Count: %lu\n\r", instr_count);
        printf("Execution Time (approx): %.3f us\n\r", time_us);
        printf("Stack Used: %lu bytes\n\r", (unsigned long)stack_used);

//        if (N == 32) {
//            printf("\n--- Q15 Functional Verification (N=32) ---\n\r");
//            printf("Input (as float), Output (as float)\n\r");
//            for (int k = 0; k < N; k++) {
//                // CORRECT: Convert the Q15 integer back to a float for printing
//                float32_t in_val_as_float  = (float32_t)in_q15[k] / 32767.0f;
//                float32_t out_val_as_float = (float32_t)out_q15[k] / 32767.0f;
//                printf("%.4f, %.4f\n\r", in_val_as_float, out_val_as_float);
//            }
//        }

        free(in_f32);  free(out_f32);
        free(in_q15);  free(out_q15);
    }
    printf("=== SQRT Benchmark Done ===\n\r");
}
