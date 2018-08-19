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
// Title:   Explicit instantiation of basic_ostream<char> 
// Version: $Id: costream.cpp,v 1.1.1.1 2002/06/05 01:00:19 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/ostream.cc>
#include <cxxrt/ctraits.h>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template class basic_ostream<char, char_traits<char> >;

// -------------------------------------------------------------------------- 

template basic_ostream<char, char_traits<char> >& endl(basic_ostream<char, char_traits<char> > &);
template basic_ostream<char, char_traits<char> >& ends(basic_ostream<char, char_traits<char> > &);
template basic_ostream<char, char_traits<char> >& flush(basic_ostream<char, char_traits<char> > &);

#if !defined(_MSC_VER)
template basic_ostream<char, char_traits<char> >&
  operator<< (basic_ostream<char, char_traits<char> >&, char);
#endif
template basic_ostream<char, char_traits<char> >&
  operator<< (basic_ostream<char, char_traits<char> >&, signed char);
template basic_ostream<char, char_traits<char> >&
  operator<< (basic_ostream<char, char_traits<char> >&, unsigned char);
template basic_ostream<char, char_traits<char> >&
  operator<< (basic_ostream<char, char_traits<char> >&, char const*);
template basic_ostream<char, char_traits<char> >&
  operator<< (basic_ostream<char, char_traits<char> >&, signed char const*);
template basic_ostream<char, char_traits<char> >&
  operator<< (basic_ostream<char, char_traits<char> >&, unsigned char const*);

template <>
char const* _CS_null<char>::_CS_str()
{
  static char const* rc = "(null)";
  return rc;
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
