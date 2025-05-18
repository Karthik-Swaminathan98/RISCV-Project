/***************************************************************************
 *  Copyright (C) 2018-2020 Andes Technology Corporation                   *
 *  All rights reserved.                                                   *
 ***************************************************************************/

/** @file*/

#ifndef __RISCV_MATH_TYPES_H__
#define __RISCV_MATH_TYPES_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef int8_t q7_t;
typedef uint8_t u8_t;
typedef int16_t q15_t;
typedef uint16_t u16_t;
typedef int32_t q31_t;
typedef int64_t q63_t;
typedef float float32_t;
typedef double float64_t;

#ifdef  __cplusplus
}
#endif
#endif // NDS32_MATH_TYPES_H
