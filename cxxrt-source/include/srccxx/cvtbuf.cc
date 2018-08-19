// -*-C++-*- srccxx/cvtbuf.cc
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
// Title:   A class for handling conversion between internal and external representation
// Version: $Id: cvtbuf.cc,v 1.3 2006/04/07 07:07:13 dietmar_kuehl Exp $

// Note:   Currently, this is still rather incomplete. There is lot of work
//         still to be done.

// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_CVTBUF_CC__)
#define __SRCCXX_CVTBUF_CC__ 1

#if !defined(_CXXRT_CVTBUF_H__)
#  include <cxxrt/cvtbuf.h>
#endif
#if !defined(_CXXRT_CODECVT_H__)
#  include <cxxrt/codecvt.h>
#endif

#if !defined(_CXXRT_CONVERT_H__)
#  include <cxxrt/convert.h>
#endif
#if !defined(__CASSERT__)
#  include <cassert>
#endif
#include <iostream>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::_CS_cvtbuf():
  _CS_m_er(),
  _CS_m_cvt(0),
  _CS_m_buffered(true)
{
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::~_CS_cvtbuf()
{
  if (_CS_m_cvt != 0)
    {
      sync();
      _CS_finish(); 
    }
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
void
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::_CS_init()
{
  _CS_init(this->getloc());
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
void
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::_CS_init(locale const& loc)
{
  typedef codecvt<_CS_cT, char, typename _CS_traits::state_type> _CS_codecvt;
  _CS_codecvt const& _CS_cvt = use_facet<_CS_codecvt>(loc);
  
  // -dk:DODO
  if (_CS_m_buffered)
    {
      if (_CS_cvt.always_noconv())
        {
          _CS_m_cvt = new _CS_cvt_noconv<_CS_cT, _CS_traits, _CS_ER>(_CS_m_er);
          //-dk:TODO necessary??? setp(_CS_m_cvt->_CS_begin(), _CS_m_cvt->_CS_end() - 1);
          this->setp(_CS_m_cvt->_CS_begin(), _CS_m_cvt->_CS_end() - 1);
          return;
        }
      assert("not yet implemented" == 0);
    }

  _CS_m_cvt = new _CS_cvt_unbuffered<_CS_cT, _CS_traits, _CS_ER>(_CS_m_er, _CS_cvt);
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
bool
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::_CS_finish()
{
  delete _CS_m_cvt;
  _CS_m_cvt = 0;
  return true;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
streamsize
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::showmanyc()
{
  return this->egptr() - this->gptr();
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::int_type
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::underflow()
{
  if (this->gptr() == this->egptr())
    {
      if (_CS_m_cvt == 0)
        return _CS_traits::eof();

      _CS_cT* _CS_buffer = 0;
      streamsize _CS_sz = _CS_m_cvt->_CS_underflow(_CS_buffer);
      if (_CS_sz == 0)
        return traits_type::eof();
      this->setg(_CS_buffer, _CS_buffer, _CS_buffer + _CS_sz);
    }

  return traits_type::to_int_type(*this->gptr());
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::int_type
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::uflow()
{
  if (this->gptr() == this->egptr())
    {
      if (_CS_m_cvt == 0)
        return _CS_traits::eof();

      _CS_cT* _CS_buffer = 0;
      streamsize _CS_sz = _CS_m_cvt->_CS_underflow(_CS_buffer);
      switch (_CS_sz)
        {
        case 0: return traits_type::eof();
        case 1: return traits_type::to_int_type(*_CS_buffer);
        default:
          this->setg(_CS_buffer, _CS_buffer, _CS_buffer + _CS_sz);
          break;
        }
    }

  this->gbump(1);
  return traits_type::to_int_type(*(this->gptr() - 1));
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::int_type
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::overflow(int_type _CS_c)
{
  streamsize _CS_count = this->pptr() - this->pbase() + 1;
  if (!traits_type::eq_int_type(_CS_c, traits_type::eof()))
    if (this->pptr() != 0)
      *this->pptr() = traits_type::to_char_type(_CS_c);
    else
      { // unbuffered
        if (this->_CS_m_cvt == 0)
          return _CS_traits::eof();

        _CS_cT _CS_buffer(traits_type::to_char_type(_CS_c));
        return this->_CS_m_cvt->_CS_overflow(&_CS_buffer, 1) != 1? traits_type::eof(): _CS_c;
      }
  else
    --_CS_count;

  if (this->_CS_m_cvt == 0 || this->_CS_m_cvt->_CS_overflow(this->pbase(), _CS_count) != _CS_count)
    return traits_type::eof();
  this->setp(this->pbase(), this->epptr());
  
  return traits_type::not_eof(_CS_c);
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
int
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::sync()
{
  if (this->pbase() != this->pptr())
    overflow(traits_type::eof());
  return 0;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
basic_streambuf<_CS_cT, _CS_traits>*
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::setbuf(char_type* _CS_buf, streamsize _CS_size)
{
  if (_CS_buf == 0 && _CS_size == 0)
    {
      if (_CS_m_cvt != 0)
        _CS_finish();
      _CS_m_buffered = false;
      _CS_init();
    }
  return this;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
void
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::imbue(locale const& _CS_nloc)
{
  typedef codecvt<_CS_cT, char, typename _CS_traits::state_type> _CS_codecvt;
  if (&use_facet<_CS_codecvt>(this->getloc()) != &use_facet<_CS_codecvt>(_CS_nloc))
    {
      this->_CS_finish();
      this->_CS_init(_CS_nloc);
    }
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::pos_type
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::seekpos(pos_type _CS_sp, ios_base::openmode _CS_which)
{
  return seekoff(_CS_sp, ios_base::beg, _CS_which);
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::pos_type
_CS_cvtbuf<_CS_cT, _CS_traits, _CS_ER>::seekoff(off_type _CS_so, ios_base::seekdir _CS_dir, ios_base::openmode _CS_which)
{
  if (_CS_m_cvt == 0)
    return typename _CS_traits::pos_type(-1);
  else if (_CS_m_buffered)
    {
      pos_type _CS_pos;

      if (this->pbase() != this->pptr())
        overflow(traits_type::eof());
        
      switch (_CS_dir) 
        {
        case ios_base::beg:
          _CS_pos = _CS_so;
          break;
        case ios_base::cur:
          _CS_pos = _CS_m_cvt->_CS_seekoff(0, ios_base::cur, _CS_which);
          if (this->pptr() == this->epptr())
            _CS_pos -= (this->egptr() - this->gptr());
          _CS_pos += _CS_so;
          break;
        case ios_base::end:
          _CS_pos = _CS_m_cvt->_CS_seekoff(0, ios_base::end, _CS_which) + _CS_so;
          break;
        }

      //-dk:TODO make more efficient
      // this discards the buffer, sometimes without any need...
      this->setg(0, 0, 0);
      this->setp(0, 0);
      return _CS_m_cvt->_CS_seekoff(_CS_pos, ios_base::beg, _CS_which);
    }
  else
    return _CS_m_cvt->_CS_seekoff(_CS_so, _CS_dir, _CS_which);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_CVTBUF_CC__ */
