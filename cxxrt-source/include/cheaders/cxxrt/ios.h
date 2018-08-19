// -*-C++-*- cxxrt/ios.h
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
// Title:   Definition of the template class basic_ios
// Version: $Id: ios.h,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_IOS_H__)
#define _CXXRT_IOS_H__ 1

#if !defined(_CXXRT_IOSFWD_H__)
#  include <cxxrt/iosfwd.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/sbuf_fwd.h>
#endif
#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif
// #if !defined(_CXXRT_NUM_PUT_H__)
// #  include <cxxrt/num_put.h>
// #endif

#if !defined(_CXXRT_WEAK_NP)
#  define _CXXRT_WEAK_NP
#else
#  undef _CXXRT_WEAK_NP
#  define _CXXRT_WEAK_NP _CXXRT_WEAK
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
class basic_ios : public ios_base
{
public:
  typedef _CS_cT                        char_type;
  typedef typename _CS_traits::int_type int_type;
  typedef typename _CS_traits::pos_type pos_type;
  typedef typename _CS_traits::off_type off_type;
  typedef _CS_traits                    traits_type;

  operator void*() const;
  bool operator!() const;
  iostate rdstate() const;
  void clear(iostate = goodbit);
  void clear(io_state);
  void _CS_process_exception();
  void setstate(iostate);
  void setstate(io_state);
 
  bool good() const;
  bool eof()  const;
  bool fail() const;
  bool bad()  const;
  iostate exceptions() const;
  void exceptions(iostate);
  void exceptions(io_state);

  _CXXRT_EXPLICIT basic_ios(basic_streambuf<_CS_cT,_CS_traits>*);
  virtual ~basic_ios();

  basic_ostream<_CS_cT,_CS_traits>* tie() const;
  basic_ostream<_CS_cT,_CS_traits>* tie(basic_ostream<_CS_cT,_CS_traits>*);
  basic_streambuf<_CS_cT,_CS_traits>* rdbuf() const;
  basic_streambuf<_CS_cT,_CS_traits>* rdbuf(basic_streambuf<_CS_cT,_CS_traits>*);
  basic_ios& copyfmt(basic_ios const&);
  char_type fill() const;
  char_type fill(char_type);

  locale    imbue(locale const&);
  char      narrow(char_type, char) const;
  char_type widen(char) const;

  char_type _CS_newline() const;

protected:
  basic_ios();
  void init(basic_streambuf<_CS_cT,_CS_traits>*);

private:
  basic_ios(basic_ios const&);          // not defined
  void operator=(basic_ios const&);     // not defined

  basic_streambuf<_CS_cT, _CS_traits>* _CS_m_sbuf;
  iostate                              _CS_m_state;
  iostate                              _CS_m_exceptions;
  basic_ostream<_CS_cT, _CS_traits>*   _CS_m_tie;

  // static num_put<_CS_cT> const* _CS_get_num_put(basic_ios<_CS_cT, _CS_traits> const&) _CXXRT_WEAK_NP;

#if !defined(_CXXRT_USE_FACET_INSTALLS)
  char_type _CS_m_newline;
#endif

  char_type _CS_m_fill;

#if defined(_CXXRT_USE_FACET_INSTALLS)
  friend class locale;
  static _CS_cT _CS_locale_fill(ios_base const&);
  static _CS_cT (*_CS_s_fill)(ios_base const&);
#endif
};

// --------------------------------------------------------------------------

#if !defined(_CXXRT_NO_INLINE)

template <class _CS_cT, class _CS_traits>
inline bool
basic_ios<_CS_cT, _CS_traits>::good() const
{
  return _CS_m_state == goodbit;
}

template <class _CS_cT, class _CS_traits>
inline basic_ostream<_CS_cT,_CS_traits>*
basic_ios<_CS_cT, _CS_traits>::tie() const
{
  return _CS_m_tie;
}

template <class _CS_cT, class _CS_traits>
inline basic_streambuf<_CS_cT,_CS_traits>*
basic_ios<_CS_cT, _CS_traits>::rdbuf() const
{
  return _CS_m_sbuf;
}

#if !defined(_CXXRT_USE_FACET_INSTALLS)
template <class _CS_cT, class _CS_traits>
inline typename basic_ios<_CS_cT, _CS_traits>::char_type
basic_ios<_CS_cT, _CS_traits>::fill() const
{
  return _CS_m_fill;
}
#endif

#if !defined(_CXXRT_USE_FACET_INSTALLS)
template <class _CS_cT, class _CS_traits>
inline typename basic_ios<_CS_cT, _CS_traits>::char_type
basic_ios<_CS_cT, _CS_traits>::_CS_newline() const
{
  return _CS_m_newline;
}
#endif

#endif

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
inline void
basic_ios<_CS_cT, _CS_traits>::exceptions(io_state _CS_s)
{
  _CXXRT_DEPRECATED(io_state__is__deprecated,use__iostate__instead)
  exceptions(iostate(_CS_s));
}

template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::clear(io_state _CS_s)
{
  _CXXRT_DEPRECATED(io_state__is__deprecated,use__iostate__instead)
  clear(iostate(_CS_s));
}

template <class _CS_cT, class _CS_traits>
void
basic_ios<_CS_cT, _CS_traits>::setstate(io_state _CS_s)
{
  _CXXRT_DEPRECATED(io_state__is__deprecated,use__iostate__instead)
  setstate(iostate(_CS_s));
}

// --------------------------------------------------------------------------

#if defined(_CXXRT_MIN_IO) || defined(_CXXRT_USE_MIN_IO)
extern template class basic_ios<char>;
extern template class basic_ios<wchar_t>;
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_IOS_CC__)
#  include <srccxx/ios.cc>
#endif

#endif /* _CXXRT_IOS_H__ */
