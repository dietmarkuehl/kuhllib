// -*-C++-*- num_lim.cc
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
// Title:   Definition of the static member of numeric_limits and associates.
// Version: $Id: num_lim.cc,v 1.1.1.1 2002/06/05 01:02:15 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

/*
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is not portable code.  Parts of numeric_limits<> are
 * inherently machine-dependent, and this file is written for the MIPS
 * architecture and the SGI MIPSpro C++ compiler.  Parts of it (in
 * particular, some of the characteristics of floating-point types)
 * are almost certainly incorrect for any other platform.
 */

// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_IOS_CC__)
#define __SRCCXX_IOS_CC__ 1

// -------------------------------------------------------------------------- 

#include <limits>

#if 1 || !defined(__STL_STATIC_CONST_INIT_BUG)

_CXXRT_NAMESPACE_BEGIN

template <class _Number> bool const _Numeric_limits_base<_Number>::is_specialized;
template <class _Number> int const _Numeric_limits_base<_Number>::digits;
template <class _Number> int const _Numeric_limits_base<_Number>::digits10;
template <class _Number> bool const _Numeric_limits_base<_Number>::is_signed;
template <class _Number> bool const _Numeric_limits_base<_Number>::is_integer;
template <class _Number> bool const _Numeric_limits_base<_Number>::is_exact;
template <class _Number> int const _Numeric_limits_base<_Number>::radix;
template <class _Number> int const _Numeric_limits_base<_Number>::min_exponent;
template <class _Number> int const _Numeric_limits_base<_Number>::max_exponent;
template <class _Number> int const _Numeric_limits_base<_Number>::min_exponent10;
template <class _Number> int const _Numeric_limits_base<_Number>::max_exponent10;
template <class _Number> bool const _Numeric_limits_base<_Number>::has_infinity;
template <class _Number> bool const _Numeric_limits_base<_Number>::has_quiet_NaN;
template <class _Number> bool const _Numeric_limits_base<_Number>::has_signaling_NaN;
template <class _Number> float_denorm_style const _Numeric_limits_base<_Number>::has_denorm;
template <class _Number> bool const _Numeric_limits_base<_Number>::has_denorm_loss;
template <class _Number> bool const _Numeric_limits_base<_Number>::is_iec559;
template <class _Number> bool const _Numeric_limits_base<_Number>::is_bounded;
template <class _Number> bool const _Numeric_limits_base<_Number>::is_modulo;
template <class _Number> bool const _Numeric_limits_base<_Number>::traps;
template <class _Number> bool const _Numeric_limits_base<_Number>::tinyness_before;
template <class _Number> float_round_style const _Numeric_limits_base<_Number>::round_style;


template <class _Int, _Int __imin, _Int __imax, int __idigits>
bool const _Integer_limits<_Int, __imin, __imax, __idigits>::is_specialized;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
int const _Integer_limits<_Int, __imin, __imax, __idigits>::digits;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
int const _Integer_limits<_Int, __imin, __imax, __idigits>::digits10;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
bool const _Integer_limits<_Int, __imin, __imax, __idigits>::is_signed;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
bool const _Integer_limits<_Int, __imin, __imax, __idigits>::is_integer;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
bool const _Integer_limits<_Int, __imin, __imax, __idigits>::is_exact;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
int const _Integer_limits<_Int, __imin, __imax, __idigits>::radix;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
bool const _Integer_limits<_Int, __imin, __imax, __idigits>::is_bounded;
template <class _Int, _Int __imin, _Int __imax, int __idigits>
bool const _Integer_limits<_Int, __imin, __imax, __idigits>::is_modulo;


template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::is_specialized;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
int const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::digits;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
int const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::digits10;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::is_signed;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
int const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::radix;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
int const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::min_exponent;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
int const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::max_exponent;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
int const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::min_exponent10;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
int const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::max_exponent10;  

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::has_infinity;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::has_quiet_NaN;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::has_signaling_NaN;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
float_denorm_style const _Floating_limits<__Num, __Dig, __Dig10,
  __MnX, __MxX, __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::has_denorm;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::has_denorm_loss;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::is_iec559;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::is_bounded;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::traps;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
bool const _Floating_limits<__Num, __Dig, __Dig10, __MnX, __MxX,
  __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::tinyness_before;

template <class __Num, int __Dig, int __Dig10, int __MnX,
  int __MxX, int __MnX10, int __MxX10, unsigned int __Inf,
  unsigned int __QNaN, unsigned int __SNaN, bool __IsIEEE,
  float_round_style __Sty>
float_round_style const _Floating_limits<__Num, __Dig, __Dig10,
  __MnX, __MxX, __MnX10, __MxX10, __Inf, __QNaN, __SNaN,__IsIEEE, __Sty>::round_style;

// Class numeric_limits

_CXXRT_NAMESPACE_END

#endif /* !defined(__STL_STATIC_CONST_INIT_BUG) */

// -----------------------------------------------------------------------------

#endif /* __SRCCXX_IOS_CC__ */
