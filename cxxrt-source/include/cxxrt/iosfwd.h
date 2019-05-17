// -*-C++-*- iosfwd.h
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
// Title:   Forward declaration for IOStream stuff 
// Version: $Id: iosfwd.h,v 1.1.1.1 2002/06/05 01:01:45 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_IOSFWD_H__)
#define _CXXRT_IOSFWD_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(__CWCHAR__)
#  include <cwchar>
#endif

_CXXRT_NAMESPACE_BEGIN

#if defined(_CRT_STD)
  using _CRT_STD mbstate_t;
  using _CRT_STD wint_t;
#endif

// --------------------------------------------------------------------------

template <class _CS_cT> struct char_traits;
_CXXRT_FULL_SPECIALIZATION struct char_traits<char>;
_CXXRT_FULL_SPECIALIZATION struct char_traits<wchar_t>;
template <class _CS_state_t> class fpos;

typedef fpos<mbstate_t> streampos;
typedef fpos<mbstate_t> wstreampos;

// --------------------------------------------------------------------------
// base classes of the IOStream library:

template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>) >
class basic_ios;

template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>) >
class basic_istream;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>) >
class basic_ostream;
template <class _CS_cT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_cT>) >
class basic_iostream;

// --------------------------------------------------------------------------
// typedefs for the 'char' IOStreams:

typedef basic_ios<char, char_traits<char> >       ios;

typedef basic_istream<char, char_traits<char> >   istream;
typedef basic_ostream<char, char_traits<char> >   ostream;
typedef basic_iostream<char, char_traits<char> >  iostream;

// --------------------------------------------------------------------------
// typedefs for the 'wchar_t' IOStreams

typedef basic_ios<wchar_t, char_traits<wchar_t> >      wios;
typedef basic_istream<wchar_t, char_traits<wchar_t> >  wistream;
typedef basic_ostream<wchar_t, char_traits<wchar_t> >  wostream;
typedef basic_iostream<wchar_t, char_traits<wchar_t> > wiostream;

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_IOSFWD_H__ */
