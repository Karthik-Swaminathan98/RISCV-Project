#ifndef __RISCV_DEBUG_H
#define __RISCV_DEBUG_H

#include "assert.h"

static inline void dump_checksum(FILE *file, char fun_name[], char buffer_name[], void* begin_addr, void* end_addr)
{
    /*  Note1.  No matter what the data type of check_buffer is, we take "1 byte" data to calculate the
                checksum. This mechanism no only makes sure this checksum function could be compatible with all
                data type of check_buffer but also reduce the probility of checksum overflow.
        Note2.  checksum formula: x^3+x+1 (x: 1 byte data) */


    q63_t checksum = 0;
    q7_t *ptr_byte = (q7_t*)begin_addr;
    while(ptr_byte != (q7_t*)end_addr)
    {
        checksum += ((q31_t)*ptr_byte)*(*ptr_byte)*(*ptr_byte) + (*ptr_byte) + 1;
        ptr_byte++;
    }

    fprintf(file, "after %s the checksum of %s = %08x%08x\n", fun_name, buffer_name, (q31_t)(checksum>>32), (q31_t)checksum);
}

#endif  //_NDS_DEBUG_H
