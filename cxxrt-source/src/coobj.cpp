// -*-C++-*- coobj.cc
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
// Title:   Definition of the standard output stream objects 
// Version: $Id: coobj.cpp,v 1.1.1.1 2002/06/05 01:00:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ctraits.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <streambuf>
#include <cassert>
#include <cstdlib>

extern "C" int write(int, char const*, int);

// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_STANDALONE)

_CXXRT_NAMESPACE_BEGIN
  extern streamsize _CS_std_bufsize;
_CXXRT_NAMESPACE_END
using namespace std;

namespace {

  class cstdiobuf: public _CXXRT_STD basic_streambuf<char, _CXXRT_STD char_traits<char> >
  {
  public:
    cstdiobuf(FILE *f);

  protected:
    int        sync();
    int_type   overflow(int_type c);

  private:
    cstdiobuf(cstdiobuf const&);
    void operator= (cstdiobuf const&);
    bool flush();

    FILE* m_file;
    char                     m_buffer[65536];
  };

  inline bool cstdiobuf::flush()
  {
    _CXXRT_STD size_t sz = pptr() - pbase();
    if (sz > 0)
      {
	setp(m_buffer, static_cast<char*>(m_buffer) + _CXXRT_STD _CS_std_bufsize - 1);
	return  fwrite(pbase(), 1, sz, m_file) == sz;
      }

    return true;
  }

  cstdiobuf::cstdiobuf(FILE *f):
    m_file(f)
  {
    setp(m_buffer, m_buffer);
  }

  int cstdiobuf::sync()
  {
    if (!flush())
      return -1;
    return fflush(m_file);
  }

  _CXXRT_STD ostream::int_type cstdiobuf::overflow(_CXXRT_STD ostream::int_type c)
  {
    if (!traits_type::eq_int_type(c, traits_type::eof()))
      {
        *pptr() = _CXXRT_STD char_traits<char>::to_char_type(c);
        pbump(1); // ++sz;
      }

    return flush()? traits_type::not_eof(c): traits_type::eof();
  }

// -------------------------------------------------------------------------- 

  class flush_helper
  {
  public:
    flush_helper(_CXXRT_STD basic_streambuf<char>* sbuf): m_sbuf(sbuf) {}
    ~flush_helper() { m_sbuf->pubsync(); }

    operator _CXXRT_STD basic_streambuf<char>* () { return m_sbuf; }

  private:
    flush_helper(flush_helper const&);
    void operator=(flush_helper const&);

    _CXXRT_STD basic_streambuf<char>* m_sbuf;
  };

// -------------------------------------------------------------------------- 

}

_CXXRT_NAMESPACE_BEGIN

// using _CS_swamp::_impure_ptr;
flush_helper cout_helper(new cstdiobuf(stdout));
flush_helper cerr_helper(new cstdiobuf(stderr));
flush_helper clog_helper(new cstdiobuf(stderr));

basic_ostream<char, char_traits<char> > cout(cout_helper, false);
basic_ostream<char, char_traits<char> > cerr(cerr_helper, true);
basic_ostream<char, char_traits<char> > clog(clog_helper, false);

namespace {
  ios_base::Init _CS_flush_streams;
}

_CXXRT_NAMESPACE_END

#else

#include <unistd.h>

namespace
{
  class stdiobuf:
    public _CXXRT_STD streambuf
  {
  public:
    stdiobuf(int fd): m_fd(fd) { setp(m_buffer, m_buffer + s_size - 1); }
  private:
    int_type overflow(int_type c)
    {
      if (!traits_type::eq_int_type(c, traits_type::eof()))
	{
	  *pptr() = traits_type::to_char_type(c);
	  pbump(1);
	}

      if (_Cxxrt_write(m_fd, m_buffer, pptr() - pbase()) != pptr() - pbase())
	return traits_type::eof();
      setp(m_buffer, m_buffer + s_size - 1);
      return traits_type::not_eof(c);
    }

    int sync()
    {
      return traits_type::eq_int_type(traits_type::eof(), overflow(traits_type::eof()));
    }

    int m_fd;
    enum { s_size = 256 };
    char m_buffer[s_size];
  };

  stdiobuf _Cxxrt_coutbuf(1);
  stdiobuf _Cxxrt_cerrbuf(2);
}

void
_Cxxrt_iostream_flush()
{
  _Cxxrt_coutbuf.pubsync();
}

#if 1
_CXXRT_NAMESPACE_BEGIN

ostream cout(&_Cxxrt_coutbuf);
ostream cerr(&_Cxxrt_cerrbuf, true);
ostream clog(&_Cxxrt_cerrbuf);

_CXXRT_NAMESPACE_END
#endif

#endif
