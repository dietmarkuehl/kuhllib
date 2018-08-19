// -*-C++-*- limits.h
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
// Title:   Forwarding header for system limits
// Version: $Id: limits.h,v 1.1.1.1 2002/06/05 01:00:53 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(__LIMITS_H_CXX__)
#define __LIMITS_H_CXX__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

#if defined(__EDG__)
#include <syslimits.h>
#else
extern "C"
{
#include _CXXRT_C_LIMITS_H
}
#endif

// --------------------------------------------------------------------------

#endif /* __LIMITS_H_CXX__ */
