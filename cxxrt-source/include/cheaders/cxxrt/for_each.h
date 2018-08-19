// -*-C++-*- cxxrt/for_each.h
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
// Title:   Optimized implementation of the 'for_each()' algorithm
// Version: $Id: for_each.h,v 1.1.1.1 2002/06/05 01:01:56 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_FOR_EACH_H)
#define _CXXRT_FOR_EACH_H 1

#include <cxxrt/iterhelp.h>
#include <cxxrt/iterator.h>
#include <algorithm>

namespace _Cxxrt_algorithm
{

  // ------------------------------------------------------------------------
  // The general and simple case:

  template <class _CS_InIt, class _CS_Func>
  inline void
  _Cxxrt_for_each(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_Func& _CS_func, _Cxxrt_bool<true> const&)
  {
    for (; _CS_beg != _CS_end; ++_CS_beg)
      _CS_func(*_CS_beg);
  }

  // ------------------------------------------------------------------------
  // Hoist test for segments out of the inner loop:

  template <class _CS_InIt, class _CS_Func>
  inline void
  _Cxxrt_for_each(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_Func& _CS_func, _Cxxrt_bool<false> const&)
  {
    _Cxxrt_iterhelper<_CS_InIt> _CS_helper(_CS_beg, _CS_end);
    
    for (; _CS_helper; ++_CS_helper)
      _Cxxrt_for_each(_CS_helper.begin(), _CS_helper.end(), _CS_func, _Cxxrt_bool<true>());
  }

  // ------------------------------------------------------------------------
  // Some delegation functions which forward the request to the actual
  // counting function:

  template <class _CS_InIt, class _CS_Func>
  inline _CS_Func
  for_each(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_Func _CS_func)
  {
    _Cxxrt_for_each(_CS_beg, _CS_end, _CS_func, _Cxxrt_bool<false>());
    return _CS_func;
  }

// --------------------------------------------------------------------------

}

#endif /* _CXXRT_FOR_EACH_H */
