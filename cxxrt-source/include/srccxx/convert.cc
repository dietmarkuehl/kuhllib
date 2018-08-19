// -*-C++-*- srccxx/convert.cc
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
// Title:   Implementation of the converter classes
// Version: $Id: convert.cc,v 1.1.1.1 2002/06/05 01:02:35 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_CONVERT_CC__)
#define __SRCCXX_CONVERT_CC__ 1

#if !defined(__CASSERT__)
#  include <cassert>
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
_CS_converter<_CS_cT, _CS_traits, _CS_ER>::~_CS_converter()
{
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
streamsize
_CS_converter<_CS_cT, _CS_traits, _CS_ER>::_CS_underflow(_CS_cT*&)
{
  return 0;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
streamsize
_CS_converter<_CS_cT, _CS_traits, _CS_ER>::_CS_overflow(_CS_cT const*, streamsize)
{
  return 0;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_traits::pos_type
_CS_converter<_CS_cT, _CS_traits, _CS_ER>::_CS_seekpos(typename _CS_traits::pos_type, ios_base::openmode)
{
  return -1;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_traits::pos_type
_CS_converter<_CS_cT, _CS_traits, _CS_ER>::_CS_seekoff(typename _CS_traits::off_type, ios_base::seekdir, ios_base::openmode)
{
  return -1;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
_CS_cvt_unbuffered<_CS_cT, _CS_traits, _CS_ER>::_CS_cvt_unbuffered(_CS_ER& _CS_file,
                                                                   _CS_codecvt const& _CS_cvt):
  _CS_m_file(_CS_file),
  _CS_m_codecvt(_CS_cvt),
  _CS_m_state(),
  _CS_m_encoding(_CS_cvt.encoding() > 0? _CS_cvt.encoding(): 1)
{
  if (_CS_m_encoding > 4)
    _CS_m_encoding = 4;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
_CS_cvt_unbuffered<_CS_cT, _CS_traits, _CS_ER>::~_CS_cvt_unbuffered()
{
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
streamsize
_CS_cvt_unbuffered<_CS_cT, _CS_traits, _CS_ER>::_CS_underflow(_CS_cT*& _CS_buffer)
{
  char        _CS_input[1];
  char const* _CS_in_end = _CS_input;
  _CS_cT*     _CS_out_end = _CS_m_buffer;

  do
    {
      if (_CS_m_file._CS_read(_CS_input, _CS_m_encoding) != _CS_m_encoding)
        return 0;

      switch (_CS_m_codecvt.in(_CS_m_state,
                               _CS_input, _CS_input + _CS_m_encoding, _CS_in_end,
                               _CS_m_buffer, _CS_m_buffer + 1, _CS_out_end))
        {
        case codecvt_base::noconv:
          *_CS_out_end++ = _CS_input[0];
          break;
        case codecvt_base::ok:
          break;
        case codecvt_base::error:
          return 0;
        case codecvt_base::partial:
          break;
        }
    }
  while (_CS_out_end != _CS_m_buffer + 1);

  _CS_buffer = _CS_m_buffer;
  return 1;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
streamsize
_CS_cvt_unbuffered<_CS_cT, _CS_traits, _CS_ER>::_CS_overflow(_CS_cT const* _CS_in, streamsize _CS_size)
{
  streamsize _CS_rc = 0;
  char  _CS_out[1024]; // random value
  char* _CS_out_end = _CS_out;

  _CS_cT const* _CS_in_next = _CS_in;
  _CS_cT const* _CS_in_end  = _CS_in + _CS_size;
  
  while (_CS_in_next != _CS_in_end)
    {
      switch (_CS_m_codecvt.out(_CS_m_state,
                                _CS_in_next, _CS_in_end, _CS_in_next,
                                _CS_out, _CS_out + 1024, _CS_out_end))
        {
        case codecvt_base::error: return 0;
        case codecvt_base::noconv: return _CS_m_file._CS_write(_CS_in, _CS_size);
        case codecvt_base::ok: // fall through
        case codecvt_base::partial:
          if (_CS_m_file._CS_write(_CS_out, _CS_out_end - _CS_out) != streamsize(_CS_out_end - _CS_out))
            return _CS_rc;
          _CS_rc = _CS_in_next - _CS_in;
          break;
        }
    }

  return _CS_rc;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_traits::pos_type
_CS_cvt_unbuffered<_CS_cT, _CS_traits, _CS_ER>::_CS_seekpos(typename _CS_traits::pos_type, ios_base::openmode)
{
  return -1;
}

template <class _CS_cT, class _CS_traits, class _CS_ER>
typename _CS_traits::pos_type
_CS_cvt_unbuffered<_CS_cT, _CS_traits, _CS_ER>::_CS_seekoff(typename _CS_traits::off_type _CS_off,
                                                            ios_base::seekdir _CS_dir,
                                                            ios_base::openmode _CS_which)
{
  return _CS_m_file._CS_seek(_CS_off, _CS_dir);
}

// --------------------------------------------------------------------------
template <class _CS_cT, class _CS_traits, class _CS_ER>
_CS_cvt_noconv<_CS_cT, _CS_traits, _CS_ER>::_CS_cvt_noconv(_CS_ER& _CS_er)
{
  assert("no conversion requested for wide characters!" != 0);
}

template <class _CS_traits, class _CS_ER>
_CS_cvt_noconv<char, _CS_traits, _CS_ER>::_CS_cvt_noconv(_CS_ER& _CS_er):
  _CS_m_file(_CS_er),
  _CS_m_buffer(new char[_CS_m_file._CS_bufsiz()])
{
}

template <class _CS_traits, class _CS_ER>
streamsize
_CS_cvt_noconv<char, _CS_traits, _CS_ER>::_CS_underflow(char*& _CS_buf)
{
  _CS_buf = _CS_m_buffer;
  return _CS_m_file._CS_read(_CS_buf, _CS_m_file._CS_bufsiz());
}

template <class _CS_traits, class _CS_ER>
streamsize
_CS_cvt_noconv<char, _CS_traits, _CS_ER>::_CS_overflow(char const* _CS_buf, streamsize _CS_n)
{
  return _CS_m_file._CS_write(_CS_buf, _CS_n);
}

template <class _CS_traits, class _CS_ER>
typename _CS_traits::pos_type
_CS_cvt_noconv<char, _CS_traits, _CS_ER>::_CS_seekpos(typename _CS_traits::pos_type _CS_pos, ios_base::openmode)
{
  return _CS_m_file._CS_seek(_CS_pos, ios_base::beg);
}

template <class _CS_traits, class _CS_ER>
typename _CS_traits::pos_type
_CS_cvt_noconv<char, _CS_traits, _CS_ER>::_CS_seekoff(typename _CS_traits::off_type _CS_off,
                                                        ios_base::seekdir _CS_dir,
                                                        ios_base::openmode)
{
  return _CS_m_file._CS_seek(_CS_off, _CS_dir);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_CONVERT_CC__ */
