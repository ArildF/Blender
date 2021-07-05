/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** \file
 * \ingroup freestyle
 */

#pragma once

#include "BPy_ChainingIterator.h"

#include "../../stroke/ChainingIterators.h"

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////

extern PyTypeObject ChainSilhouetteIterator_Type;

#define BPy_ChainSilhouetteIterator_Check(v) \
  (PyObject_IsInstance((PyObject *)v, (PyObject *)&ChainSilhouetteIterator_Type))

/*---------------------------Python BPy_ChainSilhouetteIterator structure definition----------*/
typedef struct {
  BPy_ChainingIterator py_c_it;
  Freestyle::ChainSilhouetteIterator *cs_it;
} BPy_ChainSilhouetteIterator;

///////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif
