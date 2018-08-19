// -*-C++-*- cxxrt/fstream.h
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
// Title:   Declarations for the file stream classes
// Version: $Id: fstream.h,v 1.1.1.1 2002/06/05 01:01:54 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_FSTREAM_H__)
#define _CXXRT_FSTREAM_H__ 1

#if !defined(_CXXRT_FSTR_FWD_H__)
#  include <cxxrt/fstr_fwd.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif
#if !defined(_CXXRT_IOSTREAM_H__)
#  include <cxxrt/iostream.h>
#endif
#if !defined(_CXXRT_CVTBUF_H__)
#  include <cxxrt/cvtbuf.h>
#endif
#if !defined(_CXXRT_SYSFILE_H__)
#  include <cxxrt/sysfile.h>
#endif
#if !defined(__CSTDDEF__)
#  include <cstddef>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
class basic_filebuf: public _CS_cvtbuf<_CS_charT, _CS_traits, _CS_sysfile>
{
public:
  typedef _CS_charT                     char_type;
  typedef _CS_traits                    traits_type;
  typedef typename _CS_traits::int_type int_type;
  typedef typename _CS_traits::pos_type pos_type;
  typedef typename _CS_traits::off_type off_type;

  basic_filebuf();
  ~basic_filebuf();

  bool is_open() const;
  basic_filebuf<_CS_charT, _CS_traits>* open(char const*, ios_base::openmode);
  basic_filebuf<_CS_charT, _CS_traits>* open(char const* _CS_n, ios_base::open_mode _CS_m)
    {
      _CXXRT_DEPRECATED(open_mode__is__deprecated,use__openmode__instead)
      return open(_CS_n, ios_base::openmode(_CS_m));
    }
  basic_filebuf<_CS_charT, _CS_traits>* close();
};

// --------------------------------------------------------------------------

typedef basic_filebuf<char, char_traits<char> > filebuf;
typedef basic_filebuf<wchar_t, char_traits<wchar_t> > wfilebuf;

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
struct _CS_basic_fstream_pbase
{
  _CS_basic_fstream_pbase();
  _CS_basic_fstream_pbase(_CS_basic_fstream_pbase const&): _CS_m_buf() {}
  virtual ~_CS_basic_fstream_pbase();
  mutable basic_filebuf<_CS_charT, _CS_traits> _CS_m_buf;
};

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
class basic_ifstream: private virtual _CS_basic_fstream_pbase<_CS_charT, _CS_traits>,
                      public basic_istream<_CS_charT, _CS_traits>
{
public:
  basic_ifstream();
  basic_ifstream(char const*, ios_base::openmode = ios_base::in);
  
  basic_filebuf<_CS_charT, _CS_traits>* rdbuf() const;

  bool is_open() const;
  void open(char const*, ios_base::openmode = ios_base::in);
  void open(char const* _CS_n, ios_base::open_mode _CS_m)
    {
      _CXXRT_DEPRECATED(open_mode__is__deprecated,use__openmode__instead)
      open(_CS_n, ios_base::openmode(_CS_m));
    }
  void close();

private:
  basic_ifstream(basic_ifstream const&); // deliberatly not accessible
  void operator= (basic_ifstream const&); // deliberatly not accessible
};

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
class basic_ofstream: private virtual _CS_basic_fstream_pbase<_CS_charT, _CS_traits>,
                      public basic_ostream<_CS_charT, _CS_traits>
{
public:
  basic_ofstream();
  basic_ofstream(char const*, ios_base::openmode = ios_base::out);
  
  basic_filebuf<_CS_charT, _CS_traits>* rdbuf() const;

  bool is_open() const;
  void open(char const*, ios_base::openmode = ios_base::out);
  void open(char const* _CS_n, ios_base::open_mode _CS_m)
    {
      _CXXRT_DEPRECATED(open_mode__is__deprecated,use__openmode__instead)
      open(_CS_n, ios_base::openmode(_CS_m));
    }
  void close();

private:
  basic_ofstream(basic_ofstream const&); // deliberatly not accessible
  void operator= (basic_ofstream const&); // deliberatly not accessible
};

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
class basic_fstream: private virtual _CS_basic_fstream_pbase<_CS_charT, _CS_traits>,
                     public basic_iostream<_CS_charT, _CS_traits>
{
public:
  typedef typename basic_ios<_CS_charT, _CS_traits>::char_type char_type;

  basic_fstream();
  basic_fstream(char const*, ios_base::openmode = ios_base::in | ios_base::out);
  
  basic_filebuf<_CS_charT, _CS_traits>* rdbuf() const;

  bool is_open() const;
  void open(char const*, ios_base::openmode = ios_base::in | ios_base::out);
  void close();

private:
  basic_fstream(basic_fstream const&);   // deliberatly not accessible
  void operator= (basic_fstream const&); // deliberatly not accessible
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_FSTREAM_CC__)
#  include <srccxx/fstream.cc>
#endif

#endif /* _CXXRT_FSTREAM_H__ */
