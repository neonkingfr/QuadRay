/******************************************************************************/
/* Copyright (c) 2013-2015 VectorChief (at github, bitbucket, sourceforge)    */
/* Distributed under the MIT software license, see the accompanying           */
/* file COPYING or http://www.opensource.org/licenses/mit-license.php         */
/******************************************************************************/

#ifndef RT_RTIMAG_H
#define RT_RTIMAG_H

#include "rtbase.h"
#include "format.h"
#include "system.h"

/******************************************************************************/
/*********************************   LEGEND   *********************************/
/******************************************************************************/

/*
 * rtimag.h: Interface for the image utils library.
 *
 * More detailed description of this subsystem is given in rtimag.cpp.
 * Recommended naming scheme for C++ types and definitions is given in rtbase.h.
 */

/******************************************************************************/
/********************************   TEXTURE   *********************************/
/******************************************************************************/

/*
 * Load image from file to memory.
 */
rt_void load_image(rt_Heap *hp, rt_pstr name, rt_TEX *tx);

/*
 * Save image from memory to file.
 */
rt_void save_image(rt_Heap *hp, rt_pstr name, rt_TEX *tx);

/*
 * Convert image from file to C static array initializer format.
 */
rt_void convert_image(rt_Heap *hp, rt_pstr name);

#endif /* RT_RTIMAG_H */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
