/******************************************************************************/
/* Copyright (c) 2013 VectorChief (at github, bitbucket, sourceforge)         */
/* Distributed under the MIT software license, see the accompanying           */
/* file COPYING or http://www.opensource.org/licenses/mit-license.php         */
/******************************************************************************/

#include "rtbase.h"
#include "engine.h"

#define RT_X_RES    800
#define RT_Y_RES    480

rt_astr     title   = "QuadRay engine demo, (C) 2013 VectorChief";

rt_cell     x_res   = RT_X_RES;
rt_cell     y_res   = RT_Y_RES;
rt_cell     x_row   = RT_X_RES;
rt_word    *frame   = RT_NULL;

rt_Scene   *scene   = RT_NULL;

#include <stdio.h>

#define RT_LOGI     printf
#define RT_LOGE     printf

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
