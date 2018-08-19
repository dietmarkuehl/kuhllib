// -*-C++-*- wctype.h
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
// Title:   Wrapper for the C library's wctype.h
// Version: $Id: wctype.h,v 1.1.1.1 2002/06/05 01:00:52 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(__WCTYPE_H_CXX__)
#define __WCTYPE_H_CXX__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
extern "C"
{
#include _CXXRT_C_WCTYPE_H
} // extern "C"

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

  using ::wint_t;
  using ::wctype_t;
  using ::wctrans_t;

  using ::iswalnum;
  using ::iswalpha;
  using ::iswcntrl;
  using ::iswctype;
  using ::iswdigit;
  using ::iswgraph;
  using ::iswlower;
  using ::iswprint;
  using ::iswpunct;
  using ::iswspace;
  using ::iswupper;
  using ::iswxdigit;
  using ::towctrans;
  using ::towlower;
  using ::towupper;
  using ::wctrans;
  using ::wctype;

_CXXRT_NAMESPACE_END
#if !defined(__CWCTYPE__)
#  include <cwctype>
#endif

// --------------------------------------------------------------------------

#endif /* __WCTYPE_H_CXX__ */
