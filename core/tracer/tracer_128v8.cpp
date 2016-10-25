/******************************************************************************/
/* Copyright (c) 2013-2016 VectorChief (at github, bitbucket, sourceforge)    */
/* Distributed under the MIT software license, see the accompanying           */
/* file COPYING or http://www.opensource.org/licenses/mit-license.php         */
/******************************************************************************/

#undef RT_SIMD_CODE /* fix redefinition warnings on legacy Visual C++ 6.0 */

#include "tracer.h"
#include "format.h"
#if RT_DEBUG >= 1
#include "system.h"
#endif /* RT_DEBUG */

#undef  RT_SIMD_REGS
#undef  RT_SIMD_ALIGN
#undef  RT_SIMD_WIDTH32
#undef  RT_SIMD_SET32
#undef  RT_SIMD_WIDTH64
#undef  RT_SIMD_SET64
#define RT_SIMD_CODE /* enable SIMD instruction definitions */

#if defined (RT_128) && (RT_128 & 8)
#undef  RT_128
#define RT_128 8
#define RT_RENDER_CODE /* enable contents of render0 routine */
#endif /* RT_128 */

#if defined (RT_X86) && (RT_SIMD_COMPAT_128 == 2)
#undef  RT_X86
#define RT_X86 2 /* enable BMI1+BMI2 for 128-bit AVX2 target on x86 */
#endif /* RT_X86 */

#if defined (RT_X32) && (RT_SIMD_COMPAT_128 == 2)
#undef  RT_X32
#define RT_X32 2 /* enable BMI1+BMI2 for 128-bit AVX2 target on x32 */
#endif /* RT_X32 */

#if defined (RT_X64) && (RT_SIMD_COMPAT_128 == 2)
#undef  RT_X64
#define RT_X64 2 /* enable BMI1+BMI2 for 128-bit AVX2 target on x64 */
#endif /* RT_X64 */

#if   RT_ELEMENT == 32

#if   defined (RT_ARM)
#error "AArch32 doesn't have SIMD variant 8, \
exclude this file from compilation"
#elif defined (RT_A32) || defined (RT_A64)
#error "AArch64 doesn't have SIMD variant 8, \
exclude this file from compilation"
#elif defined (RT_M32) || defined (RT_M64)
#error "mipsMSA doesn't have SIMD variant 8, \
exclude this file from compilation"
#elif defined (RT_P32) || defined (RT_P64)
#error "AltiVec doesn't have SIMD variant 8, \
exclude this file from compilation"
#elif defined (RT_X32) || defined (RT_X64)
#undef RT_RTARCH_X32_128_H
#undef RT_RTARCH_X64_128_H
#include "rtarch_x64_128.h"
#elif defined (RT_X86)
#undef RT_RTARCH_X86_128_H
#include "rtarch_x86_128.h"
#endif /* RT_ARM, RT_A32/A64, RT_M32/M64, RT_P32/P64, RT_X32/X64, RT_X86 */

#elif RT_ELEMENT == 64

#if   defined (RT_ARM)
#error "UniSIMD doesn't support 64-bit SIMD elements in 32-bit mode, \
adjust RT_ELEMENT build flag to be equal to 32"
#elif defined (RT_A32) || defined (RT_A64)
#error "AArch64 doesn't have SIMD variant 8, \
exclude this file from compilation"
#elif defined (RT_M32) || defined (RT_M64)
#error "mipsMSA doesn't have SIMD variant 8, \
exclude this file from compilation"
#elif defined (RT_P32) || defined (RT_P64)
#error "AltiVec doesn't have SIMD variant 8, \
exclude this file from compilation"
#elif defined (RT_X32) || defined (RT_X64)
#undef RT_RTARCH_X32_128_H
#undef RT_RTARCH_X64_128_H
#include "rtarch_x64_128.h"
#elif defined (RT_X86)
#error "UniSIMD doesn't support 64-bit SIMD elements in 32-bit mode, \
adjust RT_ELEMENT build flag to be equal to 32"
#endif /* RT_ARM, RT_A32/A64, RT_M32/M64, RT_P32/P64, RT_X32/X64, RT_X86 */

#endif /* RT_ELEMENT */

/*
 * Global pointer tables
 * for quick entry point resolution.
 */
extern
rt_pntr t_ptr[3];
extern
rt_pntr t_mat[3];
extern
rt_pntr t_clp[3];
extern
rt_pntr t_pow[6];

namespace simd_128v8
{
#include "tracer.cpp"
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/