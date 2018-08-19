// -*-C++-*- cxxrt/convert.h
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
// Title:   Declarations for the conversion classes used by cvtbuf 
// Version: $Id: convert.h,v 1.1.1.1 2002/06/05 01:01:54 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_CONVERT_H__)
#define _CXXRT_CONVERT_H__ 1

#if !defined(_CXXRT_CODECVT_H__)
#  include <cxxrt/codecvt.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
class _CS_converter
{
public:
  virtual ~_CS_converter();

  virtual streamsize _CS_underflow(_CS_cT*&);
  virtual streamsize _CS_overflow(_CS_cT const*, streamsize);

  virtual typename _CS_traits::pos_type _CS_seekpos(typename _CS_traits::pos_type, ios_base::openmode);
  virtual typename _CS_traits::pos_type _CS_seekoff(typename _CS_traits::off_type, ios_base::seekdir, ios_base::openmode);

  virtual _CS_cT* _CS_begin() { return 0; }
  virtual _CS_cT* _CS_end() { return 0; }
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
class _CS_cvt_unbuffered: public _CS_converter<_CS_cT, _CS_traits, _CS_ER>
{
public:
  typedef typename _CS_traits::state_type       _CS_state_type;
  typedef codecvt<_CS_cT, char, _CS_state_type> _CS_codecvt;

  _CS_cvt_unbuffered(_CS_ER& _CS_er, _CS_codecvt const& _CS_cvt);
  ~_CS_cvt_unbuffered();

protected:
  streamsize _CS_underflow(_CS_cT*&);
  streamsize _CS_overflow(_CS_cT const*, streamsize);

  typename _CS_traits::pos_type _CS_seekpos(typename _CS_traits::pos_type, ios_base::openmode);
  typename _CS_traits::pos_type _CS_seekoff(typename _CS_traits::off_type, ios_base::seekdir, ios_base::openmode);

private:
  _CS_ER&                         _CS_m_file;
  _CS_codecvt const&              _CS_m_codecvt;
  typename _CS_traits::state_type _CS_m_state;
  streamsize                      _CS_m_encoding;
  _CS_cT                          _CS_m_buffer[4];
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
class _CS_cvt_noconv: public _CS_converter<_CS_cT, _CS_traits, _CS_ER>
{
public:
  _CS_cvt_noconv(_CS_ER& _CS_er);
};

template <class _CS_traits, class _CS_ER>
class _CS_cvt_noconv<char, _CS_traits, _CS_ER>:
  public _CS_converter<char, _CS_traits, _CS_ER>
{
public:
  _CS_cvt_noconv(_CS_ER& _CS_er);

protected:
  streamsize _CS_underflow(char*&);
  streamsize _CS_overflow(char const*, streamsize);

  typename _CS_traits::pos_type _CS_seekpos(typename _CS_traits::pos_type, ios_base::openmode);
  typename _CS_traits::pos_type _CS_seekoff(typename _CS_traits::off_type, ios_base::seekdir, ios_base::openmode);

  char* _CS_begin() { return _CS_m_buffer; }
  char* _CS_end() { return _CS_m_buffer + _CS_m_file._CS_bufsiz(); }

private:
  _CS_cvt_noconv(_CS_cvt_noconv const&);
  void operator= (_CS_cvt_noconv const&);

  _CS_ER& _CS_m_file;
  char*   _CS_m_buffer;
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
class _CS_cvt_fixed: public _CS_converter<_CS_cT, _CS_traits, _CS_ER>
{
  virtual streamsize _CS_underflow(_CS_cT*&);
  virtual streamsize _CS_overflow(_CS_cT const*, streamsize);
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
class _CS_cvt_variable: public _CS_converter<_CS_cT, _CS_traits, _CS_ER>
{
  virtual streamsize _CS_underflow(_CS_cT*&);
  virtual streamsize _CS_overflow(_CS_cT const*, streamsize);
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if !defined(__SRCCXX_CONVERT_CC__)
#  include <srccxx/convert.cc>
#endif

#endif /* _CXXRT_CONVERT_H__ */
