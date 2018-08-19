// -*-C++-*- srccxx/collate.cc
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
// Title:   Implementations for the srccxx/collate category
// Version: $Id: collate.cc,v 1.1.1.1 2002/06/05 01:02:11 dietmar_kuehl Exp $

// Note: Currently, this is only a stub for the real implementation which
//       has still to be done!
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_COLLATE_CC__)
#define __SRCCXX_COLLATE_CC__ 1

#if !defined(_CXXRT_COLLATE_H__)
#  include <cxxrt/collate.h>
#endif
#if !defined(_CXXRT_ALLOC_H__)
#  include <cxxrt/alloc.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT>
locale::id collate<_CS_cT>::id;

template <class _CS_cT>
collate<_CS_cT>::collate(size_t _CS_refs):
        locale::facet(_CS_refs)
{
}
        
template <class _CS_cT>
collate<_CS_cT>::~collate()
{
}
        
template <class _CS_cT>
int collate<_CS_cT>::compare(_CS_cT const* _CS_b1, _CS_cT const* _CS_e1,
                                                                                                                                         _CS_cT const* _CS_b2, _CS_cT const* _CS_e2) const
{
        return do_compare(_CS_b1, _CS_e1, _CS_b2, _CS_e2);
}

template <class _CS_cT>
_CXXRT_TYPENAME collate<_CS_cT>::string_type
collate<_CS_cT>::transform(_CS_cT const* _CS_b, _CS_cT const* _CS_e) const
{
        return do_transform(_CS_b, _CS_e);
}

template <class _CS_cT>
long
collate<_CS_cT>::hash(_CS_cT const* _CS_b, _CS_cT const* _CS_e) const
{
        return do_hash(_CS_b, _CS_e);
}

// --------------------------------------------------------------------------
        
template <class _CS_cT>
int collate<_CS_cT>::do_compare(_CS_cT const*, _CS_cT const*,
                                                                                                                                                        _CS_cT const*, _CS_cT const*) const
{
        return 0; //-dk:TODO
}

template <class _CS_cT>
_CXXRT_TYPENAME collate<_CS_cT>::string_type
collate<_CS_cT>::do_transform(_CS_cT const*, _CS_cT const*) const
{
        return string_type(); //-dk:TODO
}

template <class _CS_cT>
long
collate<_CS_cT>::do_hash(_CS_cT const*, _CS_cT const*) const
{
        return 0; //-dk:TODO
}

// --------------------------------------------------------------------------

template <class _CS_cT>
collate_byname<_CS_cT>::collate_byname(char const*, size_t _CS_refs):
        collate<_CS_cT>(_CS_refs)
{
}
        
template <class _CS_cT>
int collate_byname<_CS_cT>::do_compare(_CS_cT const*, _CS_cT const*,
                                                                                                                                                                                 _CS_cT const*, _CS_cT const*) const
{
        return 0; //-dk:TODO
}

template <class _CS_cT>
_CXXRT_TYPENAME collate<_CS_cT>::string_type
collate_byname<_CS_cT>::do_transform(_CS_cT const*, _CS_cT const*) const
{
        return string_type(); //-dk:TODO
}

template <class _CS_cT>
long
collate_byname<_CS_cT>::do_hash(_CS_cT const*, _CS_cT const*) const
{
        return 0; //-dk:TODO
}


// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_COLLATE_CC__ */
