// -*-C++-*- cxxrt/sstream.h
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
// Title:   Declarations for the string stream classes 
// Version: $Id: sstream.h,v 1.1.1.1 2002/06/05 01:02:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_SSTREAM_H__)
#define _CXXRT_SSTREAM_H__ 1

#if !defined(_CXXRT_SSTR_FWD_H__)
#  include <cxxrt/sstr_fwd.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif
#if !defined(_CXXRT_ALLOC_H__)
#  include <cxxrt/alloc.h>
#endif
#if !defined(_CXXRT_IOSTREAM_H__)
#  include <cxxrt/iostream.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_alloc>
class basic_stringbuf: public basic_streambuf<_CS_cT, _CS_traits>
{
public:
  typedef basic_string<_CS_cT, _CS_traits, _CS_alloc> string_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::char_type char_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::traits_type traits_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::int_type int_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::pos_type pos_type;
  typedef typename basic_streambuf<_CS_cT, _CS_traits>::off_type off_type;

  basic_stringbuf(ios_base::openmode = ios_base::in | ios_base::out);
  basic_stringbuf(basic_string<_CS_cT, _CS_traits, _CS_alloc> const&,
                  ios_base::openmode = ios_base::in | ios_base::out);
  ~basic_stringbuf();

  basic_string<_CS_cT, _CS_traits, _CS_alloc> str() const;
  void str(basic_string<_CS_cT, _CS_traits, _CS_alloc> const&);

protected:
  int_type   underflow();
  int_type   pbackfail(int_type);
  int_type   overflow(int_type);
  basic_streambuf<_CS_cT, _CS_traits>* setbuf(char_type*, streamsize);
  pos_type   seekoff(off_type, ios_base::seekdir, ios_base::openmode);
  pos_type   seekpos(pos_type pos, ios_base::openmode);
  int sync();
  void imbue(locale const&);
  // streamsize xsgetn(char_type*, streamsize);
  // streamsize xsputn(char_type const*, streamsize);

private:
  basic_stringbuf(basic_stringbuf const&); // deliberatly not accessible
  void operator=(basic_stringbuf const&);  // deliberatly not accessible

  ios_base::openmode _CS_m_mode;
  _CS_cT*      _CS_m_beg;
  _CS_cT*      _CS_m_end;
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_Tr, class _CS_Al>
class _CS_basic_sstream_pbase
{
 public:
  typedef basic_string<_CS_cT, _CS_Tr, _CS_Al> _CS_string_type;

  explicit _CS_basic_sstream_pbase(ios_base::openmode);
  _CS_basic_sstream_pbase(_CS_string_type const&, ios_base::openmode);
  virtual ~_CS_basic_sstream_pbase();

  _CS_string_type str() const;
  void                  str(_CS_string_type const&);

 protected:
  mutable basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al> _CS_m_ssbuf;
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_Tr, class _CS_Al>
class basic_istringstream: public virtual _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>,
                           public basic_istream<_CS_cT, _CS_Tr>
{
public:
  typedef typename basic_istream<_CS_cT, _CS_Tr>::char_type char_type;
  typedef typename basic_istream<_CS_cT, _CS_Tr>::int_type int_type;
  typedef typename basic_istream<_CS_cT, _CS_Tr>::pos_type pos_type;
  typedef typename basic_istream<_CS_cT, _CS_Tr>::off_type off_type;
  typedef typename basic_istream<_CS_cT, _CS_Tr>::traits_type traits_type;
  typedef basic_string<_CS_cT, _CS_Tr, _CS_Al> _CS_string_type;

  explicit basic_istringstream(ios_base::openmode = ios_base::in);
  explicit basic_istringstream(_CS_string_type const&, ios_base::openmode = ios_base::in);

  basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>* rdbuf() const { return &this->_CS_m_ssbuf; }

private:
  basic_istringstream(basic_istringstream const&); // deliberately not available
  void operator=(basic_istringstream const&);      // deliberately not available
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_Tr, class _CS_Al>
class basic_ostringstream: public virtual _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>,
                           public basic_ostream<_CS_cT, _CS_Tr>
{
 public:
  typedef typename basic_ostream<_CS_cT, _CS_Tr>::char_type char_type;
  typedef typename basic_ostream<_CS_cT, _CS_Tr>::int_type int_type;
  typedef typename basic_ostream<_CS_cT, _CS_Tr>::pos_type pos_type;
  typedef typename basic_ostream<_CS_cT, _CS_Tr>::off_type off_type;
  typedef basic_string<_CS_cT, _CS_Tr, _CS_Al> _CS_string_type;

  explicit basic_ostringstream(ios_base::openmode = ios_base::out);
  explicit basic_ostringstream(_CS_string_type const&, ios_base::openmode = ios_base::out);

  basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>* rdbuf() const { return &this->_CS_m_ssbuf; }

private:
  basic_ostringstream(basic_ostringstream const&); // deliberately not available
  void operator=(basic_ostringstream const&);      // deliberately not available
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_Tr, class _CS_Al>
class basic_stringstream: public virtual _CS_basic_sstream_pbase<_CS_cT, _CS_Tr, _CS_Al>,
                          public basic_iostream<_CS_cT, _CS_Tr>
{
 public:
  typedef typename basic_ios<_CS_cT, _CS_Tr>::char_type char_type;
  typedef typename basic_ios<_CS_cT, _CS_Tr>::int_type int_type;
  typedef typename basic_ios<_CS_cT, _CS_Tr>::pos_type pos_type;
  typedef typename basic_ios<_CS_cT, _CS_Tr>::off_type off_type;
  typedef basic_string<_CS_cT, _CS_Tr, _CS_Al> _CS_string_type;

  explicit basic_stringstream(ios_base::openmode = ios_base::in | ios_base::out);
  explicit basic_stringstream(_CS_string_type const&, ios_base::openmode = ios_base::in|ios_base::out);

  basic_stringbuf<_CS_cT, _CS_Tr, _CS_Al>* rdbuf() const { return &this->_CS_m_ssbuf; }

private:
  basic_stringstream(basic_stringstream const&); // deliberately not available
  void operator=(basic_stringstream const&);     // deliberately not available
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_SSTREAM_CC__)
#  include <srccxx/sstream.cc>
#endif

#endif /* _CXXRT_SSTREAM_H__ */
