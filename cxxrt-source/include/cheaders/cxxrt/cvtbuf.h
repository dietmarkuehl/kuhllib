// -*-C++-*- cxxrt/cvtbuf.h
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
// Title:   A streambuf converting between charT and char (used by filebuf)
// Version: $Id: cvtbuf.h,v 1.1.1.1 2002/06/05 01:01:57 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_CVTBUF_H__)
#define _CXXRT_CVTBUF_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
class _CS_converter;

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_ER>
class _CS_cvtbuf: public basic_streambuf<_CS_cT, _CS_traits>
{
protected:
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::char_type   char_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::traits_type traits_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::int_type    int_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::pos_type    pos_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::off_type    off_type;

  _CS_cvtbuf();
  ~_CS_cvtbuf();

  void _CS_init();
  void _CS_init(locale const&);
  bool _CS_finish();

  streamsize showmanyc();
  int_type   underflow();
  int_type   uflow();
  int_type   overflow(int_type);

  int        sync();
  void       imbue(locale const &);
#if 0
  streamsize xsgetn(char_type*, streamsize);
  streamsize xsputn(char_type const*, streamsize);

  int_type   pbackfail(int_type);
#endif

  pos_type   seekpos(pos_type, ios_base::openmode);
  pos_type   seekoff(off_type, ios_base::seekdir, ios_base::openmode);

  basic_streambuf<_CS_cT, _CS_traits>* setbuf(char_type*, streamsize);

  _CS_ER _CS_m_er;

private:
  _CS_cvtbuf(_CS_cvtbuf const&); // deliberatly not accessible
  void operator=(_CS_cvtbuf const&); // deliberatly not accessible

  _CS_converter<_CS_cT, _CS_traits, _CS_ER>* _CS_m_cvt;

  bool _CS_m_buffered;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_CVTBUF_CC__)
#  include <srccxx/cvtbuf.cc>
#endif

#endif /* _CXXRT_CVTBUF_H__ */
