// -*-C++-*- cxxrt/find.h
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
// Title:   Optimized implementation of the 'find()' algorithm
// Version: $Id: find.h,v 1.1.1.1 2002/06/05 01:01:56 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_FIND_H)
#define _CXXRT_FIND_H 1

#include <cxxrt/iterhelp.h>
#include <cxxrt/iterator.h>
#include <algorithm>

namespace _Cxxrt_algorithm
{

  // ------------------------------------------------------------------------
  // The general and simple case:

  template <class _CS_InIt, class _CS_Pred>
  inline _CS_InIt
  _Cxxrt_find_if(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_Pred& _CS_pred, _Cxxrt_bool<true> const&)
  {
    return _CXXRT_STD find_if(_CS_beg, _CS_end, _CS_pred);
  }

  // ------------------------------------------------------------------------
  // Hoist test for segments out of the inner loop:

  template <class _CS_InIt, class _CS_Pred>
  inline _CS_InIt
  _Cxxrt_find_if(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_Pred& _CS_pred, _Cxxrt_bool<false> const&)
  {
    typedef typename _CXXRT_STD iterator_traits<_CS_InIt>::difference_type _CS_Difference;
    _CS_Difference _CS_rc = _CS_Difference();
    _Cxxrt_iterhelper<_CS_InIt> _CS_helper(_CS_beg, _CS_end);
    
    for (; _CS_helper; ++_CS_helper)
      {
        _Cxxrt_iterhelper<_CS_InIt>::iterator _CS_it =  _Cxxrt_find_if(_CS_helper.begin(), _CS_helper.end(), _CS_pred,
                                                                       typename _Cxxrt_iterhelper<_CS_InIt>::_Cxxrt_fundamental());
        if (_CS_it != _CS_helper.end())
          return _CS_helper._Cxxrt_done(_CS_it);
      }
    
    return _CS_end;
  }

  // ------------------------------------------------------------------------
  // Some delegation functions which forward the request to the actual
  // counting function:

  template <class _CS_InIt, class _CS_Pred>
  inline _CS_InIt
  find_if(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_Pred _CS_pred)
  {
    return _Cxxrt_find_if(_CS_beg, _CS_end, _CS_pred, _Cxxrt_bool<false>());
  }

  template <class _CS_InIt, class _CS_T>
  inline _CS_InIt
  find(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_T const& _CS_val)
  {
    return find_if(_CS_beg, _CS_end, _CXXRT_STD bind1st(_CXXRT_STD equal_to<_CS_T>(), _CS_val));
  }

// --------------------------------------------------------------------------

}

#endif /* _CXXRT_FIND_H */
