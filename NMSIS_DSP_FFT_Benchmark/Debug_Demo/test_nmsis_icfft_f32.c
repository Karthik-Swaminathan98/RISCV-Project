#include "main.h"

void benchmark_ifft_f32() {
    for (int size_idx = 0; size_idx < FFT_SIZES_COUNT; size_idx++) {
        int N = FFT_SIZES[size_idx];

        float32_t* input = (float32_t*)malloc(2 * N * sizeof(float32_t));
        float32_t* freq_domain = (float32_t*)malloc(2 * N * sizeof(float32_t));
        float32_t* time_domain_ref = (float32_t*)malloc(2 * N * sizeof(float32_t));
        if (!input || !freq_domain || !time_domain_ref) {
            printf("Memory allocation failed for FFT size N = %d\n\r", N);
            continue;
        }

        // Generate original time domain signal
        generate_sine_wave_f32(time_domain_ref, N, SINE_FREQ, SAMPLING_FREQ);

        // Forward FFT to create freq_domain input for IFFT
        memcpy(input, time_domain_ref, 2 * N * sizeof(float32_t));
        riscv_cfft_instance_f32 fft_instance_fwd;
        riscv_cfft_init_f32(&fft_instance_fwd, N);
        riscv_cfft_f32(&fft_instance_fwd, input, 0, 1);
        memcpy(freq_domain, input, 2 * N * sizeof(float32_t));  // Save freq domain result

        riscv_cfft_instance_f32 fft_instance_inv;
        if (riscv_cfft_init_f32(&fft_instance_inv, N) != RISCV_MATH_SUCCESS) {
            printf("IFFT init failed for N = %d\n", N);
            free(input); free(freq_domain); free(time_domain_ref);
            continue;
        }

        uint32_t cycle_counts[NUM_EXECUTIONS];
        uint32_t instr_counts[NUM_EXECUTIONS];
        uint32_t stack_usages[NUM_EXECUTIONS];
        float exec_time_us_values[NUM_EXECUTIONS];

        int signals_consistent = 1;
        printf("\nIFFT Size: %d\n", N);

        for (int execution = 0; execution < NUM_EXECUTIONS; execution++) {
            // Use freq_domain as input to IFFT
            memcpy(input, freq_domain, 2 * N * sizeof(float32_t));

            fill_stack_pattern_to_sp();
            reset_counters();
            unsigned int start_cycles, start_inst, end_cycles, end_inst;
            read_perf_counters(&start_cycles, &start_inst);

            // The core: **inverse** FFT (ifftFlag = 1)
            riscv_cfft_f32(&fft_instance_inv, input, 1, 1);

            read_perf_counters(&end_cycles, &end_inst);
            uint32_t cycle_count = end_cycles - start_cycles;
            uint32_t instr_count = end_inst - start_inst;
            uint32_t stack_used = measure_stack_usage();
            float time_sec = (float)cycle_count / clkFastfreq;
            float time_us = time_sec * 1e6f;

            cycle_counts[execution] = cycle_count;
            instr_counts[execution] = instr_count;
            stack_usages[execution] = stack_used;
            exec_time_us_values[execution] = time_us;

            // Consistency check: compare time-domain output to reference
            if (execution == 0) {
                // You may print a few output samples if you want:
                // for (int i = 0; i < N; i++) printf("%d: %.4f (ref: %.4f)\n\r", i, input[2*i], time_domain_ref[2*i]);
            } else {
                for (int i = 0; i < N; i++) {
                    if (fabsf(input[2 * i] - time_domain_ref[2 * i]) > 1e-3) { // Only real part (sine wave)
                        signals_consistent = 0;
                        break;
                    }
                }
            }
        }

        if (signals_consistent) {
            printf("All IFFT outputs are consistent across executions for FFT size N = %d\n\r", N);
        } else {
            printf("Inconsistent IFFT outputs detected for FFT size N = %d\n\r", N);
        }

        calculate_averages(cycle_counts, instr_counts, exec_time_us_values, stack_usages, NUM_EXECUTIONS);

        free(input);
        free(freq_domain);
        free(time_domain_ref);
    }

    printf("\nBenchmark completed for NMSIS IFFT F32.\n\r");
}
