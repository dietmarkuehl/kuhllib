// -*-C++-*- srccxx/osbuf_it.cc
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
// Title:   Implementations for ostreambuf_iterator
// Version: $Id: osbuf_it.cc,v 1.1.1.1 2002/06/05 01:02:25 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_OSBUF_IT_CC__)
#define __SRCCXX_OSBUF_IT_CC__ 1

#if !defined(_CXXRT_SBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif
#if !defined(_CXXRT_OSTREAM_H__)
#  include <cxxrt/ostream.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>::ostreambuf_iterator(
    ostreambuf_iterator<_CS_charT, _CS_traits>::streambuf_type *_CS_sb) throw():
  _CS_m_sbuf(_CS_sb),
  _CS_fail(false)
{
}

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>::ostreambuf_iterator(
    ostreambuf_iterator<_CS_charT, _CS_traits>::ostream_type& _CS_os) throw():
  _CS_m_sbuf(_CS_os.rdbuf()),
  _CS_fail(false)
{
}

#if 0
template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>::ostreambuf_iterator(
    ostreambuf_iterator const& _CS_it) throw():
  _CS_m_sbuf(_CS_it._CS_m_sbuf),
  _CS_fail(_CS_it._CS_fail)
{
}
#endif

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>&
ostreambuf_iterator<_CS_charT, _CS_traits>::operator= (
    ostreambuf_iterator const& _CS_it) throw()
{
  _CS_m_sbuf = _CS_it._CS_m_sbuf;
  _CS_fail = _CS_it._CS_fail;
  return *this;
}

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>::~ostreambuf_iterator() throw()
{
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>&
ostreambuf_iterator<_CS_charT, _CS_traits>::operator*() throw()
{
  return *this;
}

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>&
ostreambuf_iterator<_CS_charT, _CS_traits>::operator= (_CS_charT _CS_c)
{
  if (_CS_traits::eq_int_type(_CS_m_sbuf->sputc(_CS_c), _CS_traits::eof()))
    _CS_fail = true;
  return *this;
}

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>&
ostreambuf_iterator<_CS_charT, _CS_traits>::operator++ () throw()
{
  return *this;
}

template <class _CS_charT, class _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>
ostreambuf_iterator<_CS_charT, _CS_traits>::operator++ (int) throw()
{
  return *this;
}

template <class _CS_charT, class _CS_traits>
bool
ostreambuf_iterator<_CS_charT, _CS_traits>::failed() const throw()
{
  return _CS_fail;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_OSBUF_IT_CC__ */
