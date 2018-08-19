// -*-C++-*- srccxx/ios.cc
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
// Title:   Definition of the members of basic_ios
// Version: $Id: ios.cc,v 1.3 2003/04/13 19:36:46 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_IOS_CC__)
#define __SRCCXX_IOS_CC__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_IOS_H__)
#  include <cxxrt/ios.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif
#if !defined(_CXXRT_CTYPE_H__)
#  include <cxxrt/ctype.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif
#include <iostream>

#if !defined(_CXXRT_TRACE)
#  define _CXXRT_TRACE(str)
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
basic_ios<_CS_cT, _CS_traits>::basic_ios(basic_ios const&)
{
  _CXXRT_TRACE("std::basic_ios::basic_ios(basic_ios const&)");
}

template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::operator=(basic_ios const&)
{
  _CXXRT_TRACE("std::basic_ios::operator=(basic_ios const&)");
}
#endif

template <class _CS_cT, class _CS_traits>
basic_ios<_CS_cT, _CS_traits>::basic_ios():
  _CS_m_sbuf(0),
  _CS_m_state(badbit),
  _CS_m_exceptions(goodbit),
  _CS_m_tie(0),
#if !defined(_CXXRT_USE_FACET_INSTALLS)
  _CS_m_newline(), //-dk:TODO handle general case somehow _CS_m_newline(_CS_cT())
#endif
  _CS_m_fill() //-dk:TODO handle general case somehow _CS_m_fill(_CS_cT()),
{
  _CXXRT_TRACE("std::basic_ios::basic_ios()");
  setf(_CS_os_good);
}

template <class _CS_cT, class _CS_traits>
basic_ios<_CS_cT, _CS_traits>::
basic_ios(basic_streambuf<_CS_cT, _CS_traits>* _CS_sb):
  _CS_m_sbuf(_CS_sb),
  _CS_m_state(_CS_m_sbuf == 0? badbit: goodbit),
  _CS_m_exceptions(goodbit),
  _CS_m_tie(0),
#if !defined(_CXXRT_USE_FACET_INSTALLS)
  _CS_m_newline(), //-dk:TODO handle general case somehow _CS_m_newline(_CS_cT())
#endif
  _CS_m_fill() //-dk:TODO handle general case somehow _CS_m_fill(widen(' ')),
{
  _CXXRT_TRACE("std::basic_ios::basic_ios(basic_streambuf*)");
  if (_CS_m_sbuf == 0)
    setf(_CS_os_good);
  init(_CS_sb);
}


template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::init(
  basic_streambuf<_CS_cT, _CS_traits>* _CS_sb)
{
  _CXXRT_TRACE("std::basic_ios::init(basic_streambuf*)");
  _CS_m_sbuf       = _CS_sb;
  _CS_m_state      = _CS_m_sbuf == 0? badbit: goodbit;
  if (_CS_m_sbuf == 0)
    setf(_CS_os_good);
  _CS_m_exceptions = goodbit;
  _CS_m_tie        = 0;
#if defined(_CXXRT_USE_FACET_INSTALLS)
  _CS_m_fill       = _CS_cT(); //-dk:TODO _CS_s_fill(*this);
#else
  _CS_m_fill       = widen(' ');
#endif
#if !defined(_CXXRT_USE_FACET_INSTALLS)
  _CS_m_newline    = widen('\n');
#endif
}

template <class _CS_cT, class _CS_traits>
basic_ios<_CS_cT, _CS_traits>::~basic_ios()
{
  _CXXRT_TRACE("std::basic_ios::~basic_ios()");
}

// --------------------------------------------------------------------------

#if 1 || !defined(_CXXRT_STANDALONE)

template <class _CS_cT, class _CS_traits>
basic_streambuf<_CS_cT, _CS_traits>*
basic_ios<_CS_cT, _CS_traits>::rdbuf(basic_streambuf<_CS_cT, _CS_traits>* _CS_sb)
{
  _CXXRT_TRACE("std::basic_ios::rdbuf(basic_streambuf*)");
  basic_streambuf<_CS_cT, _CS_traits>* _CS_rc = _CS_m_sbuf;
  _CS_m_sbuf = _CS_sb;
  clear();
  return _CS_rc;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>*
basic_ios<_CS_cT, _CS_traits>::tie(
  basic_ostream<_CS_cT, _CS_traits>* _CS_tie)
{
  _CXXRT_TRACE("std::basic_ios::tie(basic_ostream*)");
  basic_ostream<_CS_cT, _CS_traits>* _CS_rc = _CS_m_tie;
  _CS_m_tie = _CS_tie;
  if (_CS_m_tie == 0)
    unsetf(_CS_os_tie);
  else
    setf(_CS_os_tie);
  return _CS_rc;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
locale 
basic_ios<_CS_cT, _CS_traits>::imbue(locale const& _CS_l)
{
  _CXXRT_TRACE("std::basic_ios::imbue(locale const&)");
  locale _CS_rc = ios_base::imbue(_CS_l);
#if !defined(_CXXRT_USE_FACET_INSTALLS)
  _CS_m_newline = widen('\n');
#endif

  if (_CS_m_sbuf != 0)
    _CS_m_sbuf->pubimbue(_CS_l);
  return _CS_rc;
}

#if 1
template <class _CS_cT, class _CS_traits>
char
basic_ios<_CS_cT, _CS_traits>::narrow(
  _CS_cT _CS_c, 
  char _CS_default) const
{
  _CXXRT_TRACE("std::basic_ios::narrow(_CS_cT, char)");
  typedef ctype<_CS_cT> _CS_ctype;
  return use_facet<_CS_ctype>(getloc()).narrow(_CS_c, _CS_default);
}
#endif

#if 1 || !defined(_CXXRT_USE_FACET_INSTALLS)
template <class _CS_cT, class _CS_traits>
_CS_cT
basic_ios<_CS_cT, _CS_traits>::widen(char _CS_c) const
{
  _CXXRT_TRACE("std::basic_ios::widen(char)");
  typedef ctype<_CS_cT> _CS_ctype;
  return use_facet<_CS_ctype>(getloc()).widen(_CS_c);
}
#endif

// --------------------------------------------------------------------------

#if defined(_CXXRT_NO_INLINE) || defined(_CXXRT_USE_FACET_INSTALLS)
template <class _CS_cT, class _CS_traits>
_CS_cT
basic_ios<_CS_cT, _CS_traits>::fill() const
{
  _CXXRT_TRACE("std::basic_ios::fill()");
  return _CS_m_fill;
}
#endif

template <class _CS_cT, class _CS_traits>
_CS_cT
basic_ios<_CS_cT, _CS_traits>::fill(_CS_cT _CS_f)
{
  _CXXRT_TRACE("std::basic_ios::fill(_CS_cT)");
  _CS_cT _CS_rc = _CS_m_fill;
  _CS_m_fill = _CS_f;
  return _CS_rc;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_ios<_CS_cT, _CS_traits>&
basic_ios<_CS_cT, _CS_traits>::copyfmt(
  basic_ios<_CS_cT, _CS_traits> const& _CS_cpy)
{
  _CXXRT_TRACE("std::basic_ios::copyfmt(basic_ios const&)");
  _CS_call_callbacks(erase_event);

  _CS_m_tie = _CS_cpy._CS_m_tie;
  if (_CS_m_tie == 0)
    unsetf(_CS_os_tie);
  else
    setf(_CS_os_tie);
  _CS_m_fill = _CS_cpy._CS_m_fill;
  //_CS_m_exceptions = _CS_cpy._CS_m_exceptions;

  _CS_copyfmt(_CS_cpy);

  _CS_call_callbacks(copyfmt_event);
  exceptions(_CS_cpy._CS_m_exceptions);
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_ios<_CS_cT, _CS_traits>::operator void*() const
{
  _CXXRT_TRACE("std::basic_ios::operator void*() const");
  return (_CS_m_state & (failbit | badbit)) == 0? rdbuf(): 0;
}

template <class _CS_cT, class _CS_traits>
bool
basic_ios<_CS_cT, _CS_traits>::operator! () const
{
  _CXXRT_TRACE("std::basic_ios::operator!() const");
  return fail();
}

template <class _CS_cT, class _CS_traits>
ios_base::iostate
basic_ios<_CS_cT, _CS_traits>::rdstate() const
{
  _CXXRT_TRACE("std::basic_ios::rdstate() const");
  return _CS_m_state;
}

template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::clear(
  ios_base::iostate _CS_state)
{
  _CXXRT_TRACE("std::basic_ios::clear(iostate)");
  _CS_m_state = _CS_m_sbuf == 0? (_CS_state | badbit): _CS_state;
  if ((_CS_m_state & _CS_m_exceptions) != 0)
    throw failure("-dk:TODO");
  if (_CS_m_state == goodbit)
    unsetf(_CS_os_good);
  else
    setf(_CS_os_good);
}

template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::_CS_process_exception()
{
  _CXXRT_TRACE("std::basic_ios::_CS_process_exception()");
  _CS_m_state |= badbit;
  setf(_CS_os_good);
  if ((_CS_m_exceptions & badbit) != 0)
    throw; // rethrow exception
}

template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::setstate(
  ios_base::iostate _CS_state)
{
  _CXXRT_TRACE("std::basic_ios::setstate(iostate)");
  clear(_CS_m_state | _CS_state);
}

#if 0
template <class _CS_cT, class _CS_traits>
bool
basic_ios<_CS_cT, _CS_traits>::good() const
{
  _CXXRT_TRACE("std::basic_ios::good() const");
  return _CS_m_state == goodbit;
}
#endif

template <class _CS_cT, class _CS_traits>
bool
basic_ios<_CS_cT, _CS_traits>::eof() const
{
  _CXXRT_TRACE("std::basic_ios::eof() const");
  return (_CS_m_state & eofbit) != 0? true: false;
}

template <class _CS_cT, class _CS_traits>
bool
basic_ios<_CS_cT, _CS_traits>::fail() const
{
  _CXXRT_TRACE("std::basic_ios::fail() const");
  return (_CS_m_state & (failbit | badbit)) != 0? true: false;
}

template <class _CS_cT, class _CS_traits>
bool
basic_ios<_CS_cT, _CS_traits>::bad() const
{
  _CXXRT_TRACE("std::basic_ios::bad() const");
  return (_CS_m_state & badbit) != 0? true: false;
}

template <class _CS_cT, class _CS_traits>
ios_base::iostate
basic_ios<_CS_cT, _CS_traits>::exceptions() const
{
  _CXXRT_TRACE("std::basic_ios::exceptions() const");
  return _CS_m_exceptions;
}

template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::exceptions(
  ios_base::iostate _CS_exceptions)
{
  _CXXRT_TRACE("std::basic_ios::exceptions(iostate)");
  _CS_m_exceptions = _CS_exceptions;
  clear(_CS_m_state);
}

#if defined(_CXXRT_USE_FACET_INSTALLS)
template <class _CS_cT, class _CS_traits>
inline basic_ios<_CS_cT, _CS_traits>::char_type
basic_ios<_CS_cT, _CS_traits>::_CS_newline() const
{
  _CXXRT_TRACE("std::basic_ios::_CS_newline() const");
  return _CS_cT(); //-dk:TODO widen('\n');
}
#endif

#if 1
#if defined(_CXXRT_USE_FACET_INSTALLS)
template <class _CS_cT, class _CS_traits>
_CS_cT
basic_ios<_CS_cT, _CS_traits>::_CS_locale_fill(ios_base const& _CS_fmt)
{
  _CXXRT_TRACE("std::basic_ios::_CS_locale_fill(ios_base const&)");
  return use_facet<ctype<_CS_cT> >(_CS_fmt.getloc()).widen(' ');
}

template <class _CS_cT, class _CS_traits>
_CS_cT (*basic_ios<_CS_cT, _CS_traits>::_CS_s_fill)(ios_base const&) = _CS_locale_fill;
#endif
#endif

#endif /* standalone */

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_IOS_CC__ */
