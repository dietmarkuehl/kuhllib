// -*-C++-*- setjmp.h
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
// Title:   Wrapper for the C library's setjmp.h
// Version: $Id: setjmp.h,v 1.1.1.1 2002/06/05 01:00:51 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(__SETJMP_H_CXX__)
#define __SETJMP_H_CXX__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
extern "C"
{
#include _CXXRT_C_SETJMP_H
} // extern "C"

_CXXRT_NAMESPACE_BEGIN

  using ::jmp_buf;
  using ::longjmp;

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#endif /* __SETJMP_H_CXX__ */
