#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include "riscv_math.h"

#define TEST_LENGTH_SAMPLES  320
#define SNR_THRESHOLD_F32    75.0f
#define BLOCK_SIZE           32
#define NUM_TAPS             8

/* ----------------------------------------------------------------------
** Q15 Input Signal & FIR Coefficients (Time-Reversed for CMSIS-DSP)
** ------------------------------------------------------------------- */
static q15_t signal_q15_in[TEST_LENGTH_SAMPLES] = {
    0, 16436, 19259, 12778, 12465, 22935, 31161, 24333, 7704, -2261, -0, 2261,
    -7704, -24333, -31161, -22936, -12465, -12778, -19259, -16436, -0, 16436,
    19259, 12778, 12465, 22935, 31161, 24333, 7704, -2261, -0, 2261, -7704,
    -24333, -31161, -22936, -12465, -12778, -19259, -16436, -0, 16436, 19259,
    12778, 12465, 22935, 31161, 24333, 7704, -2261, -0, 2261, -7704, -24333,
    -31161, -22935, -12465, -12778, -19259, -16436, -0, 16436, 19259, 12778,
    12465, 22936, 31161, 24333, 7704, -2261, -0, 2261, -7704, -24333, -31161,
    -22935, -12465, -12778, -19259, -16436, -0, 16436, 19259, 12778, 12465,
    22936, 31161, 24333, 7704, -2261, -0, 2261, -7704, -24333, -31161, -22936,
    -12465, -12778, -19259, -16436, -0, 16436, 19259, 12778, 12465, 22936,
    31161, 24333, 7704, -2261, 0, 2261, -7704, -24333, -31161, -22936, -12465,
    -12778, -19259, -16436, -0, 16436, 19259, 12778, 12465, 22936, 31161,
    24333, 7704, -2261, 0, 2261, -7704, -24333, -31161, -22936, -12465, -12778,
    -19259, -16436, -0, 16436, 19259, 12778, 12465, 22936, 31161, 24333, 7704,
    -2261, 0, 2261, -7704, -24333, -31161, -22936, -12465, -12778, -19259,
    -16436, -0, 16436, 19259, 12778, 12465, 22935, 31161, 24333, 7704, -2261,
    0, 2261, -7704, -24333, -31161, -22936, -12465, -12778, -19259, -16436,
    -0, 16436, 19259, 12778, 12465, 22935, 31161, 24333, 7704, -2261, -0, 2261,
    -7704, -24333, -31161, -22936, -12465, -12778, -19259, -16436, -0, 16436,
    19259, 12778, 12465, 22935, 31161, 24333, 7704, -2261, 0, 2261, -7704,
    -24333, -31161, -22936, -12465, -12778, -19259, -16436, 0, 16436, 19259,
    12778, 12465, 22935, 31161, 24333, 7704, -2261, -0, 2261, -7704, -24333,
    -31161, -22936, -12465, -12778, -19259, -16436, -0, 16436, 19259, 12778,
    12465, 22935, 31161, 24333, 7704, -2261, 0, 2261, -7704, -24333, -31161,
    -22936, -12465, -12778, -19259, -16436, 0, 16436, 19259, 12778, 12465,
    22935, 31161, 24333, 7704, -2261, -0, 2261, -7704, -24333, -31161, -22936,
    -12465, -12778, -19259, -16436, -0,
};

static q15_t firCoeffs[NUM_TAPS] = {
		2411, 4172, 5626, 6446, 6446, 5626, 4172, 2411 // Coefficients padded to make NUM_TAPS even
};
/* ----------------------------------------------------------------------
** Declare State Buffer and Output Buffer
** ------------------------------------------------------------------- */
static q15_t testOutputQ15[TEST_LENGTH_SAMPLES];
static q15_t firStateQ15[BLOCK_SIZE + NUM_TAPS - 1];

/* ----------------------------------------------------------------------
** Declare a buffer to store the converted Q15 output in floating-point
** ------------------------------------------------------------------- */
//static float32_t convertedOutput_f32[TEST_LENGTH_SAMPLES];

/* ----------------------------------------------------------------------
** FIR instance for Q15
** ------------------------------------------------------------------- */
riscv_fir_instance_q15 S;

/* ----------------------------------------------------------------------
** Function to Convert Q15 Back to Float32 (Recover and Scale)
** ------------------------------------------------------------------- */
//void convert_and_recover_q15_to_f32(const q15_t *inputQ15, float32_t *outputF32, uint32_t length) {
//    for (uint32_t i = 0; i < length; i++) {
//        outputF32[i] = ((float32_t)inputQ15[i] / 32768.0f);
//    }
//}
// Function to reset cycle count (RISC-V processor)
void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

// Function to read cycle count (32-bit)
unsigned int read_cycle_count() {
    return read_csr(NDS_MCYCLE);
}

extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    printf("****************** \n");
    printf("FIR Program for RISC-V DSP Library\n");
	delay_ms(20);
	uint32_t i;

    /* Call FIR init function to initialize the instance structure. */
	riscv_fir_init_q15(&S, NUM_TAPS, firCoeffs, firStateQ15, BLOCK_SIZE);

	// Reset cycle count
	reset_cycle_count();

    // Start cycle count
    uint32_t start_cycles = read_cycle_count();

    for (i = 0; i < TEST_LENGTH_SAMPLES / BLOCK_SIZE; i++) {
    		riscv_fir_q15(&S, signal_q15_in + (i * BLOCK_SIZE), testOutputQ15 + (i * BLOCK_SIZE), BLOCK_SIZE);
    	}

    // End cycle count
    unsigned int end_cycles = read_cycle_count();

    // Calculate the total cycle count
    unsigned int cycle_count = end_cycles - start_cycles;

    //convert_and_recover_q15_to_f32(testOutputQ15, convertedOutput_f32, TEST_LENGTH_SAMPLES);

    printf("\nFIR Output vs. Reference Output:\n");
    delay_ms(20);
    for (i = 0; i < TEST_LENGTH_SAMPLES; i++) {
        printf("Index %3lu: FIR Output (Q15) = %d\n", i, testOutputQ15[i]);
        delay_ms(20);
    }

    printf("Cycle Count for D25F: %u\n", cycle_count);
    delay_ms(20);

    return 0;
}
