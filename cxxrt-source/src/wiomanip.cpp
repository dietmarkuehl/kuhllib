// -*-C++-*- cfstream.cc
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
// Title:   Explicit instantiation of manipulators for wide character stream
// Version: $Id: wiomanip.cpp,v 1.1.1.1 2002/06/05 01:00:41 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ostream.h>
#include <cxxrt/istream.h>
#include <srccxx/iomanip.cc>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template _CS_setfill<wchar_t> setfill<wchar_t>(wchar_t);
template basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, _CS_setfill<wchar_t> const &);
template basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, _CS_setfill<wchar_t> const &);

template basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, _CS_setw const &);
template basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, _CS_setw const &);

template basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, _CS_setprecision const &);
template basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, _CS_setprecision const &);

template basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, _CS_resetiosflags const &);
template basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, _CS_resetiosflags const &);

template basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, _CS_setiosflags const &);
template basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, _CS_setiosflags const &);

template basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, _CS_setbase const &);
template basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, _CS_setbase const &);

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
