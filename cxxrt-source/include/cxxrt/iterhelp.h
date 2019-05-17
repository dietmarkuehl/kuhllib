// -*-C++-*- cxxrt/iterhelp.h
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
// Title:   Auxiliary classes for specialized algorithms
// Version: $Id: iterhelp.h,v 1.1.1.1 2002/06/05 01:01:50 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_ITERHELP_H_)
#define _CXXRT_ITERHELP_H_ 1

namespace _Cxxrt_algorithm
{

  template <bool> struct _Cxxrt_bool {};
  
  // ------------------------------------------------------------------------
  
  template <typename _CS_Iterator>
  class _Cxxrt_iterhelper
  {
  public:
    typedef _Cxxrt_bool<true> _Cxxrt_fundamental;
    typedef _CS_Iterator      iterator;
    
    _Cxxrt_iterhelper(_CS_Iterator _CS_beg, _CS_Iterator _CS_end):
      _CS_m_beg(_CS_beg),
      _CS_m_end(_CS_end)
    {
    }

    operator void const*() const { return _CS_m_beg == _CS_m_end? 0: this; }
    _Cxxrt_iterhelper& operator++() { ++_CS_m_beg; return *this; }
    _CS_Iterator       begin() { return _CS_m_beg; }
    _CS_Iterator       end()   { return _CS_m_end; }
    _CS_Iterator       _Cxxrt_done(_CS_Iterator _CS_it) const { return _CS_it; }

  private:
    _CS_Iterator _CS_m_beg;
    _CS_Iterator _CS_m_end;
  };

  // ------------------------------------------------------------------------

} // namespace _Cxxrt_algorithm

#endif /* _CXXRT_ITERHELP_H_ */
