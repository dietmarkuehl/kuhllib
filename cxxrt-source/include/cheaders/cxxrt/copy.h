// -*-C++-*- cxxrt/copy.h
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
// Title:   Optimized implementation of the copy() algorithm
// Version: $Id: copy.h,v 1.1.1.1 2002/06/05 01:01:55 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_COPY_H)
#define _CXXRT_COPY_H 1

namespace _Cxxrt_algorithm
{

  // ------------------------------------------------------------------------
  
  template <class _CS_InIt, class _CS_OutIt>
  _CS_OutIt
  copy(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_OutIt _CS_to)
  {
    for (; _CS_beg != _CS_end; ++_CS_beg, ++_CS_to)
      *_CS_to = *_CS_beg;
    return _CS_to;
  }

  template <class _CS_InIt> class _CXXRT_STD reverse_iterator;

  template <class _CS_InIt, class _CS_OutIt>
  _CS_OutIt
  copy(_CXXRT_STD reverse_iterator<_CS_InIt> _CS_beg,
       _CXXRT_STD reverse_iterator<_CS_InIt> _CS_end,
       _CS_OutIt _CS_to)
  {
    _CS_InIt _CS_rbeg = _CS_beg.base();
    _CS_InIt _CS_rend = _CS_end.base();
    
    for (; _CS_rbeg != _CS_rend; ++_CS_to)
      *_CS_to = *--_CS_rbeg;

    return _CS_to;
  }

  // ------------------------------------------------------------------------

} // namespace _Cxxrt_algorithm

#endif /* _CXXRT_COPY_H */
