// -*-C++-*- time.h
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
// Title:   Wrapper for the C library's time.h
// Version: $Id: time.h,v 1.3 2003/04/13 19:35:59 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

extern "C"
{
#include _CXXRT_C_TIME_H
#if !defined(__GNUG__) || defined(_TIME_H)
#  define _CXXRT_DO_C_TIME_H
#endif
}

#if defined(_CXXRT_DO_C_TIME_H) && !defined(__TIME_H_CXX__) && !defined(_CXXRT_IN_STDLIB_H_) && !defined(_CXXRT_IN_SIGNAL_H_)
#define __TIME_H_CXX__ 1

_CXXRT_NAMESPACE_BEGIN

using ::asctime;
using ::clock;
using ::clock_t;
using ::ctime;
using ::difftime;
using ::gmtime;
using ::localtime;
using ::mktime;
using ::strftime;
using ::time;
using ::time_t;
using ::tm;

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#endif /* __TIME_H_CXX__ */
