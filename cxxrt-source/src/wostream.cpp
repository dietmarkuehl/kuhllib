// -*-C++-*- wostream.cc
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
// Title:   Explicit instantiation of basic_ostream<wchar_t> 
// Version: $Id: wostream.cpp,v 1.1.1.1 2002/06/05 01:00:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/ostream.cc>
#include <cxxrt/ctraits.h>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template class _CXXRT_STD_NAME:: basic_ostream<wchar_t, _CXXRT_STD char_traits<wchar_t> >;

#if !defined(_MSC_VER)
//-dk:TODO
template basic_ostream<wchar_t, char_traits<wchar_t> >&
  operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, char);
#endif
template basic_ostream<wchar_t, char_traits<wchar_t> >&
  operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, wchar_t);
#if !defined(_MSC_VER)
template basic_ostream<wchar_t, char_traits<wchar_t> >&
  operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, char const*);
template basic_ostream<wchar_t, char_traits<wchar_t> >&
  operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >&, wchar_t const*);
#endif

template basic_ostream<wchar_t,char_traits<wchar_t> >& endl(basic_ostream<wchar_t,char_traits<wchar_t> > &);
template basic_ostream<wchar_t,char_traits<wchar_t> >& ends(basic_ostream<wchar_t,char_traits<wchar_t> > &);
template basic_ostream<wchar_t,char_traits<wchar_t> >& flush(basic_ostream<wchar_t,char_traits<wchar_t> > &);

template <>
wchar_t const* _CS_null<wchar_t>::_CS_str()
{
  static wchar_t const* rc = L"(null)";
  return rc;
}


// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
