// -*-C++-*- srccxx/codecvt.cc
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
// Title:   Implementation of common codecvt functions
// Version: $Id: codecvt.cc,v 1.1.1.1 2002/06/05 01:02:25 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_CODECVT_CC__)
#define __SRCCXX_CODECVT_CC__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_CODECVT_H__)
#  include <cxxrt/codecvt.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_internT, class _CS_externT, class _CS_stateT>
codecvt_base::result codecvt<_CS_internT, _CS_externT, _CS_stateT>::out(
        state_type &_CS_state,
        intern_type const *_CS_fbeg, intern_type const *_CS_fend, intern_type const *&_CS_fnext,
        extern_type *_CS_tbeg, extern_type *_CS_tend, extern_type *&_CS_tnext) const
{
        return do_out(_CS_state,
                                                                _CS_fbeg, _CS_fend, _CS_fnext,
                                                                _CS_tbeg, _CS_tend, _CS_tnext);
}

template <class _CS_internT, class _CS_externT, class _CS_stateT>
codecvt_base::result codecvt<_CS_internT, _CS_externT, _CS_stateT>::unshift(
        state_type &_CS_state,
        extern_type *_CS_tbeg, extern_type *_CS_tend, extern_type *&_CS_tnext) const
{
        return do_unshift(_CS_state,
                                                                                _CS_tbeg, _CS_tend, _CS_tnext);
}

template <class _CS_internT, class _CS_externT, class _CS_stateT>
codecvt_base::result codecvt<_CS_internT, _CS_externT, _CS_stateT>::in(
        state_type &_CS_state,
        extern_type const *_CS_fbeg, extern_type const *_CS_fend, extern_type const *&_CS_fnext,
        intern_type *_CS_tbeg, intern_type *_CS_tend, intern_type *&_CS_tnext) const
{
        return do_in(_CS_state,
                                                         _CS_fbeg, _CS_fend, _CS_fnext,
                                                         _CS_tbeg, _CS_tend, _CS_tnext);
}

template <class _CS_internT, class _CS_externT, class _CS_stateT>
codecvt<_CS_internT, _CS_externT, _CS_stateT>::~codecvt()
{
}

template <class _CS_internT, class _CS_externT, class _CS_stateT>
int codecvt<_CS_internT, _CS_externT, _CS_stateT>::encoding() const throw()
{
        return do_encoding();
}

template <class _CS_internT, class _CS_externT, class _CS_stateT>
bool codecvt<_CS_internT, _CS_externT, _CS_stateT>::always_noconv() const throw()
{
        return do_always_noconv();
}

template <class _CS_internT, class _CS_externT, class _CS_stateT>
int codecvt<_CS_internT, _CS_externT, _CS_stateT>::length(
        state_type const &_CS_state,
        extern_type const *_CS_beg, extern_type const *_CS_end,
        size_t _CS_max) const
{
        return do_length(_CS_state, _CS_beg, _CS_end, _CS_max);
}

template <class _CS_internT, class _CS_externT, class _CS_stateT>
int codecvt<_CS_internT, _CS_externT, _CS_stateT>::max_length() const throw()
{
        return do_max_length();
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_CODECVT_CC__ */
