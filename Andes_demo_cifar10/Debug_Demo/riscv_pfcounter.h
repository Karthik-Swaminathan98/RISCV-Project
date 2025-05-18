#ifndef __RISCV_PFCOUNTER_H__
#define __RISCV_PFCOUNTER_H__

#include <stdio.h>
#include <inttypes.h>
#include "riscv_math_types.h"

#ifdef PF_COUNTER
#include "nds32_intrinsic.h"

//Global instruction and cycle
#ifdef USE_EXTERN
#ifdef __riscv
    extern uint64_t inst;
    extern uint64_t cycle;

    extern uint64_t startInst;
    extern uint64_t startCycle;
    extern uint64_t stopInst;
    extern uint64_t stopCycle;
#if __riscv_xlen == 32
    extern uint32_t startInst_L;
    extern uint32_t startInst_H;
    extern uint32_t startCycle_L;
    extern uint32_t startCycle_H;
    extern uint32_t stopInst_L;
    extern uint32_t stopInst_H;
    extern uint32_t stopCycle_L;
    extern uint32_t stopCycle_H;
#endif
#else
    extern unsigned int inst;
    extern unsigned int cycle;
#endif
#else
#ifdef __riscv
    uint64_t inst;
    uint64_t cycle;

    uint64_t startInst;
    uint64_t startCycle;
    uint64_t stopInst;
    uint64_t stopCycle;
#if __riscv_xlen == 32
    uint32_t startInst_L;
    uint32_t startInst_H;
    uint32_t startCycle_L;
    uint32_t startCycle_H;
    uint32_t stopInst_L;
    uint32_t stopInst_H;
    uint32_t stopCycle_L;
    uint32_t stopCycle_H;
#endif
#else
    unsigned int inst;
    unsigned int cycle;
#endif
#endif

// Performance counter used for v5
#ifdef __riscv
 #if __riscv_xlen == 32
  #define startPFM \
    __nds__mtsr(0, NDS_MINSTRET); \
    __nds__mtsr(0, NDS_MINSTRETH); \
    __nds__mtsr(0, NDS_MCYCLE); \
    __nds__mtsr(0, NDS_MCYCLEH); \
    startInst_L = __nds__mfsr(NDS_MINSTRET); \
    startInst_H = __nds__mfsr(NDS_MINSTRETH); \
    startCycle_L = __nds__mfsr(NDS_MCYCLE); \
    startCycle_H = __nds__mfsr(NDS_MCYCLEH);
 #elif __riscv_xlen == 64
  #define startPFM \
    __nds__mtsr(0, NDS_MINSTRET); \
    __nds__mtsr(0, NDS_MCYCLE); \
    startInst = __nds__mfsr(NDS_MINSTRET); \
    startCycle = __nds__mfsr(NDS_MCYCLE);
 #endif

 #if __riscv_xlen == 32
  #define stopPFM \
    stopInst_L = __nds__mfsr(NDS_MINSTRET); \
    stopInst_H = __nds__mfsr(NDS_MINSTRETH); \
    stopCycle_L = __nds__mfsr(NDS_MCYCLE); \
    stopCycle_H = __nds__mfsr(NDS_MCYCLEH); \
    startInst = (uint64_t)startInst_H << 32 | startInst_L; \
    startCycle = (uint64_t)startCycle_H << 32 | startCycle_L; \
    stopInst = (uint64_t)stopInst_H << 32 | stopInst_L; \
    stopCycle = (uint64_t)stopCycle_H << 32 | stopCycle_L; \
    inst = stopInst - startInst; \
    cycle = stopCycle - startCycle; \
    if(stopCycle < startCycle) \
        printf("ERROR!!! Cycle counter has OVERFLOWED!\n"); \
    else if(stopInst < startInst) \
        printf("ERROR!!! Instr counter has OVERFLOWED!\n");
 #elif __riscv_xlen == 64
  #define stopPFM \
    stopInst = __nds__mfsr(NDS_MINSTRET); \
    stopCycle = __nds__mfsr(NDS_MCYCLE); \
    inst = stopInst - startInst; \
    cycle = stopCycle - startCycle; \
    if(stopCycle < startCycle) \
        printf("ERROR!!! Cycle counter has OVERFLOWED!\n"); \
    else if(stopInst < startInst) \
        printf("ERROR!!! Instr counter has OVERFLOWED!\n");
 #endif
// Performance counter used for v3xx.
#else
#define startPFM \
    __nds32__mtsr(0, NDS32_SR_PFMC0); \
    __nds32__mtsr(0, NDS32_SR_PFMC1); \
    __nds32__mtsr(0, NDS32_SR_PFMC2); \
    __nds32__mtsr(0x1000B, NDS32_SR_PFM_CTL)

#define stopPFM \
    __nds32__mtsr(0x10008, NDS32_SR_PFM_CTL); \
    if (__nds32__mfsr(NDS32_SR_PFM_CTL) & (7UL << 6)) \
        printf("\n!!!Error! PFM has OVERFLOWED! PFM_CTL=0x%08X\n\n", (int)__nds32__mfsr(NDS32_SR_PFM_CTL)); \
    __nds32__dsb(); \
    cycle = __nds32__mfsr(NDS32_SR_PFMC0); \
    inst = __nds32__mfsr(NDS32_SR_PFMC1)
#endif

// static inline uint32_t test_Fread(void *buf, int numByte, uint32_t bSize, FILE *fp)
// {


//     if (fread(buf, numByte, bSize, fp) != bSize)
//     {
//         //printf("Error! Can not read file!\n");
//         return 0;
//     }

// #if (defined(NDS32_EB) || defined(__NDS32_EB__))
//     uint32_t i;
//     char temp;
//     char *tt;
//     if (numByte == 4)
//     {
//         q31_t *buf32 = buf;
//         for (i = 0; i < bSize; i++)
//         {
//             tt = (char *)&buf32[i];
//             temp = tt[0]; tt[0] = tt[3]; tt[3] = temp;
//             temp = tt[1]; tt[1] = tt[2]; tt[2] = temp;
//         }
//     }
//     else if (numByte == 8)
//     {
//         q63_t *buf64 = buf;
//         for (i = 0; i < bSize; i++)
//         {
//             tt = (char *)&buf64[i];
//             temp = tt[0]; tt[0] = tt[7]; tt[7] = temp;
//             temp = tt[1]; tt[1] = tt[6]; tt[6] = temp;
//             temp = tt[2]; tt[2] = tt[5]; tt[5] = temp;
//             temp = tt[3]; tt[3] = tt[4]; tt[4] = temp;
//         }
//     }
//     else if (numByte == 2)
//     {
//         q15_t *buf16 = buf;
//         for (i = 0; i < bSize; i++)
//         {
//             tt = (char *)&buf16[i];
//             temp = tt[0]; tt[0] = tt[1]; tt[1] = temp;
//         }
//     }
// #endif
//     return bSize;
// }

// #define myLog(fun, inst, cycle)	(printf("%s inst = %u, cycle = %u\n", fun, inst, cycle))
#define myLog(fun, inst, cycle)	(printf("%s, %u, %u\n", fun, inst, cycle))


#ifndef DISABLE_PFCOUNTER_FUNCTION  //for preventing duplicate symbol error
__attribute__ ((noinline)) void readResult()
{
#ifdef PF_ANALYZE
    printf("Reading Performance Monitor Result\n");
#endif

#ifdef __riscv
    uint64_t MAX_32BIT = 0xffffffff;
    if(inst > MAX_32BIT)
    {
        printf("WARNING !!! The inst value is more than 32-bit.\n Print the value in hex.\n");
        printf("The inst count is 0x%08x%08x\n", (unsigned int)(inst>>32), (unsigned int)inst);
    }
    else    //show the performance data in 32-bit
    {
        printf("The inst count is %u\n", (unsigned int)inst);
    }

    if(cycle > MAX_32BIT)
    {
        printf("WARNING !!! The cycle value is more than 32-bit.\n Print the value in hex.\n");
        printf("The cycle count is 0x%08x%08x\n", (unsigned int)(cycle>>32), (unsigned int)cycle);
    }
    else    //show the performance data in 32-bit
    {
        printf("The cycle count is %u\n", (unsigned int)cycle);
    }
#else
    printf("The inst count is %u\n", inst);
    printf("The cycle count is %u\n", cycle);
#endif

#ifdef PF_ANALYZE
    unsigned int ONEMHZ = 1000000;
    float32_t  mhzbycycle = (float32_t) ONEMHZ / cycle * 100.0f;
    printf("The MHZ per cycle count is %5.2f%%\n", mhzbycycle);
#endif
}

typedef struct
{
    double  inst;
    double  cycle;
} acc_result;

__attribute__ ((noinline)) void ACC_Result(acc_result *result)
{
    result->inst += (double)inst;
    result->cycle += (double)cycle;
}


__attribute__ ((noinline)) void Read_ACC_Result(acc_result *result)
{
#ifdef PF_ANALYZE
    printf("Reading Performance Monitor Result\n");
#endif
    printf("The inst count is %.0lf\n", result->inst);
    printf("The cycle count is %.0lf\n", result->cycle);
#ifdef PF_ANALYZE
    unsigned int ONEMHZ = 1000000;
    float32_t  mhzbycycle = (float32_t) ONEMHZ / (result->cycle) * 100.0f;
    printf("The MHZ per cycle count is %5.2f%%\n", mhzbycycle);
#endif
}
#endif  //DISABLE_PFCOUNTER_FUNCTION

#endif  // PF_COUNTER
#endif // __NDS32_PFCOUNTER_H__
