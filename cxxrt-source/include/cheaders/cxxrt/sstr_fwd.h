// -*-C++-*- cxxrt/sstr_fwd.h
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
// Title:   Forward declaration of the string stream types
// Version: $Id: sstr_fwd.h,v 1.1.1.1 2002/06/05 01:01:47 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_SSTR_FWD_H__)
#define _CXXRT_SSTR_FWD_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_BSTR_FWD_H__)
#  include <cxxrt/bstr_fwd.h>
#endif
#if !defined(_CXXRTCF_CTRAITS_FWD_H__)
#  include <cxxrt/ctraits_fwd.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------
// allocator:

template <class _CS_T> class allocator;

// --------------------------------------------------------------------------

#if defined(__STL_USE_STD_ALLOCATORS)
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(allocator<_CS_cT>) >
class basic_stringbuf;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(allocator<_CS_cT>) >
class basic_istringstream;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(allocator<_CS_cT>) >
class basic_ostringstream;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(allocator<_CS_cT>) >
class basic_stringstream;
#else
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(alloc) >
class basic_stringbuf;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(alloc) >
class basic_istringstream;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(alloc) >
class basic_ostringstream;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>),
          class _CS_alloc  _CXXRT_TEMPLATE_DEF_ARG(alloc) >
class basic_stringstream;
#endif

#if defined(__STL_USE_STD_ALLOCATORS)
typedef basic_stringbuf<char, char_traits<char>, allocator<char> >
        stringbuf;
typedef basic_istringstream<char, char_traits<char>, allocator<char> >
        istringstream;
typedef basic_ostringstream<char, char_traits<char>, allocator<char> >
        ostringstream;
typedef basic_stringstream<char, char_traits<char>, allocator<char> >
        stringstream;

typedef basic_stringbuf<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
        wstringbuf;
typedef basic_istringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
        wistringstream;
typedef basic_ostringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
        wostringstream;
typedef basic_stringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
        wstringstream;
#else
typedef basic_stringbuf<char, char_traits<char>, alloc>
        stringbuf;
typedef basic_istringstream<char, char_traits<char>, alloc>
        istringstream;
typedef basic_ostringstream<char, char_traits<char>, alloc>
        ostringstream;
typedef basic_stringstream<char, char_traits<char>, alloc>
        stringstream;

typedef basic_stringbuf<wchar_t, char_traits<wchar_t>, alloc>
        wstringbuf;
typedef basic_istringstream<wchar_t, char_traits<wchar_t>, alloc>
        wistringstream;
typedef basic_ostringstream<wchar_t, char_traits<wchar_t>, alloc>
        wostringstream;
typedef basic_stringstream<wchar_t, char_traits<wchar_t>, alloc>
        wstringstream;
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_SSTR_FWD_H__ */
