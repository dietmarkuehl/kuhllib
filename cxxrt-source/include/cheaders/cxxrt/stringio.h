// -*-C++-*- cxxrt/stringio.h
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
// Title:   Functions for basic_string I/O which should actually be in the <string> header
// Version: $Id: stringio.h,v 1.1.1.1 2002/06/05 01:01:53 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_STRINGIO_H__)
#define _CXXRT_STRINGIO_H__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_IOSFWD_H__)
#  include <cxxrt/iosfwd.h>
#endif
#if !defined(_CXXRT_BSTR_FWD_H__)
#  include <cxxrt/bstr_fwd.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------
// this should actually be in the <string> header:

template <class _CS_charT, class _CS_traits, class _CS_alloc>
basic_istream<_CS_charT, _CS_traits>&
getline(basic_istream<_CS_charT, _CS_traits>&,
        basic_string<_CS_charT, _CS_traits, _CS_alloc>&,
        _CS_charT);

template <class _CS_charT, class _CS_traits, class _CS_alloc>
basic_istream<_CS_charT, _CS_traits>&
getline(basic_istream<_CS_charT, _CS_traits>&,
        basic_string<_CS_charT, _CS_traits, _CS_alloc>&);

template <class _CS_charT, class _CS_traits, class _CS_alloc>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>&,
            basic_string<_CS_charT, _CS_traits, _CS_alloc> const&);

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_STRINGIO_H__ */
