// -*-C++-*- srccxx/ostream.cc
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
// Title:   Definition of the basic_ostream member functions
// Version: $Id: ostream.cc,v 1.4 2006/04/07 07:07:13 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_OSTREAM_CC__)
#define __SRCCXX_OSTREAM_CC__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_OSTREAM_H__)
#  include <cxxrt/ostream.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif
#if !defined(_CXXRT_NUM_PUT_H__)
#  include <cxxrt/num_put.h>
#endif
#if !defined(_CXXRT_SBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif
#include <iostream>

#if !defined(_CXXRT_TRACE)
#  define _CXXRT_TRACE(str)
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>::basic_ostream(basic_ostream const&):
  basic_ios<_CS_cT, _CS_traits>(0)
{
  _CXXRT_TRACE("std::basic_ostream::basic_ostream(basic_ostream const&)");
}

template <class _CS_cT, class _CS_traits>
void
basic_ostream<_CS_cT, _CS_traits>::operator= (basic_ostream const&)
{
  _CXXRT_TRACE("std::basic_ostream::operator=(basic_ostream const&)");
}
#endif

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>::
basic_ostream(basic_streambuf<_CS_cT, _CS_traits> *_CS_sbuf):
  basic_ios<_CS_cT, _CS_traits>(_CS_sbuf)
{
  _CXXRT_TRACE("std::basic_ostream::basic_ostream(basic_streambuf*)");
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>::
basic_ostream(basic_streambuf<_CS_cT, _CS_traits> *_CS_sbuf, bool _CS_nunitbuf):
  basic_ios<_CS_cT, _CS_traits>(_CS_sbuf)
{
  _CXXRT_TRACE("std::basic_ostream::basic_ostream(basic_streambuf*, bool)");
  if (_CS_nunitbuf)
    this->setf(ios_base::unitbuf);
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>::~basic_ostream()
{
  _CXXRT_TRACE("std::basic_ostream::~basic_ostream()");
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::flush()
{
  _CXXRT_TRACE("std::basic_ostream::flush()");
  if (this->rdbuf() != 0 && this->rdbuf()->pubsync() == -1)
    this->setstate(this->badbit);
  return *this;
}

// --------------------------------------------------------------------------

#if !defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
template <class _CS_T>
inline void
basic_ostream<_CS_cT, _CS_traits>::_CS_print(_CS_T _CS_t)
{
  _CXXRT_TRACE("std::basic_ostream::_CS_print(_CS_T)");
  if (!this->_CS_need_osentry())
    {
      try
        {
          if (use_facet<num_put<_CS_cT> >(this->getloc()).put(*this, *this, this->fill(), _CS_t).failed())
            this->setstate(this->badbit);
        }
      catch (...)
        {
          this->_CS_process_exception();
        }
    }
  else
    {
      typename basic_ostream<_CS_cT, _CS_traits>::sentry _CS_kerberos(*this);
      if (_CS_kerberos)
        try
          {
            if (use_facet<num_put<_CS_cT> >(this->getloc()).put(*this, *this, this->fill(), _CS_t).failed())
              this->setstate(this->badbit);
          }
        catch (...)
          {
            this->_CS_process_exception();
          }
    }
}
#endif

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (bool _CS_b)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (bool)");
  _CS_print(static_cast<bool>(_CS_b));
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (short _CS_i)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (short)");
  _CS_print(static_cast<long>(static_cast<unsigned short>(_CS_i)));
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (unsigned short _CS_i)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (unsigned short)");
  _CS_print(static_cast<unsigned long>(_CS_i));
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (int _CS_i)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (int)");
  _CS_print(static_cast<long>(static_cast<unsigned int>(_CS_i)));
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (unsigned int _CS_i)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (unsigned int)");
  _CS_print(static_cast<unsigned long>(_CS_i));
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (long _CS_i)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (long)");
  _CS_print(_CS_i);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (unsigned long _CS_i)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (unsigned long)");
  _CS_print(_CS_i);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (float _CS_d)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (float)");
  _CS_print(_CS_d);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (double _CS_d)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (double)");
  _CS_print(_CS_d);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (long double _CS_d)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (long double)");
  _CS_print(_CS_d);
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (void const* _CS_vp)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (void const*)");
  _CS_print(static_cast<void const*>(_CS_vp));
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
inline basic_ostream<_CS_cT, _CS_traits>&
_CS_setstate(basic_ostream<_CS_cT, _CS_traits>& _CS_strm, ios_base::iostate _CS_state, char const* _CS_message)
{
  _CXXRT_TRACE("std::basic_ostream::_CS_setstate(basic_ostream, iostate, char const*)");
  try
    {
      _CS_strm.setstate(_CS_state);
    }
  catch (ios_base::failure const&)
    {
      throw ios_base::failure(_CS_message);
    }
  return _CS_strm;
}

template <class _CS_cT, class _CS_traits>
inline typename _CS_traits::int_type
_CS_osgetc(basic_streambuf<_CS_cT, _CS_traits>* _CS_sb, bool& _CS_extracted)
{
  _CXXRT_TRACE("std::_CS_osgetc(basic_streambuf*, bool&)");
  try { return _CS_sb->sgetc(); }
  catch (...) { _CS_extracted = true; throw; }
}

template <class _CS_cT, class _CS_traits>
inline _CS_cT
_CS_osnextc(basic_streambuf<_CS_cT, _CS_traits>* _CS_sb, bool& _CS_extracted)
{
  _CXXRT_TRACE("std::_CS_osnextc(basic_streambuf*, bool&)");
  try { return _CS_sb->sbumpc(); }
  catch (...) { _CS_extracted = true; throw; }
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
basic_ostream<_CS_cT, _CS_traits>::operator<< (basic_streambuf<_CS_cT, _CS_traits>* _CS_from)
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (basic_streambuf*)");
  sentry _CS_kerberos(*this);
  if (_CS_kerberos)
    {
      if (_CS_from == 0)
        return _CS_setstate(*this, ios_base::badbit | ios_base::failbit,
                            "stream buffer argument to operator<<() is null");
      bool _CS_extracted = false;
        
      try
        {
          basic_streambuf<_CS_cT, _CS_traits>* _CS_to = this->rdbuf();
          typename _CS_traits::int_type _CS_c = _CS_from->sgetc();

          // special processing of the first character
          if (_CS_traits::eq_int_type(_CS_c, _CS_traits::eof())
              || _CS_traits::eq_int_type(_CS_to->sputc(_CS_c), _CS_traits::eof()))
            return _CS_setstate(*this, this->failbit,
                                "no character could be extracted from stream buffer argument to operator()");
          
          for (_CS_c = _CS_from->snextc();
               !_CS_traits::eq_int_type(_CS_c, _CS_traits::eof());
               _CS_c = _CS_from->sgetc())
            {
              streamsize _CS_n = _CS_from->_CS_end() - _CS_from->_CS_begin();
              if (_CS_n > 1)
                {
                  streamsize _CS_m = _CS_to->sputn(_CS_from->_CS_begin(), _CS_n);
                  _CS_from->_CS_commit(_CS_from->_CS_begin() + _CS_m);
                  if (_CS_m != _CS_n)
                    break;
                }
              else
                if (_CS_traits::eq_int_type(_CS_to->sputc(_CS_traits::to_char_type(_CS_c)), _CS_traits::eof()))
                  break;
            }
        }
      catch (...)
        {
          this->_CS_process_exception();
        }
    }
  else
    this->setstate(this->failbit);
  
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_tr>
basic_ostream<_CS_cT, _CS_tr>&
basic_ostream<_CS_cT, _CS_tr>::operator<< (
  ios_base& (*_CS_pf)(ios_base&))
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (ios_base& (*)(ios_base&))");
  try
    {
      _CS_pf(*this);
    }
  catch (...)
    {
      // setting badbit might throw an exception, but the wrong one...
      try { this->setstate(this->badbit); } catch (...) {}
      if (this->exceptions() & this->badbit)
        throw;
    }

  return *this;
}

template <class _CS_cT, class _CS_tr>
basic_ostream<_CS_cT, _CS_tr>&
basic_ostream<_CS_cT, _CS_tr>::operator<< (
  basic_ios<_CS_cT, _CS_tr>& (*_CS_pf)(basic_ios<_CS_cT, _CS_tr>&))
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (basic_ios& (*)(basic_ios&))");
  try
    {
      _CS_pf(*this);
    }
  catch (...)
    {
      // setting badbit might throw an exception, but the wrong one...
      try { this->setstate(this->badbit); } catch (...) {}
      if (this->exceptions() & this->badbit)
        throw;
    }

  return *this;
}

template <class _CS_cT, class _CS_tr>
basic_ostream<_CS_cT, _CS_tr>&
basic_ostream<_CS_cT, _CS_tr>::operator<< (
   basic_ostream<_CS_cT, _CS_tr>& (*_CS_pf)(basic_ostream<_CS_cT, _CS_tr>&))
{
  _CXXRT_TRACE("std::basic_ostream::operator<< (basic_ostream& (*)(basic_ostream&))");
  try
    {
      _CS_pf(*this);
    }
  catch (...)
    {
      // setting badbit might throw an exception, but the wrong one...
      try { this->setstate(this->badbit); } catch (...) {}
      if (this->exceptions() & this->badbit)
        throw;
    }

  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_tr>
basic_ostream<_CS_cT, _CS_tr>&
basic_ostream<_CS_cT, _CS_tr>::put(basic_ostream<_CS_cT, _CS_tr>::char_type _CS_c)
{
  _CXXRT_TRACE("std::basic_ostream::put(typename basic_ostream::char_type)");
  if (!this->_CS_need_osentry())
  {
      try
      {
	  if (_CS_tr::eq_int_type(this->rdbuf()->sputc(_CS_c), _CS_tr::eof()))
	      this->setstate(this->badbit);
      }
      catch (...)
      {
	  this->_CS_process_exception();
      }
  }
  else
  {
      sentry _CS_kerberos(*this);
      if (_CS_kerberos)
	  try
	  {
	      if (_CS_tr::eq_int_type(this->rdbuf()->sputc(_CS_c), _CS_tr::eof()))
		  this->setstate(this->badbit);
	  }
	  catch (...)
	  {
	      this->_CS_process_exception();
	  }
      else
	  this->setstate(this->badbit);
  }
  return *this;
}

template <class _CS_cT, class _CS_tr>
basic_ostream<_CS_cT, _CS_tr>&
basic_ostream<_CS_cT, _CS_tr>::write(typename basic_ios<_CS_cT, _CS_tr>::char_type const* _CS_str,
                                     streamsize _CS_n)
{
  _CXXRT_TRACE("std::basic_ostream::write(typename basic_ostream::char_type const*, streamsize)");
  sentry _CS_kerberos(*this);
  if (_CS_kerberos)
    try
      {
        if (this->rdbuf()->sputn(_CS_str, _CS_n) != _CS_n)
          this->setstate(this->badbit);
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  else
    this->setstate(this->badbit);
  return *this;
}

template <class _CS_cT, class _CS_tr>
typename basic_ostream<_CS_cT, _CS_tr>::pos_type
basic_ostream<_CS_cT, _CS_tr>::tellp()
{
  _CXXRT_TRACE("std::basic_ostream::tellp()");
  return this->fail()? pos_type(-1): this->rdbuf()->pubseekoff(off_type(0), this->cur, this->out);
}

template <class _CS_cT, class _CS_tr>
basic_ostream<_CS_cT, _CS_tr>&
basic_ostream<_CS_cT, _CS_tr>::seekp(pos_type _CS_pos)
{
  _CXXRT_TRACE("std::basic_ostream::seekp(pos_type)");
  if (!this->fail())
    this->rdbuf()->pubseekpos(_CS_pos, this->out);
  return *this;
}

template <class _CS_cT, class _CS_tr>
basic_ostream<_CS_cT, _CS_tr>&
basic_ostream<_CS_cT, _CS_tr>::seekp(off_type _CS_off, ios_base::seekdir _CS_dir)
{
  _CXXRT_TRACE("std::basic_ostream::seekp(off_type, ios_base::seekdir)");
  if (!this->fail())
    this->rdbuf()->pubseekoff(_CS_off, _CS_dir, this->out);
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
endl(basic_ostream<_CS_cT, _CS_traits> &_CS_out)
{
  _CXXRT_TRACE("std::endl(basic_ostream&)");
  _CS_out.rdbuf()->sputc(_CS_out._CS_newline());
  _CS_out.rdbuf()->pubsync();
  return _CS_out;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
ends(basic_ostream<_CS_cT, _CS_traits> &_CS_out)
{
  _CXXRT_TRACE("std::ends(basic_ostream&)");
  _CS_out.rdbuf()->sputc(_CS_cT());
  return _CS_out;
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
flush(basic_ostream<_CS_cT, _CS_traits> &_CS_out)
{
  _CXXRT_TRACE("std::flush(basic_ostream&)");
  _CS_out.flush();
  return _CS_out;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
inline basic_ostream<_CS_cT, _CS_traits>&
_CS_print_char(basic_ostream<_CS_cT, _CS_traits>& _CS_out, _CS_cT _CS_c)
{
  _CXXRT_TRACE("std::_CS_print_char(basic_ostream&, _CS_cT)");
  if (_CS_out.width() == 0)
    _CS_out.rdbuf()->sputc(_CS_c);
  else
    {
      basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf = _CS_out.rdbuf();
      switch (_CS_out.flags() & ios_base::adjustfield)
        {
        case ios_base::left:
          {
            _CS_sbuf->sputc(_CS_c);
            for (streamsize _CS_n = _CS_out.width(); _CS_n-- > 1; )
              _CS_sbuf->sputc(_CS_out.fill());
          }
          break;
        case ios_base::internal:
        case ios_base::right:
        default:
          {
            for (streamsize _CS_n = _CS_out.width(); _CS_n-- > 1; )
              _CS_sbuf->sputc(_CS_out.fill());
            _CS_sbuf->sputc(_CS_c);
          }
          break;
        }
      _CS_out._CS_width(0);
    }
  return _CS_out;
}

#if !defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>& _CS_out, char _CS_c)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream&, char)");
  typename basic_ostream<_CS_cT, _CS_traits>::sentry _Kerberos(_CS_out);
  if (_Kerberos)
    try
      {
        return _CS_print_char(_CS_out, _CS_out.widen(_CS_c));
      }
    catch (...)
      {
        _CS_out._CS_process_exception();
      }

  return _CS_out;
}
#endif

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>& _CS_out, _CS_cT _CS_c)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream&, _CS_cT)");
  typename basic_ostream<_CS_cT, _CS_traits>::sentry _Kerberos(_CS_out);
  if (_Kerberos)
    try
      {
        return _CS_print_char(_CS_out, _CS_c);
      }
    catch (...)
      {
        _CS_out._CS_process_exception();
      }

  return _CS_out;
}

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>& _CS_out, char _CS_c)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream<char>&, char)");
  typename basic_ostream<char, _CS_traits>::sentry _Kerberos(_CS_out);
  if (_Kerberos)
    try
      {
        return _CS_print_char(_CS_out, _CS_c);
      }
    catch (...)
      {
        _CS_out._CS_process_exception();
      }

  return _CS_out;
}

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>& _CS_out, signed char _CS_c)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream<char>&, signed char)");
  typename basic_ostream<char, _CS_traits>::sentry _Kerberos(_CS_out);
  if (_Kerberos)
    try
      {
        return _CS_print_char(_CS_out, static_cast<char>(_CS_c));
      }
    catch (...)
      {
        _CS_out._CS_process_exception();
      }

  return _CS_out;
}

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>& _CS_out, unsigned char _CS_c)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream<char>&, unsigned char)");
  typename basic_ostream<char, _CS_traits>::sentry _Kerberos(_CS_out);
  if (_Kerberos)
    try
      {
        return _CS_print_char(_CS_out, static_cast<char>(_CS_c));
      }
    catch (...)
      {
        _CS_out._CS_process_exception();
      }

  return _CS_out;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
struct _CS_null
{
  static _CS_cT const* _CS_str();
};

template <class _CS_cT, class _CS_traits>
inline basic_ostream<_CS_cT, _CS_traits>&
_CS_print_string_intern(basic_ostream<_CS_cT, _CS_traits>& _CS_out, _CS_cT const* _CS_str)
{
  _CXXRT_TRACE("std::_CS_print_string_intern(basic_ostream&, _CS_cT const*)");
  try
    {
      if (_CS_str == 0)
        _CS_str = _CS_null<_CS_cT>::_CS_str();
      streamsize _CS_len = _CS_traits::length(_CS_str);
      basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf = _CS_out.rdbuf();
      streamsize _CS_width = _CS_out.width(0);
      if (_CS_width <= _CS_len)
        {
          if (_CS_sbuf->sputn(_CS_str, _CS_len) != _CS_len)
            _CS_out.setstate(ios_base::failbit);
        }
      else
        {
          switch (_CS_out.flags() & ios_base::adjustfield)
            {
            case ios_base::left:
              {
                for (streamsize _CS_i = 0; _CS_i < _CS_len; ++_CS_i)
                  _CS_sbuf->sputc(_CS_str[_CS_i]);
                for (streamsize _CS_n = _CS_width; _CS_n-- > _CS_len; )
                  _CS_sbuf->sputc(_CS_out.fill());
              }
              break;
            case ios_base::internal:
            case ios_base::right:
            default:
              {
                for (streamsize _CS_n = _CS_width; _CS_n-- > _CS_len; )
                  _CS_sbuf->sputc(_CS_out.fill());
                for (streamsize _CS_i = 0; _CS_i < _CS_len; ++_CS_i)
                  _CS_sbuf->sputc(_CS_str[_CS_i]);
              }
              break;
            }
        }
    }
  catch (...)
    {
      _CS_out._CS_process_exception();
    }

  return _CS_out;
}

template <class _CS_cT, class _CS_traits>
inline basic_ostream<_CS_cT, _CS_traits>&
_CS_print_string(basic_ostream<_CS_cT, _CS_traits>& _CS_out, _CS_cT const* _CS_str)
{
  _CXXRT_TRACE("std::_CS_print_string(basic_ostream&, _CS_cT const*)");
  if (!_CS_out._CS_need_osentry())
    _CS_print_string_intern(_CS_out, _CS_str);
  else
    {
      typename basic_ostream<_CS_cT, _CS_traits>::sentry _CS_kerberos(_CS_out);
      if (_CS_kerberos)
        _CS_print_string_intern(_CS_out, _CS_str);
    }
  
  return _CS_out;
}

#if !defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>&_CS_out, _CS_cT const* _CS_str)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream&, _CS_cT const*)");
  return _CS_print_string(_CS_out, _CS_str);
}

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>& _CS_out, char const* _CS_str)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream&, char const*)");
  typename basic_ostream<_CS_cT, _CS_traits>::sentry _Kerberos(_CS_out);
  if (_Kerberos)
    try
      {
        streamsize _CS_len = char_traits<char>::length(_CS_str);
        basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf = _CS_out.rdbuf();
        if (_CS_out.width() <= _CS_len)
          for (streamsize _CS_i = 0; _CS_i < _CS_len; ++_CS_i)
            _CS_sbuf->sputc(_CS_out.widen(_CS_str[_CS_i]));
        else
          {
            switch (_CS_out.flags() & ios_base::adjustfield)
              {
              case ios_base::left:
                {
                  for (streamsize _CS_i = 0; _CS_i < _CS_len; ++_CS_i)
                    _CS_sbuf->sputc(_CS_out.widen(_CS_str[_CS_i]));
                  for (streamsize _CS_n = _CS_out.width(); _CS_n-- > _CS_len; )
                    _CS_sbuf->sputc(_CS_out.fill());
                }
                break;
              case ios_base::internal:
              case ios_base::right:
                {
                  for (streamsize _CS_n = _CS_out.width(); _CS_n-- > _CS_len; )
                    _CS_sbuf->sputc(_CS_out.fill());
                }
              default:
                {
                  for (streamsize _CS_i = 0; _CS_i < _CS_len; ++_CS_i)
                    _CS_sbuf->sputc(_CS_out.widen(_CS_str[_CS_i]));
                }
                break;
              }
          }

        _CS_out.width(0);
      }
    catch (...)
      {
        _CS_out._CS_process_exception();
      }
  return _CS_out;
}
#endif

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>& _CS_out, char const* _CS_str)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream<char>&, char const*)");
  return _CS_print_string(_CS_out, _CS_str);
}

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>& _CS_out, signed char const* _CS_str)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream<char>&, signed char const*)");
  return _CS_print_string(_CS_out, reinterpret_cast<char const*>(_CS_str));
}

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>& _CS_out, unsigned char const* _CS_str)
{
  _CXXRT_TRACE("std::operator<< (basic_ostream<char>&, unsigned char const*)");
  return _CS_print_string(_CS_out, reinterpret_cast<char const*>(_CS_str));
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_OSTREAM_CC__ */
