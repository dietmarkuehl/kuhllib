// -*-C++-*- wfstream.cc
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
// Title:   Definition of the fstream functions for character type wchar_t
// Version: $Id: wfstream.cpp,v 1.1.1.1 2002/06/05 01:00:20 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_MSC_VER)
//-dk:TODO port for MSVC++

#include <srccxx/fstream.cc>
#include <srccxx/filebuf.cc>
#include <srccxx/cvtbuf.cc>
#include <srccxx/convert.cc>

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

template class _CS_basic_fstream_pbase<wchar_t, char_traits<wchar_t> >;
template class basic_filebuf<wchar_t, char_traits<wchar_t> >;
template class basic_ifstream<wchar_t, char_traits<wchar_t> >;
template class basic_ofstream<wchar_t, char_traits<wchar_t> >;
template class basic_fstream<wchar_t, char_traits<wchar_t> >;
template class _CS_cvtbuf<wchar_t, char_traits<wchar_t>, _CS_sysfile>;
template class _CS_converter<wchar_t, char_traits<wchar_t>, _CS_sysfile>;
template class _CS_cvt_unbuffered<wchar_t, char_traits<wchar_t>, _CS_sysfile>;
template class _CS_cvt_noconv<wchar_t, char_traits<wchar_t>, _CS_sysfile>;

_CXXRT_NAMESPACE_END

#endif
