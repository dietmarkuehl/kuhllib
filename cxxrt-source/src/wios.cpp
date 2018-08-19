// -*-C++-*- wios.cc
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
// Title:   instantiation of basic_ios<wchar_t>
// Version: $Id: wios.cpp,v 1.1.1.1 2002/06/05 01:00:20 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/ios.cc>

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

static wchar_t _CS_simple(ios_base const&) { return L' '; }

#if defined(_CXXRT_USE_FACET_INSTALLS)
wchar_t (*basic_ios<wchar_t, char_traits<wchar_t> >::_CS_s_fill)(ios_base const&) = _CS_simple;
#endif

template class basic_ios<wchar_t, char_traits<wchar_t> >;

_CXXRT_NAMESPACE_END
