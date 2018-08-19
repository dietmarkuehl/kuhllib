// -*-C++-*- cxxrt/fstr_fwd.h
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
// Title:   Forward declarations for the file streams
// Version: $Id: fstr_fwd.h,v 1.1.1.1 2002/06/05 01:02:03 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_FSTR_FWD_H__)
#define _CXXRT_FSTR_FWD_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRTCF_CTRAITS_FWD_H__)
#  include <cxxrt/ctraits_fwd.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_charT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_charT>) >
class basic_filebuf;
template <class _CS_charT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_charT>) >
class basic_ifstream;
template <class _CS_charT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_charT>) >
class basic_ofstream;
template <class _CS_charT,
          class _CS_traits _CXXRT_TEMPLATE_DEF_ARG(char_traits<_CS_charT>) >
class basic_fstream;

// --------------------------------------------------------------------------

typedef basic_filebuf<char, char_traits<char> > filebuf;
typedef basic_ifstream<char, char_traits<char> > ifstream;
typedef basic_ofstream<char, char_traits<char> > ofstream;
typedef basic_fstream<char, char_traits<char> > fstream;

typedef basic_filebuf<wchar_t, char_traits<wchar_t> > wfilebuf;
typedef basic_ifstream<wchar_t, char_traits<wchar_t> > wifstream;
typedef basic_ofstream<wchar_t, char_traits<wchar_t> > wofstream;
typedef basic_fstream<wchar_t, char_traits<wchar_t> > wfstream;

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_FSTR_FWD_H__ */
