// -*-C++-*- srccxx/streambuf.cc
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
// Title:   Implementation of the basic_streambuf members
// Version: $Id: streambuf.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_STREAMBUF_CC__)
#define __SRCCXX_STREAMBUF_CC__ 1

#if !defined(_CXXRT_STREAMBUF_H___)
#  include <cxxrt/streambuf.h>
#endif

#if !defined(_CXXRT_TRACE)
#  define _CXXRT_TRACE(str)
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_charT, class _CS_traits>
basic_streambuf<_CS_charT, _CS_traits>::basic_streambuf(basic_streambuf const&)
{
  _CXXRT_TRACE("std::basic_streambuf::basic_streambuf(basic_streambuf const&)");
}

template <class _CS_charT, class _CS_traits>
void
basic_streambuf<_CS_charT, _CS_traits>::operator= (basic_streambuf const&)
{
  _CXXRT_TRACE("std::basic_streambuf::operator= (basic_streambuf const&)");
}
#endif

template <class _CS_charT, class _CS_traits>
basic_streambuf<_CS_charT, _CS_traits>::basic_streambuf():
  _CS_gbeg(0),
  _CS_gcur(0),
  _CS_gend(0),
  _CS_pbeg(0),
  _CS_pcur(0),
  _CS_pend(0),
  _CS_loc(new locale())
  // _CS_loc(locale())
{
  _CXXRT_TRACE("std::basic_streambuf::basic_streambuf()");
  //-dk:TODO caching of facets
}

template <class _CS_charT, class _CS_traits>
basic_streambuf<_CS_charT, _CS_traits>::~basic_streambuf()
{
  _CXXRT_TRACE("std::basic_streambuf::~basic_streambuf()");
  //-dk:TODO delete _CS_loc
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
_CXXRT_STD locale
basic_streambuf<_CS_charT, _CS_traits>::pubimbue(locale const &_CS_l)
{
  _CXXRT_TRACE("std::basic_streambuf::pubimbue(locale const&)");
  locale _CS_rc(*_CS_loc);
  imbue(_CS_l);
  *_CS_loc = _CS_l;
  return _CS_rc;
}

template <class _CS_charT, class _CS_traits>
_CXXRT_STD locale
basic_streambuf<_CS_charT, _CS_traits>::getloc() const
{
  _CXXRT_TRACE("std::basic_streambuf::getloc() const");
  return *_CS_loc;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
_CXXRT_STD basic_streambuf<_CS_charT, _CS_traits> *
basic_streambuf<_CS_charT, _CS_traits>::pubsetbuf(char_type *_CS_s,
                   streamsize _CS_n)
{
  _CXXRT_TRACE("std::basic_streambuf::pubsetbuf(char_type*, streamsize)");
  return setbuf(_CS_s, _CS_n);
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::pos_type
basic_streambuf<_CS_charT, _CS_traits>::pubseekoff(off_type _CS_off,
              ios_base::seekdir _CS_way,
              ios_base::openmode _CS_which)
{
  _CXXRT_TRACE("std::basic_streambuf::pubseekoff(off_type, ios_base::seekdir, ios_base::openmode)");
  return seekoff(_CS_off, _CS_way, _CS_which);
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::pos_type
basic_streambuf<_CS_charT, _CS_traits>::pubseekpos(pos_type _CS_pos,
              ios_base::openmode _CS_which)
{
  _CXXRT_TRACE("std::basic_streambuf::pubseekpos(pos_type, ios_base::openmode)");
  return seekpos(_CS_pos, _CS_which);
}

template <class _CS_charT, class _CS_traits>
int
basic_streambuf<_CS_charT, _CS_traits>::pubsync()
{
  _CXXRT_TRACE("std::basic_streambuf::pubync()");
  return sync();
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
_CXXRT_STD streamsize
basic_streambuf<_CS_charT, _CS_traits>::in_avail()
{
  _CXXRT_TRACE("std::basic_streambuf::in_avail()");
  return _CS_gend != _CS_gcur
    ? streamsize(_CS_gend - _CS_gcur)
    : streamsize(showmanyc());
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::snextc()
{
  _CXXRT_TRACE("std::basic_streambuf::snextc()");
  switch (_CS_gend - _CS_gcur)
    {
    case 0:
      return _CS_traits::eq_int_type(sbumpc(), _CS_traits::eof())? _CS_traits::eof(): sgetc();
    case 1:
      gbump(1);
      return sgetc();
    default:
      gbump(1);
      return _CS_traits::to_int_type(*gptr());
    }
}

#if 0
template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::sbumpc()
{
  _CXXRT_TRACE("std::basic_streambuf::sbumpc()");
  return _CS_gend == _CS_gcur? uflow(): _CS_traits::to_int_type(*_CS_gcur++);
}
#endif

#if 0
template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::sgetc()
{
  _CXXRT_TRACE("std::basic_streambuf::sgetc()");
  return _CS_gend == _CS_gcur? underflow(): _CS_traits::to_int_type(*_CS_gcur);
}
#endif

template <class _CS_charT, class _CS_traits>
_CXXRT_STD streamsize
basic_streambuf<_CS_charT, _CS_traits>::sgetn(char_type* _CS_s,
                                              _CXXRT_STD streamsize _CS_n)
{
  _CXXRT_TRACE("std::basic_streambuf::sgetn(char_type*, streamsize)");
  return xsgetn(_CS_s, _CS_n);
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::sputbackc(char_type _CS_c)
{
  _CXXRT_TRACE("std::basic_streambuf::sputbackc(char_type)");
  if (_CS_gbeg == _CS_gcur || !_CS_traits::eq(_CS_c, _CS_gcur[-1]))
    return pbackfail(_CS_traits::to_int_type(_CS_c));
  else
    return _CS_traits::to_int_type(*--_CS_gcur);
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::sungetc()
{
  _CXXRT_TRACE("std::basic_streambuf::sungetc()");
  return _CS_gbeg == _CS_gcur? pbackfail(): _CS_traits::to_int_type(*--_CS_gcur);
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
_CXXRT_STD streamsize
basic_streambuf<_CS_charT, _CS_traits>::sputn(char_type const *_CS_s, _CXXRT_STD streamsize _CS_n)
{
  _CXXRT_TRACE("std::basic_streambuf::sputn(char_type const*, streamsize)");
  return xsputn(_CS_s, _CS_n);
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::char_type *
basic_streambuf<_CS_charT, _CS_traits>::eback() const
{
  _CXXRT_TRACE("std::basic_streambuf::eback() const");
  return _CS_gbeg;
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::char_type *
basic_streambuf<_CS_charT, _CS_traits>::gptr() const
{
  _CXXRT_TRACE("std::basic_streambuf::gptr() const");
  return _CS_gcur;
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::char_type *
basic_streambuf<_CS_charT, _CS_traits>::egptr() const
{
  _CXXRT_TRACE("std::basic_streambuf::egptr() const");
  return _CS_gend;
}

template <class _CS_charT, class _CS_traits>
void
basic_streambuf<_CS_charT, _CS_traits>::gbump(int _CS_n)
{
  _CXXRT_TRACE("std::basic_streambuf::gbump(int)");
  //-dk: what happens if _CS_n > _CS_gend - _CS_gcur
  //-dk: or _CS_n < (_CS_gcur - _CS_gbeg)?
  _CS_gcur += _CS_n;
}

template <class _CS_charT, class _CS_traits>
void
basic_streambuf<_CS_charT, _CS_traits>::setg(char_type *_CS_beg,
                                             char_type *_CS_cur,
                                             char_type *_CS_bufend)
{
  _CXXRT_TRACE("std::basic_streambuf::setg(char_type*, char_type*, char_type*)");
  _CS_gbeg = _CS_beg;
  _CS_gcur = _CS_cur;
  _CS_gend = _CS_bufend;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::char_type *
basic_streambuf<_CS_charT, _CS_traits>::pbase() const
{
  _CXXRT_TRACE("std::basic_streambuf::pbase() const");
  return _CS_pbeg;
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::char_type *
basic_streambuf<_CS_charT, _CS_traits>::pptr() const
{
  _CXXRT_TRACE("std::basic_streambuf::pptr() const");
  return _CS_pcur;
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::char_type *
basic_streambuf<_CS_charT, _CS_traits>::epptr() const
{
  _CXXRT_TRACE("std::basic_streambuf::epptr() const");
  return _CS_pend;
}

template <class _CS_charT, class _CS_traits>
void
basic_streambuf<_CS_charT, _CS_traits>::setp(char_type *_CS_beg,
                                             char_type *_CS_bufend)
{
  _CXXRT_TRACE("std::basic_streambuf::setp(char_type*, char_type*)");
  _CS_pbeg = _CS_pcur = _CS_beg;
  _CS_pend = _CS_bufend;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
void
basic_streambuf<_CS_charT, _CS_traits>::imbue(locale const &)
{
  _CXXRT_TRACE("std::basic_streambuf::imbue(locale const&)");
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
_CXXRT_STD basic_streambuf<_CS_charT, _CS_traits> *
basic_streambuf<_CS_charT, _CS_traits>::setbuf(char_type *, _CXXRT_STD streamsize)
{
  _CXXRT_TRACE("std::basic_streambuf::setbuf(char_type*, streamsize)");
  if (!(_CS_gcur != 0 && _CS_gcur != _CS_gend))
    {
      //-dk: what is to be done here?
    }
  return this;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::pos_type
basic_streambuf<_CS_charT, _CS_traits>::seekoff(off_type,
                 ios_base::seekdir,
                 ios_base::openmode)
{
  _CXXRT_TRACE("std::basic_streambuf::seekoff(off_type, ios_base::seekdir, ios_base::openmode)");
  return pos_type(off_type(-1));
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::pos_type
basic_streambuf<_CS_charT, _CS_traits>::seekpos(pos_type,
                 ios_base::openmode)
{
  _CXXRT_TRACE("std::basic_streambuf::seekpos(pos_type, ios_base::openmode)");
  return pos_type(off_type(-1));
}

template <class _CS_charT, class _CS_traits>
int
basic_streambuf<_CS_charT, _CS_traits>::sync()
{
  _CXXRT_TRACE("std::basic_streambuf::sync()");
  return 0;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
streamsize
basic_streambuf<_CS_charT, _CS_traits>::showmanyc()
{
  _CXXRT_TRACE("std::basic_streambuf::showmanyc()");
  return 0;
}

template <class _CS_charT, class _CS_traits>
_CXXRT_STD streamsize
basic_streambuf<_CS_charT, _CS_traits>::xsgetn(char_type *_CS_s,
                                                           _CXXRT_STD streamsize _CS_n)
{
  _CXXRT_TRACE("std::basic_streambuf::xsgetn(char_type*, streamsize)");
  _CXXRT_STD streamsize _CS_count = 0;
  while (_CS_count < _CS_n)
    {
      if (gptr() != egptr()) // if there is get buffer, drain this one first
        {
          streamsize _CS_bsize(egptr() - gptr());
          streamsize _CS_csize(_CS_n - _CS_count);
          _CS_csize = _CS_bsize < _CS_csize? _CS_bsize: _CS_csize;
          _CS_traits::copy(_CS_s, gptr(), _CS_csize);
          gbump(_CS_csize);
          _CS_s += _CS_csize;
          _CS_count += _CS_csize;
        }
      else // then do what the standard pretends is always done: get individual characters.
      {
        int_type _CS_c(sbumpc());
        if (_CS_traits::eq_int_type(_CS_traits::eof(), _CS_c))
          break;
        _CS_traits::assign(*_CS_s++, _CS_traits::to_char_type(_CS_c));
        ++_CS_count;
      }
    }
  return _CS_count;
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::underflow()
{
  _CXXRT_TRACE("std::basic_streambuf::underflow()");
  return _CS_traits::eof();
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::uflow()
{
  _CXXRT_TRACE("std::basic_streambuf::uflow()");
  return traits_type::eq_int_type(underflow(), traits_type::eof())
    ? traits_type::eof()
    : traits_type::to_int_type(*_CS_gcur++);
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
typename basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::pbackfail(int_type)
{
  _CXXRT_TRACE("std::basic_streambuf::pbackfail(int_type)");
  return _CS_traits::eof();
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
streamsize
basic_streambuf<_CS_charT, _CS_traits>::xsputn(char_type const *_CS_s,
                                               streamsize _CS_n)
{
  _CXXRT_TRACE("std::basic_streambuf::xsputn(char_type const*, streamsize)");
  if (_CS_n < epptr() - pptr())
    {
      _CS_traits::copy(pptr(), _CS_s, _CS_n);
      pbump(_CS_n);
      return _CS_n;
    }

  _CS_charT const* _CS_cur = _CS_s;
  _CS_charT const* _CS_bufend = _CS_s + _CS_n;
  while (_CS_cur != _CS_bufend)
    {
      streamsize _CS_t = epptr() - pptr();
      if (_CS_t != 0)
        {
          if (_CS_bufend - _CS_cur < _CS_t)
            _CS_t = _CS_bufend - _CS_cur;
          _CS_traits::copy(pptr(), _CS_cur, _CS_t);
          _CS_cur += _CS_t;
          pbump(_CS_t);
        }
      else
        if (_CS_traits::eq_int_type(_CS_traits::eof(), sputc(*_CS_cur)))
          break;
        else
          ++_CS_cur;
    }
  return _CS_cur - _CS_s;
}

template <class _CS_charT, class _CS_traits>
typename _CXXRT_STD basic_streambuf<_CS_charT, _CS_traits>::int_type
basic_streambuf<_CS_charT, _CS_traits>::overflow(int_type)
{
  _CXXRT_TRACE("std::basic_streambuf::overflow(int_type)");
  return _CS_traits::eof();
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_STREAMBUF_CC__ */
