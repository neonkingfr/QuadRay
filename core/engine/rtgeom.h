/******************************************************************************/
/* Copyright (c) 2013-2014 VectorChief (at github, bitbucket, sourceforge)    */
/* Distributed under the MIT software license, see the accompanying           */
/* file COPYING or http://www.opensource.org/licenses/mit-license.php         */
/******************************************************************************/

#ifndef RT_RTGEOM_H
#define RT_RTGEOM_H

#include "rtbase.h"
#include "format.h"
#include "object.h"

/******************************************************************************/
/*********************************   LEGEND   *********************************/
/******************************************************************************/

/*
 * rtgeom.h: Interface for the geometry utils library.
 *
 * More detailed description of this subsystem is given in rtgeom.cpp.
 * Recommended naming scheme for C++ types and definitions is given in rtbase.h.
 */

/******************************************************************************/
/*********************************   VECTORS   ********************************/
/******************************************************************************/

/* ---------------------------------   SET   -------------------------------- */

/*
 * Set vec3 to another vec3.
 */
#define RT_VEC3_SET(vr, va)                                                 \
do                                                                          \
{                                                                           \
    vr[0] = va[0];                                                          \
    vr[1] = va[1];                                                          \
    vr[2] = va[2];                                                          \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/*
 * Set vec3 to 1 scalar value.
 */
#define RT_VEC3_SET_VAL1(vr, sa)                                            \
do                                                                          \
{                                                                           \
    vr[0] = (sa);                                                           \
    vr[1] = (sa);                                                           \
    vr[2] = (sa);                                                           \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/* ---------------------------------   MIN   -------------------------------- */

/*
 * Compute the minimum of two vec3.
 */
#define RT_VEC3_MIN(vr, va, vb)                                             \
do                                                                          \
{                                                                           \
    vr[0] = RT_MIN(va[0], vb[0]);                                           \
    vr[1] = RT_MIN(va[1], vb[1]);                                           \
    vr[2] = RT_MIN(va[2], vb[2]);                                           \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/* ---------------------------------   MAX   -------------------------------- */

/*
 * Compute the maximum of two vec3.
 */
#define RT_VEC3_MAX(vr, va, vb)                                             \
do                                                                          \
{                                                                           \
    vr[0] = RT_MAX(va[0], vb[0]);                                           \
    vr[1] = RT_MAX(va[1], vb[1]);                                           \
    vr[2] = RT_MAX(va[2], vb[2]);                                           \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/* ---------------------------------   ADD   -------------------------------- */

/*
 * Compute the sum of two vec3.
 */
#define RT_VEC3_ADD(vr, va, vb)                                             \
do                                                                          \
{                                                                           \
    vr[0] = va[0] + vb[0];                                                  \
    vr[1] = va[1] + vb[1];                                                  \
    vr[2] = va[2] + vb[2];                                                  \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/* ---------------------------------   SUB   -------------------------------- */

/*
 * Compute the difference of two vec3.
 */
#define RT_VEC3_SUB(vr, va, vb)                                             \
do                                                                          \
{                                                                           \
    vr[0] = va[0] - vb[0];                                                  \
    vr[1] = va[1] - vb[1];                                                  \
    vr[2] = va[2] - vb[2];                                                  \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/* ---------------------------------   MUL   -------------------------------- */

/*
 * Compute the product of two vec3 (cross).
 */
#define RT_VEC3_MUL(vr, va, vb)                                             \
do                                                                          \
{                                                                           \
    vr[0] = va[1] * vb[2] - vb[1] * va[2];                                  \
    vr[1] = va[2] * vb[0] - vb[2] * va[0];                                  \
    vr[2] = va[0] * vb[1] - vb[0] * va[1];                                  \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/*
 * Compute the product of vec3 and 1 scalar value.
 */
#define RT_VEC3_MUL_VAL1(vr, va, sa)                                        \
do                                                                          \
{                                                                           \
    vr[0] = va[0] * (sa);                                                   \
    vr[1] = va[1] * (sa);                                                   \
    vr[2] = va[2] * (sa);                                                   \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/*
 * Compute and add the product of vec3 and 1 scalar value.
 */
#define RT_VEC3_MAD_VAL1(vr, va, sa)                                        \
do                                                                          \
{                                                                           \
    vr[0] += va[0] * (sa);                                                  \
    vr[1] += va[1] * (sa);                                                  \
    vr[2] += va[2] * (sa);                                                  \
}                                                                           \
while (0) /* "do {...} while (0)" to enforce semicolon ";" at the end */

/* ---------------------------------   DOT   -------------------------------- */

/*
 * Compute the product of two vec3 (dot).
 */
#define RT_VEC3_DOT(va, vb)                                                 \
(                                                                           \
    va[0] * vb[0] +                                                         \
    va[1] * vb[1] +                                                         \
    va[2] * vb[2]                                                           \
)

/* ---------------------------------   LEN   -------------------------------- */

/*
 * Compute the length of vec3.
 */
#define RT_VEC3_LEN(va)                                                     \
(                                                                           \
    RT_SQRT(RT_VEC3_DOT(va, va))                                            \
)

/******************************************************************************/
/********************************   MATRICES   ********************************/
/******************************************************************************/

/*
 * Identity matrix.
 */
extern rt_mat4 iden4;

/*
 * Multiply matrix by vector.
 */
rt_void matrix_mul_vector(rt_vec4 vp, rt_mat4  m1, rt_vec4 v1);

/*
 * Multiply matrix by matrix.
 */
rt_void matrix_mul_matrix(rt_mat4 mp, rt_mat4 m1, rt_mat4 m2);

/*
 * Compute matrix from transform.
 */
rt_void matrix_from_transform(rt_mat4 mp, rt_TRANSFORM3D *t1);

/*
 * Compute upper-left 3x3 inverse of a 4x4 matrix.
 */
rt_void matrix_inverse(rt_mat4 mp, rt_mat4 m1);

/******************************************************************************/
/********************************   GEOMETRY   ********************************/
/******************************************************************************/

/*
 * Determine if "nd1" bbox casts shadow on "nd2" bbox as seen from "obj".
 *
 * Return values:
 *  0 - no
 *  1 - yes
 */
rt_cell bbox_shad(rt_BOUND *obj, rt_BOUND *nd1, rt_BOUND *nd2);

/*
 * Determine the order of "nd1" and "nd2" bboxes as seen from "obj".
 *
 * Return values:
 *  1 - neutral
 *  2 - unsortable
 *  3 - don't swap
 *  4 - do swap, not part of the stored order value in the engine
 */
rt_cell bbox_sort(rt_BOUND *obj, rt_BOUND *nd1, rt_BOUND *nd2);

/*
 * Determine which side of clipped "srf" is seen from "obj".
 *
 * Return values:
 *  0 - none, if both surfaces are the same plane
 *  1 - inner
 *  2 - outer
 *  3 - both, also if on the surface with margin
 */
rt_cell bbox_side(rt_BOUND *obj, rt_SHAPE *srf);

#endif /* RT_RTGEOM_H */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
