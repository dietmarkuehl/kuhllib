// -*-C++-*- src/wioobj.cc
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
// Title:   Definition of the standard wide character stream objects 
// Version: $Id: wioobj.cpp,v 1.1.1.1 2002/06/05 01:00:40 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ctraits.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <streambuf>

// -------------------------------------------------------------------------- 

#if 1
namespace
{

  class stdiobuf: public _CXXRT_STD basic_streambuf<wchar_t, _CXXRT_STD char_traits<wchar_t> >
  {
  public:
    stdiobuf(FILE *f);
    typedef _CXXRT_STD basic_streambuf<wchar_t, _CXXRT_STD char_traits<wchar_t> >::int_type int_type;

  protected:
    int        sync();
    int_type   overflow(int_type c);
    int_type   underflow();
    _CXXRT_STD streamsize xsputn(char_type const *s, _CXXRT_STD streamsize n);

  private:
    stdiobuf(stdiobuf const&);
    void operator= (stdiobuf const&);

    FILE *m_file;
    wchar_t     m_char[1];
  };

  stdiobuf::stdiobuf(FILE *f):
    m_file(f)
  {
    setg(0, 0, 0);
    setp(0, 0);
  }

  int stdiobuf::sync()
  {
    return fflush(m_file);
  }

  _CXXRT_STD basic_ios<wchar_t, _CXXRT_STD char_traits<wchar_t> >::int_type stdiobuf::underflow()
  {
    // using _CS_swamp::_impure_ptr;
    //-dk:TODO this is real slow!
    if ((fread(m_char, 1, 1, stdin)) != 1)
      return _CXXRT_STD char_traits<wchar_t>::eof();
    setg(m_char, m_char, m_char + 1);
    return _CXXRT_STD char_traits<char>::to_int_type(m_char[0]);
  }

  stdiobuf::int_type stdiobuf::overflow(stdiobuf::int_type c)
  {
    return fputc(c, m_file);
  }

  _CXXRT_STD streamsize stdiobuf::xsputn(wchar_t const *str, _CXXRT_STD streamsize s)
  {
    return fwrite(str, 1, s, m_file);
  }

// -------------------------------------------------------------------------- 

}

_CXXRT_NAMESPACE_BEGIN

// using _CS_swamp::_impure_ptr;
basic_istream<wchar_t, char_traits<wchar_t> > wcin (new stdiobuf(stdin), &wcout);
basic_ostream<wchar_t, char_traits<wchar_t> > wcout(new stdiobuf(stdout), false);
basic_ostream<wchar_t, char_traits<wchar_t> > wcerr(new stdiobuf(stderr), true);
basic_ostream<wchar_t, char_traits<wchar_t> > wclog(new stdiobuf(stderr), false);

_CXXRT_NAMESPACE_END
#endif
