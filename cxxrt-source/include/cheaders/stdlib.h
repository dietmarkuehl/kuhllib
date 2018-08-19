// -*-C++-*- stdlib.h
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
// Title:   Wrapper for the C library's stdlib.h
// Version: $Id: stdlib.h,v 1.2 2002/06/06 01:56:54 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(__STDLIB_H_CXX__)
#define __STDLIB_H_CXX__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
extern "C"
{
#define _CXXRT_IN_STDLIB_H_
#include _CXXRT_C_STDLIB_H
#undef  _CXXRT_IN_STDLIB_H_
} // extern "C"

inline int abs(int _CS_i) throw() { return ::labs(_CS_i); }
inline long abs(long _CS_l) throw() { return ::labs(_CS_l); }
inline ldiv_t div(long _CS_l, long _CS_d) { return ::ldiv(_CS_l, _CS_d); }

_CXXRT_NAMESPACE_BEGIN

  using ::div_t;
  using ::ldiv_t;
//using ::size_t;

  using ::abort;
  using ::abs;
  using ::atexit;
  using ::bsearch;
  using ::calloc;
  using ::div;
  using ::exit;
  using ::atof;
  using ::atoi;
  using ::atol;
  using ::free;
  using ::getenv;
  using ::labs;
  using ::ldiv;
  using ::malloc;
  using ::mblen;
  using ::mbstowcs;
  using ::mbtowc;
  using ::qsort;
  using ::rand;
  using ::realloc;
  using ::srand;
  using ::strtod;
  using ::strtol;
  using ::strtoul;
  using ::wcstombs;
  using ::wctomb;

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#endif /* __STDLIB_H_CXX__ */
