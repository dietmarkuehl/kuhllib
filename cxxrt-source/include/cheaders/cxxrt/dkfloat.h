// -*-C++-*- dkfloat.h
// -------------------------------------------------------------------------- 
// Copyright (c) 2002 Dietmar Kuehl

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -------------------------------------------------------------------------- 
// Author:  Dietmar Kuehl <http://www.dietmar-kuehl.de/>
// Title:   An dummy for non-existing <cfloat> and <float.h>
// Version: $Id: dkfloat.h,v 1.1.1.1 2002/06/05 01:01:52 dietmar_kuehl Exp $

// Note: This is really a very dump dummy!
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_DKFLOAT_H__)
#define _CXXRT_DKFLOAT_H__ 1

// --------------------------------------------------------------------------

#include "cxxrtcf.h"

//------------------------------------------------------------------------------

#define FLT_DIG          5
#define FLT_EPSILON      1e-3
#define FLT_MANT_DIG     2
#define FLT_MAX          1e6
#define FLT_MAX_10_EXP   10
#define FLT_MAX_EXP      20
#define FLT_MIN          -1e6
#define FLT_MIN_10_EXP   -10
#define FLT_MIN_EXP      -20
#define FLT_RADIX        2
#define FLT_ROUNDS       1

#define DBL_DIG          FLT_DIG
#define DBL_EPSILON      FLT_EPSILON
#define DBL_MANT_DIG     FLT_MANT_DIG
#define DBL_MAX          FLT_MAX
#define DBL_MAX_10_EXP   FLT_MAX_10_EXP
#define DBL_MAX_EXP      FLT_MAX_EXP
#define DBL_MIN          FLT_MIN
#define DBL_MIN_10_EXP   FLT_MIN_10_EXP
#define DBL_MIN_EXP      FLT_MIN_EXP

#define LDBL_DIG         FLT_DIG
#define LDBL_MANT_DIG    FLT_MANT_DIG
#define LDBL_MAX_10_EXP  FLT_MAX_EXP
#define LDBL_MAX_EXP     FLT_MAX_EXP
#define LDBL_MIN_10_EXP  FLT_MIN_10_EXP
#define LDBL_MIN_EXP     FLT_MIN_EXP

//------------------------------------------------------------------------------

#endif /* _CXXRT_DKFLOAT_H__ */
