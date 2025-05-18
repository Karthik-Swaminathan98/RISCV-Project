#ifndef __RISCV_UTIL_H__
#define __RISCV_UTIL_H__

#include "nds32_intrinsic.h"

#if 0
#define PRINTF_DBG printf
#else
#define PRINTF_DBG(...)
#endif

void enable_fs_and_vs()
{
#ifdef ENA_FS_VS
// #if 1
    {
        //Set MSTATUS.FS = 2 (clean) to avoid post FPU instructions raise exception in other simulator (Imperas).
        uint32_t csr_mstatus;
        const uint32_t mask_fs = (3L << 13);
        const uint32_t fs_clean = (2L << 13);

        PRINTF_DBG("\nSet FPU...\n");
        PRINTF_DBG("Setting MSTATUS.FS = 2 (=clean) ...\n");

        csr_mstatus = __nds__read_and_clear_csr(mask_fs, NDS_MSTATUS);
        //PRINTF_DBG("Original MSTATUS CSR = 0x%08X\n", csr_mstatus);

        //csr_mstatus = __nds__read_csr(NDS_MSTATUS);
        PRINTF_DBG("After read and clear, MSTATUS CSR = 0x%08X\n", csr_mstatus);

        csr_mstatus |= fs_clean;
        PRINTF_DBG("New MSTATUS value = 0x%08X\n", csr_mstatus);
        __nds__write_csr(csr_mstatus, NDS_MSTATUS);

        //Read again to verify it.
        csr_mstatus = __nds__read_csr(NDS_MSTATUS);
        PRINTF_DBG("MSTATUS CSR = 0x%08X\n", csr_mstatus);
        PRINTF_DBG("FPU is set to clean.\n\n");
    }

    {
        //Enable VPU
        uint32_t csr_mstatus;
#if (NDS_VEC_RVV_VERSION == 80)     //for v-ext 0.8
        const uint32_t mask_vs = (3L << 23);
        const uint32_t vs_clean = (2L << 23);
#else   //for rvv-0.9 or higher
        const uint32_t mask_vs = (3L << 9);
        const uint32_t vs_clean = (2L << 9);
#endif

        PRINTF_DBG("Set VPU...\n");
        PRINTF_DBG("Setting MSTATUS.VS = 2 (=clean) ...\n");
        csr_mstatus = __nds__read_and_clear_csr(mask_vs, NDS_MSTATUS);
        PRINTF_DBG("After read and clear, MSTATUS CSR = 0x%08X\n", csr_mstatus);

        //csr_mstatus = __nds__read_csr(NDS_MSTATUS);
        //PRINTF_DBG("Original MSTATUS CSR = 0x%08X\n", csr_mstatus);

        csr_mstatus |=  vs_clean;
        PRINTF_DBG("New MSTATUS value = 0x%08X\n", csr_mstatus);
        __nds__write_csr(csr_mstatus, NDS_MSTATUS);

        //Read again to verify it.
        csr_mstatus = __nds__read_csr(NDS_MSTATUS);
        PRINTF_DBG("MSTATUS CSR = 0x%08X\n", csr_mstatus);
        PRINTF_DBG("VPU is set to clean.\n\n");
    }
#endif
}

#endif  //__NDS_UTIL_H__
