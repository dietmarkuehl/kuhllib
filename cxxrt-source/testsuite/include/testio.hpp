// -*-C++-*- testio.hpp
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
// Author: Dietmar Kuehl <http://www.dietmar-kuehl.de/>
// Title:   
// Version: $Id: testio.hpp,v 1.1.1.1 2002/06/05 01:03:51 dietmar_kuehl Exp $ 
// ----------------------------------------------------------------------- 

#if !defined(TESTIO_HPP)
#define TESTIO_HPP 1

#include <cstdio>

// ------------------------------------------------------------------------

namespace my_std
{
#if !defined(_CXXRT_STD)
#  define STD std::
  using namespace std;
  struct flush { ~flush() { fflush(0); } } dummy;
#else
#  define STD _CXXRT_STD
  using STD printf;
  using STD fprintf;
#  if defined(__STRING_H_CXX__)
  using STD strcmp;
#endif
#  if defined(__STDLIB_H_CXX__)
  using STD exit;
#endif
#endif
} // namespace my_std

void write(char const* msg) { my_std::printf(msg); }

// ------------------------------------------------------------------------

#endif /* TESTIO_HPP */
