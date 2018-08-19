// -*-C++-*- srccxx/isbuf_it.cc
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
// Title:  Implementations for istreambuf_iterator
// Version: $Id: isbuf_it.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_ISBUF_IT_CC__)
#define __SRCCXX_ISBUF_IT_CC__ 1

#if !defined(_CXXRT_ISBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif
#if !defined(_CXXRT_ISTREAM_H__)
#  include <cxxrt/istream.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
istreambuf_iterator<_CS_cT, _CS_traits>::istreambuf_iterator() throw():
  _CS_m_sbuf(0)
{
}

template <class _CS_cT, class _CS_traits>
istreambuf_iterator<_CS_cT, _CS_traits>::istreambuf_iterator(
    istreambuf_iterator<_CS_cT, _CS_traits>::streambuf_type* _CS_sb) throw():
  _CS_m_sbuf(_CS_sb)
{
  if (_CS_m_sbuf && _CS_traits::eq_int_type(_CS_m_sbuf->sgetc(), _CS_traits::eof()))
    _CS_m_sbuf = 0;
}

template <class _CS_cT, class _CS_traits>
istreambuf_iterator<_CS_cT, _CS_traits>::istreambuf_iterator(
    istreambuf_iterator<_CS_cT, _CS_traits>::istream_type& _CS_is) throw():
  _CS_m_sbuf(_CS_is.rdbuf())
{
  if (_CS_m_sbuf && _CS_traits::eq_int_type(_CS_m_sbuf->sgetc(), _CS_traits::eof()))
    _CS_m_sbuf = 0;
}

template <class _CS_charT, class _CS_traits>
istreambuf_iterator<_CS_charT, _CS_traits>::istreambuf_iterator(
    istreambuf_iterator const& _CS_it):
  _CS_m_sbuf(_CS_it._CS_m_sbuf)
{
}

template <class _CS_charT, class _CS_traits>
istreambuf_iterator<_CS_charT, _CS_traits>&
istreambuf_iterator<_CS_charT, _CS_traits>::operator= (istreambuf_iterator const& _CS_it)
{
  _CS_m_sbuf = _CS_it._CS_m_sbuf;
  return *this;
}

template <class _CS_charT, class _CS_traits>
istreambuf_iterator<_CS_charT, _CS_traits>::~istreambuf_iterator()
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
_CS_cT
istreambuf_iterator<_CS_cT, _CS_traits>::operator*() const
{
  return _CS_traits::to_char_type(_CS_m_sbuf->sgetc());
}

template <class _CS_cT, class _CS_traits>
istreambuf_iterator<_CS_cT, _CS_traits>&
istreambuf_iterator<_CS_cT, _CS_traits>::operator++()
{
  if (_CS_traits::eq_int_type(_CS_m_sbuf->snextc(), _CS_traits::eof()))
    _CS_m_sbuf = 0;
  return *this;
}

template <class _CS_cT, class _CS_traits>
typename istreambuf_iterator<_CS_cT, _CS_traits>::proxy
istreambuf_iterator<_CS_cT, _CS_traits>::operator++(int)
{
  proxy _CS_rc(operator*());
  operator++();
  return _CS_rc;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
bool
istreambuf_iterator<_CS_cT, _CS_traits>::equal(
  istreambuf_iterator<_CS_cT, _CS_traits> const& _CS_sb) const
{
  return _CS_m_sbuf == 0
    ? _CS_sb._CS_m_sbuf == 0
    : _CS_sb._CS_m_sbuf != 0;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
bool
operator== (istreambuf_iterator<_CS_cT, _CS_traits> const& _CS_sb1,
            istreambuf_iterator<_CS_cT, _CS_traits> const& _CS_sb2)
{
  return _CS_sb1.equal(_CS_sb2);
}

template <class _CS_cT, class _CS_traits>
bool
operator!= (istreambuf_iterator<_CS_cT, _CS_traits> const& _CS_sb1,
            istreambuf_iterator<_CS_cT, _CS_traits> const& _CS_sb2)
{
  return !_CS_sb1.equal(_CS_sb2);
}


// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_ISBUF_IT_CC__ */
