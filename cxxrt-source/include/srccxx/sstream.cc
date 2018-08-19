// -*-C++-*- srccxx/sstream.cc
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
// Title:   Implementation of the string stream classes 
// Version: $Id: sstream.cc,v 1.2 2006/04/07 07:07:13 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_SSTREAM_CC__)
#define __SRCCXX_SSTREAM_CC__ 1

#if !defined(_CXXRT_SSTREAM_H__)
#  include <cxxrt/sstream.h>
#endif

#include <cxxrt/string.h>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::basic_stringbuf(basic_stringbuf const&)
{
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
void
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::operator= (basic_stringbuf const&)
{
}
#endif

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::basic_stringbuf(ios_base::openmode _CS_m):
  _CS_m_mode(_CS_m),
  _CS_m_beg(new _CS_cT[0]),
  _CS_m_end(_CS_m_beg)
{
  if (_CS_m & ios_base::in)
    this->setg(_CS_m_beg, _CS_m_beg, _CS_m_end);
  if (_CS_m & ios_base::out)
    this->setp(_CS_m_beg, _CS_m_end);
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::basic_stringbuf(string_type const &_CS_s,
                                                         ios_base::openmode _CS_m):
  _CS_m_mode(_CS_m),
  _CS_m_beg(new _CS_cT[_CS_s.size()]),
  _CS_m_end(_CS_m_beg + _CS_s.size())
{
  _CS_Tr::copy(_CS_m_beg, _CS_s.data(), _CS_s.size());
  if (_CS_m_mode & ios_base::in)
    this->setg(_CS_m_beg, _CS_m_beg, _CS_m_end);
  if (_CS_m_mode & ios_base::out)
    this->setp(_CS_m_beg, _CS_m_end);
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::~basic_stringbuf()
{
  delete[] _CS_m_beg;
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_string<_CS_cT, _CS_Tr, _CS_Al>
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::str() const
{
  if (this->pbase() == this->epptr())
    if (this->eback() == this->egptr())
      return basic_string<_CS_cT, _CS_Tr, _CS_Al>();
    else
      return basic_string<_CS_cT, _CS_Tr, _CS_Al>(this->eback(), this->egptr());
  else
    return basic_string<_CS_cT, _CS_Tr, _CS_Al>(this->pbase(), this->epptr());
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
void
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::str(string_type const &_CS_s)
{
  _CS_cT* _CS_nv = new _CS_cT[_CS_s.size() == 0? 1: _CS_s.size()];
  _CS_Tr::copy(_CS_nv, _CS_s.data(), _CS_s.size());
  delete[] _CS_m_beg;

  _CS_m_beg = _CS_nv;
  _CS_m_end = _CS_m_beg + _CS_s.size();
  if (_CS_m_mode & ios_base::in)
    this->setg(_CS_m_beg, _CS_m_beg, _CS_m_end);
  if (_CS_m_mode & ios_base::out)
    this->setp(_CS_m_beg, _CS_m_end);
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
typename basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::int_type
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::underflow()
{
  return this->gptr() == this->egptr()? _CS_Tr::eof()
    : _CS_Tr::to_int_type(*this->gptr());
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
typename basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::int_type
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::pbackfail(int_type _CS_c)
{
  if (this->eback() != this->gptr())
    {
      if (_CS_Tr::eq_int_type(_CS_c, _CS_Tr::eof()))
        {
          this->gbump(-1);
          return _CS_Tr::not_eof(_CS_c);
        }
      else
        if (_CS_Tr::eq(_CS_Tr::to_char_type(_CS_c), this->gptr()[-1])
            || (_CS_m_mode & ios_base::out) != 0)
          {
            this->gbump(-1);
            _CS_Tr::assign(*this->gptr(), _CS_Tr::to_char_type(_CS_c));
            return _CS_c;
          }
    }
  // fails if
  // - no read position is available
  // - c is not eof, does not match the last read char, and the stream is
  //   read-only
  return _CS_Tr::eof();
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
typename basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::int_type
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::overflow(int_type _CS_c)
{
  static int const _CS_alsize = 64;

  if (_CS_Tr::eq_int_type(_CS_c, _CS_Tr::eof()))
    return _CS_Tr::not_eof(_CS_c);
  else if (_CS_m_mode & ios_base::out)
    {
      if (_CS_m_beg == 0)
        {
          _CS_m_beg = new _CS_cT[_CS_alsize];
          _CS_m_end = _CS_m_beg + _CS_alsize;
          this->setp(_CS_m_beg, _CS_m_beg);
        }

      if (this->pptr() != this->epptr())
        {
          _CS_Tr::assign(*this->pptr(), _CS_Tr::to_char_type(_CS_c));
          this->pbump(1);
        }
      else if (this->epptr() != _CS_m_end)
        {
          _CS_Tr::assign(*this->pptr(), _CS_Tr::to_char_type(_CS_c));
          this->setp(this->pbase(), this->epptr() + 1);
          this->pbump(this->epptr() - this->pbase());
          if (this->eback() != 0)
            this->setg(this->eback(), this->gptr(), this->pptr());
        }
      else
        {
          streamsize _CS_size = _CS_m_end - _CS_m_beg;
          streamsize _CS_asize = _CS_size == 0? 256: 2 * _CS_size;
          _CS_cT* _CS_nv = new _CS_cT[_CS_asize];
          _CS_Tr::copy(_CS_nv, _CS_m_beg, _CS_size);
          delete[] _CS_m_beg;
          _CS_m_beg = _CS_nv;
          _CS_m_end = _CS_nv + _CS_asize;
          _CS_Tr::assign(_CS_m_beg[_CS_size], _CS_Tr::to_char_type(_CS_c));
          this->setp(_CS_m_beg, _CS_m_beg + _CS_size + 1);
          this->pbump(_CS_size + 1);
          if (_CS_m_mode & ios_base::in)
            this->setg(this->pbase(), this->pbase() + (this->gptr() - this->eback()), this->pptr());
        }
      return _CS_c;
    }

  return _CS_Tr::eof();
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_streambuf<_CS_cT, _CS_Tr>*
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::setbuf(char_type*, streamsize)
{
  //-dk:TODO document that setbuf() has no effect! (implementation defined)
  return this;
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
typename basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::pos_type
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::seekoff(off_type _CS_off,
                                                                   ios_base::seekdir _CS_dir,
                                                                   ios_base::openmode _CS_which)
{
  _CS_cT* _CS_np = 0;
  _CS_cT* _CS_e = _CS_m_mode & ios_base::out? this->epptr(): this->egptr();

  switch (_CS_dir)
    {
    case ios_base::beg:
      if (_CS_off < 0 || (_CS_e - _CS_m_beg) < _CS_off)
        return pos_type(off_type(-1));
      _CS_np = _CS_m_beg + _CS_off;
      break;
    case ios_base::end:
      if (_CS_off > 0 || (_CS_e - _CS_m_beg) < -_CS_off)
        return pos_type(off_type(-1));
      _CS_np = _CS_e + _CS_off;
      break;
    case ios_base::cur:
      if ((_CS_which & (ios_base::in | ios_base::out)) == (ios_base::in | ios_base::out))
        return pos_type(off_type(-1));
      _CS_np = (_CS_which & ios_base::in)? this->gptr(): this->pptr();

      if (_CS_off < -(_CS_np - _CS_m_beg)
          || _CS_e - _CS_np < _CS_off)
        return pos_type(off_type(-1));
      _CS_np += _CS_off;
      break;
    }

  switch (_CS_which & (ios_base::in | ios_base::out))
    {
    case ios_base::in:
      if (_CS_m_mode & ios_base::in && this->gptr() != 0)
        this->gbump(_CS_np - this->gptr());
      else
        return pos_type(off_type(-1));
      break;
    case ios_base::out:
        if (_CS_m_mode & ios_base::out && this->pptr() != 0)
          this->pbump(_CS_np - this->pptr());
        else
          return pos_type(off_type(-1));
        break;
    case ios_base::_CS_inout:
      if ((_CS_m_mode & (ios_base::in | ios_base::out)) == (ios_base::in | ios_base::out)
          && this->gptr() != 0 && this->pptr() != 0)
        {
          this->gbump(_CS_np - this->gptr());
          this->pbump(_CS_np - this->pptr());
        }
      else
        return pos_type(off_type(-1));
      break;
    default:
      return pos_type(off_type(-1));
    }

  return pos_type(_CS_np - _CS_m_beg);
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
typename basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::pos_type
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::seekpos(pos_type _CS_pos,
                                                                   ios_base::openmode _CS_which)
{
  _CS_cT* _CS_p = (_CS_m_mode & ios_base::out)? this->epptr(): this->egptr();
  if (_CS_pos < 0 || (_CS_p - _CS_m_beg) < _CS_pos)
    return pos_type(off_type(-1));

  _CS_p = _CS_m_beg + _CS_pos;

  switch (_CS_which & (ios_base::in | ios_base::out) & _CS_m_mode)
    {
    case ios_base::in:
      this->gbump(_CS_p - this->gptr());
      break;
    case ios_base::out:
      this->pbump(_CS_p - this->pptr());
      break;
    case ios_base::_CS_inout:
      this->gbump(_CS_p - this->gptr());
      this->pbump(_CS_p - this->pptr());
      break;
    default:
      return pos_type(off_type(-1));
    }

  return _CS_pos;
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
int
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::sync()
{
  // do nothing, except signaling success
  return _CS_Tr::not_eof(_CS_Tr::eof());
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
void
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::imbue(locale const &)
{
  // do nothing
}

#if 0
template <class _CS_cT, class _CS_Tr, class _CS_Al>
streamsize
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::xsgetn(char_type* _CS_s,
                                                                  streamsize _CS_n)
{
  if ((_CS_m_mode & ios_base::in) != 0)
    {
      streamsize _CS_total(_CS_n);

      if (streamsize(_CS_m_rep.end() - _CS_m_rpos) > _CS_n)
        {
          for (; _CS_n > 0; --_CS_n)
            *_CS_s++ = *_CS_m_rpos++;
        }
      else
        {
          typename string_type::const_iterator _CS_end = _CS_m_rep.end();
          _CS_n -= _CS_end - _CS_m_rpos;
          while (_CS_m_rpos != _CS_end)
            *_CS_s++ = *_CS_m_rpos++;
        }

      return _CS_total - _CS_n;
    }
  return streamsize(0);
}
#endif

#if 0
template <class _CS_cT, class _CS_Tr, class _CS_Al>
streamsize
basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>::xsputn(char_type const* _CS_s,
                                                                  streamsize _CS_n)
{
  if ((_CS_m_mode & ios_base::out) != 0)
    {
      streamsize _CS_total(_CS_n);
      for (; _CS_m_wpos != _CS_m_rep.end() && _CS_n > 0; --_CS_n)
        _CS_Tr::assign(*_CS_m_wpos++, *_CS_s++);

      if (_CS_m_rep.size() + _CS_n <= _CS_m_rep.capacity())
        {
          _CS_m_rep.append(_CS_s, _CS_n);
          _CS_m_wpos = _CS_m_rep.end();
        }
      else
        {
          streamsize off = _CS_m_rpos - _CS_m_rep.begin();
          _CS_m_rep.append(_CS_s, _CS_n);
          _CS_m_rpos = _CS_m_rep.begin() + off;
          _CS_m_wpos = _CS_m_rep.end();
        }
      return _CS_total - _CS_n;
    }

  return streamsize(0);
}
#endif

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_Tr, class _CS_Al>
_CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>::_CS_basic_sstream_pbase(ios_base::openmode _CS_mode):
  _CS_m_ssbuf(_CS_mode)
{
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
_CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>::_CS_basic_sstream_pbase(_CS_string_type const& _CS_str,
                                                                        ios_base::openmode _CS_mode):
  _CS_m_ssbuf(_CS_str, _CS_mode)
{
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
_CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>::~_CS_basic_sstream_pbase()
{
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_string<_CS_cT, _CS_Tr, _CS_Al>
_CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>::str() const
{
  return _CS_m_ssbuf.str();
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
void
_CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>::str(_CS_string_type const& _CS_str)
{
  _CS_m_ssbuf.str(_CS_str);
}

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_istringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_istringstream(basic_istringstream const&):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(in),
  basic_ios<_CS_cT, _CS_Tr>(&_CS_m_ssbuf),
  basic_istream<_CS_cT, _CS_Tr>(&_CS_m_ssbuf)
{
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
void
basic_istringstream<_CS_cT, _CS_Tr, _CS_Al>::operator= (basic_istringstream const&)
{
}
#endif

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_istringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_istringstream(ios_base::openmode _CS_mode):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(_CS_mode | ios_base::in),
  basic_ios<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf),
  basic_istream<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf)
{
  this->init(&this->_CS_m_ssbuf);
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_istringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_istringstream(_CS_string_type const& _CS_str,
                                                                 ios_base::openmode _CS_mode):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(_CS_str, _CS_mode | ios_base::in),
  basic_ios<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf),
  basic_istream<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf)
{
  this->init(&this->_CS_m_ssbuf);
}

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_ostringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_ostringstream(basic_ostringstream const&):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(out),
  basic_ios<_CS_cT, _CS_Tr>(&_CS_m_ssbuf),
  basic_ostream<_CS_cT, _CS_Tr>(&_CS_m_ssbuf)
{
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
void
basic_ostringstream<_CS_cT, _CS_Tr, _CS_Al>::operator= (basic_ostringstream const&)
{
}
#endif

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_ostringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_ostringstream(ios_base::openmode _CS_mode):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(_CS_mode | ios_base::out),
  basic_ios<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf),
  basic_ostream<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf)
{
  this->init(&this->_CS_m_ssbuf);
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_ostringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_ostringstream(_CS_string_type const& _CS_str,
                                                                 ios_base::openmode _CS_mode):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(_CS_str, _CS_mode | ios_base::out),
  basic_ios<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf),
  basic_ostream<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf)
{
  this->init(&this->_CS_m_ssbuf);
}

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_stringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_stringstream(basic_stringstream const&):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(in | out),
  basic_ios<_CS_cT, _CS_Tr>(&_CS_m_ssbuf),
  basic_iostream<_CS_cT, _CS_Tr>(&_CS_m_ssbuf)
{
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
void
basic_stringstream<_CS_cT, _CS_Tr, _CS_Al>::operator= (basic_stringstream const&)
{
}
#endif

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_stringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_stringstream(ios_base::openmode _CS_mode):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(_CS_mode | ios_base::in | ios_base::out),
  basic_ios<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf),
  basic_iostream<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf)
{
  this->init(&this->_CS_m_ssbuf);
}

template <class _CS_cT, class _CS_Tr, class _CS_Al>
basic_stringstream<_CS_cT, _CS_Tr, _CS_Al>::basic_stringstream(_CS_string_type const& _CS_str,
                                                               ios_base::openmode _CS_mode):
  _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>(_CS_str, _CS_mode | ios_base::in | ios_base::out),
  basic_ios<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf),
  basic_iostream<_CS_cT, _CS_Tr>(&this->_CS_m_ssbuf)
{
  this->init(&this->_CS_m_ssbuf);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_SSTREAM_CC__ */
