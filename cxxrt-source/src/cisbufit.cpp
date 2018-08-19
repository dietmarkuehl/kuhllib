// -*-C++-*- src/cisbufit.cc
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
// Title:   explicit instantiation of istreambuf_iterator<char>
// Version: $Id: cisbufit.cpp,v 1.2 2002/06/06 01:56:55 dietmar_kuehl Exp $
// --------------------------------------------------------------------------

// #include <srccxx/isbuf_it.cc>
#include <cxxrt/sbuf_it.h>
#include <cxxrt/istream.h>
#include <srccxx/isbuf_it.cc>

// --------------------------------------------------------------------------

#if 1
template class _CXXRT_STD_NAME::istreambuf_iterator<char>;

_CXXRT_NAMESPACE_BEGIN
template bool operator== (istreambuf_iterator<char, char_traits<char> > const &, istreambuf_iterator<char, char_traits<char> > const &);
template bool operator!= (istreambuf_iterator<char, char_traits<char> > const &, istreambuf_iterator<char, char_traits<char> > const &);

_CXXRT_NAMESPACE_END
#else
_CXXRT_NAMESPACE_BEGIN
istreambuf_iterator<char, char_traits<char> >::istreambuf_iterator(istream_type& in) throw():
  _CS_m_sbuf(in.rdbuf())
{
  if (_CS_m_sbuf && traits_type::eq_int_type(traits_type::eof(), _CS_m_sbuf->sgetc()))
    _CS_m_ok = false;
}
_CXXRT_NAMESPACE_END
#endif

