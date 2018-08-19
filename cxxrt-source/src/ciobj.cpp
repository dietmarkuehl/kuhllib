// -*-C++-*- ciobj.cc
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
// Title:   Definition of the standard input stream object
// Version: $Id: ciobj.cpp,v 1.1.1.1 2002/06/05 01:00:22 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ctraits.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <streambuf>
#include <cassert>
#include <cstdlib>

// -------------------------------------------------------------------------- 

//-dk:TODO
#if 0 && !defined(_CXXRT_STANDALONE)

_CXXRT_NAMESPACE_BEGIN
  extern streamsize _CS_std_bufsize;
_CXXRT_NAMESPACE_END

namespace {
  class cstdiobuf: public _CXXRT_STD basic_streambuf<char, _CXXRT_STD char_traits<char> >
  {
  public:
    cstdiobuf(_CXXRT_STD FILE *f);

  protected:
    int_type   underflow();

  private:
    cstdiobuf(cstdiobuf const&);
    void operator= (cstdiobuf const&);

    _CXXRT_STD FILE* m_file;
    char             m_char[65536];
  };

  cstdiobuf::cstdiobuf(::_CXXRT_STD_NAME::FILE *f):
    m_file(f)
  {
    setg(m_char, m_char + 1, m_char + 1);
  }

  _CXXRT_STD basic_ios<char, _CXXRT_STD char_traits<char> >::int_type cstdiobuf::underflow()
  {
    using _CS_swamp::_impure_ptr;
    if (eback() == egptr())
	return _CXXRT_STD char_traits<char>::eof();
      
    _CXXRT_STD streamsize sz = _CXXRT_STD fread(m_char, 1, _CXXRT_STD _CS_std_bufsize, stdin);
    setg(m_char, m_char, m_char + sz);
    return sz > 0? _CXXRT_STD char_traits<char>::to_int_type(m_char[0]): _CXXRT_STD char_traits<char>::eof();
  }
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

using _CS_swamp::_impure_ptr;
basic_istream<char, char_traits<char> > cin(new cstdiobuf(stdin), &cout);

_CXXRT_NAMESPACE_END
#else

// #include <unistd.h>
extern "C" int read(int, char*, int);

namespace
{
  class stdiobuf:
    public _CXXRT_STD streambuf
  {
  public:
    stdiobuf(int fd): m_fd(fd) {}
  private:
    int_type underflow()
    {
      if (gptr() == egptr())
	{
	  _CXXRT_STD streamsize size = read(m_fd, m_buffer, s_size);
	  if (size > 0)
	    setg(m_buffer, m_buffer, m_buffer + size);
	}

      return gptr() == egptr()? traits_type::eof(): traits_type::to_int_type(*gptr());
    }

    int m_fd;
    enum { s_size = 256 };
    char m_buffer[s_size];
  };

  stdiobuf _Cxxrt_cinbuf(0);
}

_CXXRT_NAMESPACE_BEGIN

istream cin(&_Cxxrt_cinbuf, &cout);

_CXXRT_NAMESPACE_END

#endif
