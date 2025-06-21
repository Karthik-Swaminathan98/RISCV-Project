#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include "riscv_math.h"
#include "riscv_nnfunctions.h"

//#include "validate.h"

void layer0_riscv_depthwise_conv_s8(uint32_t *total_cycles, uint32_t *total_stack);
void layer2_riscv_depthwise_conv_s8(uint32_t *total_cycles, uint32_t *total_stack);
void layer4_riscv_depthwise_conv_s8(uint32_t *total_cycles, uint32_t *total_stack);
void layer3_riscv_conv_s8(uint32_t *total_cycles, uint32_t *total_stack);
void layer12_riscv_avgpool_s8(uint32_t *total_cycles, uint32_t *total_stack);
void layer13_riscv_fully_connected_s8(uint32_t *total_cycles, uint32_t *total_stack);


extern void user_init(void);

int main(void) {
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();
    delay_ms(20);

    uint32_t total_cycles = 0;
    uint32_t total_stack = 0;

	printf("\n\r");
    printf("-----Starting KWS-DSCNN_MEDIUM benchmark-----\n\r");
    printf("\n\r");
    printf("*****Layers 0: DEPTHWISE_CONV_2D*****\n\r");
	layer0_riscv_depthwise_conv_s8(&total_cycles, &total_stack);
	printf("\n\r");
    printf("*****Layers 2: DEPTHWISE_CONV_2D*****\n\r");
    layer2_riscv_depthwise_conv_s8(&total_cycles, &total_stack);
    printf("\n\r");
    printf("*****Layers 3, 5, 7, 9, 11: CONV_2D*****\n\r");
    layer3_riscv_conv_s8(&total_cycles, &total_stack);
    printf("\n\r");
    printf("*****Layers 4, 6, 8, 10: DEPTHWISE_CONV_2D*****\n\r");
    layer4_riscv_depthwise_conv_s8(&total_cycles, &total_stack);
    printf("\n\r");
    printf("*****Layers 12: AVERAGE_POOL_2D*****\n\r");
    layer12_riscv_avgpool_s8(&total_cycles, &total_stack);
    printf("\n\r");
    printf("*****Layers 13: FULLY_CONNECTED*****\n\r");
    layer13_riscv_fully_connected_s8(&total_cycles, &total_stack);
    // Print total results
    printf("\n\r");
    printf("-----KWS-DSCNN_MEDIUM benchmark complete-----\n\r");
    printf("Total Cycle Count: %lu\n\r", (unsigned long)total_cycles);
    printf("Total Stack Usage: %lu bytes\n\r", (unsigned long)total_stack);
	return 0;
}
