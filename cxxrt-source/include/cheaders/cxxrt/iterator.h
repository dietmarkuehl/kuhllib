// -*-C++-*- cxxrt/iterator.h
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
// Title:   The basic iterator declarations and classes
// Version: $Id: iterator.h,v 1.1.1.1 2002/06/05 01:01:46 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_ITERATOR_H__)
#define _CXXRT_ITERATOR_H__ 1

#if 0
#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: input_iterator_tag {};
struct bidirectional_iterator_tag: forward_iterator_tag {};
struct random_access_iterator_tag: bidirectional_iterator_tag {};

// --------------------------------------------------------------------------

template <class _CS_It>
struct iterator_traits
{
        typedef typename _CS_It::difference_type   difference_type;
        typedef typename _CS_It::value_type        value_type;
        typedef typename _CS_It::pointer           pointer;
        typedef typename _CS_It::reference         reference;
        typedef typename _CS_It::iterator_category iterator_category;
};

// --------------------------------------------------------------------------

template <class _CS_T>
struct iterator_traits<_CS_T*>
{
        typedef ptrdiff_t                  difference_type;
        typedef _CS_T                value_type;
        typedef _CS_T*               pointer;
        typedef _CS_T&               reference;
        typedef random_access_iterator_tag iterator_category;
};

template <class _CS_T>
struct iterator_traits<_CS_T const*>
{
        typedef ptrdiff_t                  difference_type;
        typedef _CS_T                value_type;
        typedef _CS_T const*         pointer;
        typedef _CS_T const&         reference;
        typedef random_access_iterator_tag iterator_category;
};

// --------------------------------------------------------------------------

template <class _CS_Cat, class _CS_T, class _CS_Dist = ptrdiff_t,
          class _CS_Ptr = _CS_T*, class _CS_Ref = _CS_T&>
struct iterator
{
        typedef _CS_Dist different_type;
        typedef _CS_T    value_type;
        typedef _CS_Ptr  pointer;
        typedef _CS_Ref  reference;
        typedef _CS_Cat  iterator_category;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
#else
#  include <iterator>
#endif

#endif /* _CXXRT_ITERATOR_H__ */
