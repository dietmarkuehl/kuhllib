// -*-C++-*- include/testtools.hpp
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
// Title:  stuff to simplify testing
// Version: $Id: testtools.hpp,v 1.1.1.1 2002/06/05 01:03:52 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(INCLUDE_TESTTOOLS_HPP)
#define INCLUDE_TESTTOOLS_HPP 1

// -------------------------------------------------------------------------- 

#include <iostream>

#if !defined(_CXXRT_STD)
#  define TTSTD std::
#else
#  define TTSTD _CXXRT_STD
#endif

// -------------------------------------------------------------------------- 

template <class T> inline void use(T&) {}
template <class T> inline void const_use(T const&) {}

template <class T> inline void check_type(T&) {}
template <class T> inline void check_fct_type(T const&) {}

template <class T> inline void test_type() { T* ptr = 0; use(ptr); }

template <class T> struct aux {};
template <class S, class T> inline void compare_types() 
{
  aux<S>  s;
  aux<T>& t = s;
  use(t);
}

bool settrue(bool& b) { return b = true; }

// -------------------------------------------------------------------------- 

namespace _Estd_safe {
  using TTSTD cout;
}

// -------------------------------------------------------------------------- 

namespace testtools
{

  // ------------------------------------------------------------------------

  template <class T>
  void use(T)
  {
  }

  // ------------------------------------------------------------------------

  template <class T1, class T2>
  void check_type(T2 t2)
  {
    T1* p1 = &t2;
    T2* p2 = p1;
    use(p2);
  }

  // ------------------------------------------------------------------------

  template <int N>
  struct check_static_const
  {
  };

  // ------------------------------------------------------------------------

  template <class T>
  void exist_type()
  {
    T* ptr = 0;
    use(ptr);
  }

  // ------------------------------------------------------------------------

  template <class T1, class T2>
  void check_base(T2 const& r)
  {
    T1 const* p = &r;
  }

  // ------------------------------------------------------------------------

} // namespace testtools

// -------------------------------------------------------------------------- 

#endif /* INCLUDE_TESTTOOLS_HPP */
