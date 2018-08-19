// -*-C++-*- cxxrt/istream_it.h
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
// Title:   Declaration and definition of istream_iterator
// Version: $Id: istream_it.h,v 1.1.1.1 2002/06/05 01:02:05 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_ISTREAM_IT_H__)
#define _CXXRT_ISTREAM_IT_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

#if !defined(_CXXRT_ITERATOR_H__)
#  include <cxxrt/iterator.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif
#include <iosfwd>

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_T, class _CS_cT = char,
        class _CS_traits = char_traits<_CS_cT>,
        class _CS_Dist = ptrdiff_t>
class istream_iterator: public iterator<input_iterator_tag, _CS_T, _CS_Dist,
                                                                                                _CS_T const*, _CS_T const&>
{
public:
  typedef basic_istream<_CS_cT, _CS_traits> istream_type;
        typedef _CS_cT       char_type;
        typedef _CS_traits   traits_type;
        typedef _CS_Dist     different_type;
        typedef _CS_T        value_type;
        typedef _CS_T const* pointer;
        typedef _CS_T const& reference;
        typedef input_iterator_tag iterator_category;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_ISTREAM_IT_H__ */
