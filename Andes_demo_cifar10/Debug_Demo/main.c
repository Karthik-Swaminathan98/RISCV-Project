#include "app_config.h"
#include "core.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

typedef int8_t q7_t;
typedef uint8_t u8_t;
typedef int16_t q15_t;
typedef uint16_t u16_t;
typedef int32_t q31_t;
typedef int64_t q63_t;
typedef float float32_t;
typedef double float64_t;

#define NDS_ISA_CLIPS(X, Y) sat_q31_to_any(X, (Y + 1))
#define NDS_DSP_CLIPS __nds__sclip32

static inline q31_t sat_q31_to_any(q31_t src, const uint32_t bit_size)
{
    q31_t ret;
    const q31_t max_val = (q31_t)0x7FFFFFFF >> (32 - bit_size);
    const q31_t min_val = (q31_t)0x80000000 >> (32 - bit_size);

    ret = (src > max_val) ? max_val : (src < min_val) ? min_val : src;
#ifdef SIM_SAT_OV_FLAG
    if (ret != src)
    {
        nds_sat_ov_flag = 1;
    }
#endif
    return ret;
}

#include "riscv_nn_examples_cifar10_inputs.h"
#include "riscv_nn_examples_cifar10_parameter.h"
#include "riscv_nn_examples_cifar10_weights.h"

#include "riscv_nn_softmax.h"
#include "riscv_nn_activation.h"
#include "riscv_nn_pooling.h"
#include "riscv_nn_fully_connected.h"
#include "riscv_nn_convolution.h"
#include "riscv_util.h"
#include "image_data.h"

// Model weights and biases
static q7_t conv1_wt[CONV1_IM_CH * CONV1_KER_DIM * CONV1_KER_DIM * CONV1_OUT_CH] = CONV1_WT;
static q7_t conv1_bias[CONV1_OUT_CH] = CONV1_BIAS;
static q7_t conv2_wt[CONV2_IM_CH * CONV2_KER_DIM * CONV2_KER_DIM * CONV2_OUT_CH] = CONV2_WT;
static q7_t conv2_bias[CONV2_OUT_CH] = CONV2_BIAS;
static q7_t conv3_wt[CONV3_IM_CH * CONV3_KER_DIM * CONV3_KER_DIM * CONV3_OUT_CH] = CONV3_WT;
static q7_t conv3_bias[CONV3_OUT_CH] = CONV3_BIAS;
static q7_t ip1_wt[IP1_DIM * IP1_OUT] = IP1_WT;
static q7_t ip1_bias[IP1_OUT] = IP1_BIAS;

// Input and output buffers
//uint8_t image_data[CONV1_IM_CH * CONV1_IM_DIM * CONV1_IM_DIM] = IMG_DATA;
#define _DECLARE_IMAGE(img) static uint8_t image_data[CONV1_IM_CH * CONV1_IM_DIM * CONV1_IM_DIM] = IMG_DATA_##img; \
                           const char *image_name = #img;
#define DECLARE_IMAGE(img) _DECLARE_IMAGE(img)
// Change the DECLARE_IMAGE(imgno) to select different images
// img could be airplane, automobile, bird, cat, deer, dog, horse, ship, truck
// no could be 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
// eg. Use deer1:
// DECLARE_IMAGE(deer1);
// eg. Use dog10:
// DECLARE_IMAGE(dog10);
#ifndef TEST_IMAGE
#define TEST_IMAGE horse1
#endif
DECLARE_IMAGE(TEST_IMAGE);

q7_t output_data[IP1_OUT];

// Intermediate buffers
q7_t col_buffer[2 * 5 * 5 * 32 * 2];
q15_t rgb_wt_buffer[CONV1_OUT_CH * (3 * CONV1_KER_DIM * CONV1_KER_DIM + 1)];
q7_t scratch_buffer[32 * 32 * 10 * 4];
const char* cifar10_label[] = {"Plane", "Car", "Bird", "Cat", "Deer", "Dog", "Frog", "Horse", "Ship", "Truck"};

void reset_cycle_count() {
    write_csr(NDS_MCYCLE, 0);
}

unsigned int read_cycle_count() {
    return read_csr(NDS_MCYCLE);
}
extern void user_init(void);

int main(void)
{
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    enable_fs_and_vs();
    printf("Start execution ANDES CNN to classify image %s\n\r", image_name);
    delay_ms(20);

    q7_t *img_buffer1 = scratch_buffer;
    q7_t *img_buffer2 = img_buffer1 + 32 * 32 * 32;

    // Input pre-processing: subtract mean, shift, saturate
    int mean_data[3] = INPUT_MEAN_SHIFT;
    unsigned int scale_data[3] = INPUT_RIGHT_SHIFT;
    reset_cycle_count();
    unsigned int start_cycles = read_cycle_count();
    for (int i = 0; i < 32 * 32 * 3; i += 3)
    {
#ifndef ENA_DSP_ISA
        img_buffer2[i] =   (q7_t)NDS_ISA_CLIPS(((((int)image_data[i]   - mean_data[0]) << 7) + (1 << (scale_data[0] - 1))) >> scale_data[0], 8);
        img_buffer2[i + 1] = (q7_t)NDS_ISA_CLIPS(((((int)image_data[i+1] - mean_data[1]) << 7) + (1 << (scale_data[1] - 1))) >> scale_data[1], 8);
        img_buffer2[i + 2] = (q7_t)NDS_ISA_CLIPS(((((int)image_data[i+2] - mean_data[2]) << 7) + (1 << (scale_data[2] - 1))) >> scale_data[2], 8);
#else
        img_buffer2[i] =   (q7_t)NDS_DSP_CLIPS(((((int)image_data[i]   - mean_data[0]) << 7) + (1 << (scale_data[0] - 1))) >> scale_data[0], 8);
        img_buffer2[i + 1] = (q7_t)NDS_DSP_CLIPS(((((int)image_data[i+1] - mean_data[1]) << 7) + (1 << (scale_data[1] - 1))) >> scale_data[1], 8);
        img_buffer2[i + 2] = (q7_t)NDS_DSP_CLIPS(((((int)image_data[i+2] - mean_data[2]) << 7) + (1 << (scale_data[2] - 1))) >> scale_data[2], 8);
#endif
    }
    unsigned int end_cycles = read_cycle_count();
    printf("Preprocess cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    // Layer 1: Conv -> ReLU -> Pool
    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_conv_HWC_s8_s8_s8_RGB_sft_bias_fast(img_buffer2, CONV1_IM_DIM, conv1_wt, CONV1_OUT_CH,
        CONV1_KER_DIM, CONV1_PADDING, CONV1_STRIDE, conv1_bias, CONV1_BIAS_LSHIFT,
        CONV1_OUT_RSHIFT, img_buffer1, CONV1_OUT_DIM, (q15_t *)col_buffer, rgb_wt_buffer);
    end_cycles = read_cycle_count();
    printf("Conv1 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_relu_s8(img_buffer1, CONV1_OUT_DIM * CONV1_OUT_DIM * CONV1_OUT_CH);
    end_cycles = read_cycle_count();
    printf("ReLU1 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_maxpool_HWC_s8(img_buffer1, CONV1_OUT_DIM, CONV1_OUT_CH,
        POOL1_KER_DIM, POOL1_PADDING, POOL1_STRIDE, POOL1_OUT_DIM, NULL, img_buffer2);
    end_cycles = read_cycle_count();
    printf("Pool1 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    // Layer 2: Conv -> ReLU -> Pool
    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_conv_HWC_s8_s8_s8_sft_bias_fast(img_buffer2, CONV2_IM_DIM, CONV2_IM_CH, conv2_wt,
        CONV2_OUT_CH, CONV2_KER_DIM, CONV2_PADDING, CONV2_STRIDE, conv2_bias,
        CONV2_BIAS_LSHIFT, CONV2_OUT_RSHIFT, img_buffer1, CONV2_OUT_DIM, (q15_t *)col_buffer, NULL);
    end_cycles = read_cycle_count();
    printf("Conv2 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_relu_s8(img_buffer1, CONV2_OUT_DIM * CONV2_OUT_DIM * CONV2_OUT_CH);
    end_cycles = read_cycle_count();
    printf("ReLU2 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_maxpool_HWC_s8(img_buffer1, CONV2_OUT_DIM, CONV2_OUT_CH,
        POOL2_KER_DIM, POOL2_PADDING, POOL2_STRIDE, POOL2_OUT_DIM, col_buffer, img_buffer2);
    end_cycles = read_cycle_count();
    printf("Pool2 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    // Layer 3: Conv -> ReLU -> Pool
    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_conv_HWC_s8_s8_s8_sft_bias_fast(img_buffer2, CONV3_IM_DIM, CONV3_IM_CH, conv3_wt,
        CONV3_OUT_CH, CONV3_KER_DIM, CONV3_PADDING, CONV3_STRIDE, conv3_bias,
        CONV3_BIAS_LSHIFT, CONV3_OUT_RSHIFT, img_buffer1, CONV3_OUT_DIM, (q15_t *)col_buffer, NULL);
    end_cycles = read_cycle_count();
    printf("Conv3 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_relu_s8(img_buffer1, CONV3_OUT_DIM * CONV3_OUT_DIM * CONV3_OUT_CH);
    end_cycles = read_cycle_count();
    printf("ReLU3 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_maxpool_HWC_s8(img_buffer1, CONV3_OUT_DIM, CONV3_OUT_CH,
        POOL3_KER_DIM, POOL3_PADDING, POOL3_STRIDE, POOL3_OUT_DIM, col_buffer, img_buffer2);
    end_cycles = read_cycle_count();
    printf("Pool3 cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    // Fully Connected + Softmax
    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_fc_s8_s8_s8_sft_bias_fast(img_buffer2, ip1_wt, IP1_DIM, IP1_OUT,
        IP1_BIAS_LSHIFT, IP1_OUT_RSHIFT, ip1_bias, output_data, (q15_t *)img_buffer1);
    end_cycles = read_cycle_count();
    printf("FC cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    reset_cycle_count();
    start_cycles = read_cycle_count();
    riscv_nn_softmax_s8_fast(output_data, IP1_OUT, output_data);
    end_cycles = read_cycle_count();
    printf("Softmax cycles: %u\n\r", end_cycles - start_cycles);
    delay_ms(20);

    // Output results
    float confidence = 0.0;
    for (int i = 0; i < 10; i++) {
        confidence = (output_data[i] / 127.0f) * 100;
        printf("Label %d: %d, %s, %.2f%%\n\r", i, output_data[i], cifar10_label[i], confidence);
        delay_ms(20);
    }

    return 0;
}
