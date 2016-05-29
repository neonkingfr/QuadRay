/******************************************************************************/
/* Copyright (c) 2013-2016 VectorChief (at github, bitbucket, sourceforge)    */
/* Distributed under the MIT software license, see the accompanying           */
/* file COPYING or http://www.opensource.org/licenses/mit-license.php         */
/******************************************************************************/

#undef RT_SIMD_CODE /* fix redefinition warnings on legacy Visual C++ 6.5 */

#include "tracer.h"
#include "format.h"
#if RT_DEBUG >= 1
#include "system.h"
#endif /* RT_DEBUG */

#undef  RT_SIMD_WIDTH
#undef  RT_SIMD_ALIGN
#undef  RT_SIMD_SET
#define RT_SIMD_CODE /* enable SIMD instructions definitions */

#if   defined (RT_128) && (RT_128 & 4)
#undef  RT_128
#define RT_128 4
#define RT_RENDER_CODE /* enable contents of render0 routine */
#endif /* RT_128 */

#if   defined (RT_X86)
#undef RT_RTARCH_X86_128_H
#include "rtarch_x86_128.h"
#elif defined (RT_X32)
#undef RT_RTARCH_X32_128_H
#include "rtarch_x32_128.h"
#elif defined (RT_ARM)
#undef RT_RTARCH_ARM_128_H
#include "rtarch_arm_128.h"
#elif defined (RT_A32)
#error "AArch64 doesn't have SIMD variants, \
exclude this file from compilation"
#elif defined (RT_M32)
#error "mipsMSA doesn't have SIMD variants, \
exclude this file from compilation"
#elif defined (RT_P32)
#error "AltiVec doesn't have SIMD variants, \
exclude this file from compilation"
#endif /* RT_X86, RT_X32, RT_ARM, RT_A32, RT_M32, RT_P32 */

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

namespace simd_128v4
{
#include "tracer.cpp"
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/