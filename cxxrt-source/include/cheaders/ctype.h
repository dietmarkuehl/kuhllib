// -*-C++-*- ctype.h
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
// Title:   Wrapper around the C library's ctype.h
// Version: $Id: ctype.h,v 1.1.1.1 2002/06/05 01:00:45 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(__CTYPE_H_CXX__)
#define __CTYPE_H_CXX__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
extern "C" 
{
#include _CXXRT_C_CTYPE_H
} // extern "C"

// --------------------------------------------------------------------------

#if defined(isalnum)
#  undef isalnum
#endif
#if defined(isalpha)
#  undef isalpha
#endif
#if defined(iscntrl)
#  undef iscntrl
#endif
#if defined(isdigit)
#  undef isdigit
#endif
#if defined(isgraph)
#  undef isgraph
#endif
#if defined(islower)
#  undef islower
#endif
#if defined(isprint)
#  undef isprint
#endif
#if defined(ispunct)
#  undef ispunct
#endif
#if defined(isspace)
#  undef isspace
#endif
#if defined(isupper)
#  undef isupper
#endif
#if defined(isxdigit)
#  undef isxdigit
#endif
#if defined(tolower)
#  undef tolower
#endif
#if defined(toupper)
#  undef toupper
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

using ::isalnum;
using ::isalpha;
using ::iscntrl;
using ::isdigit;
using ::isgraph;
using ::islower;
using ::isprint;
using ::ispunct;
using ::isspace;
using ::isupper;
using ::isxdigit;
using ::tolower;
using ::toupper;

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#endif /* __CTYPE_H_CXX__ */
