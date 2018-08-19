// -*-C++-*- src/cstrstrm.cc
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
// Title:   Definition of the deprecated strstream classes
// Version: $Id: cstrstrm.cpp,v 1.1.1.1 2002/06/05 01:00:18 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include "cxxrt/strstream.h"
#include <cstdlib>
#include <limits.h>

_CXXRT_NAMESPACE_BEGIN

static streamsize const _CS_default_chunk_size = 65536;

// -------------------------------------------------------------------------- 

strstreambuf::strstreambuf(streamsize chunk_size):
  _CS_m_chunk_size(chunk_size == 0? _CS_default_chunk_size: chunk_size),
  _CS_m_allocated(0),
  _CS_m_constant(0),
  _CS_m_dynamic(1),
  _CS_m_frozen(0),
  _CS_m_alloc(0),
  _CS_m_free(0)
{
}

strstreambuf::strstreambuf(void* (*_CS_alloc)(size_t), void (*_CS_free)(void*)):
  _CS_m_chunk_size(_CS_default_chunk_size),
  _CS_m_allocated(0),
  _CS_m_constant(0),
  _CS_m_dynamic(1),
  _CS_m_frozen(0),
  _CS_m_alloc(_CS_alloc),
  _CS_m_free(_CS_free)
{
}

strstreambuf::strstreambuf(char* gn, streamsize n, char* pb):
  _CS_m_chunk_size(0),
  _CS_m_allocated(0),
  _CS_m_constant(0),
  _CS_m_dynamic(0),
  _CS_m_frozen(0),
  _CS_m_alloc(0),
  _CS_m_free(0)
{
  _CS_init(gn, n, pb);
}

strstreambuf::strstreambuf(signed char* gn, streamsize n, signed char* pb):
  _CS_m_chunk_size(0),
  _CS_m_allocated(0),
  _CS_m_constant(0),
  _CS_m_dynamic(0),
  _CS_m_frozen(0),
  _CS_m_alloc(0),
  _CS_m_free(0)
{
  _CS_init(reinterpret_cast<char*>(gn), n, reinterpret_cast<char*>(pb));
}

strstreambuf::strstreambuf(unsigned char* gn, streamsize n, unsigned char* pb):
  _CS_m_chunk_size(0),
  _CS_m_allocated(0),
  _CS_m_constant(0),
  _CS_m_dynamic(0),
  _CS_m_frozen(0),
  _CS_m_alloc(0),
  _CS_m_free(0)
{
  _CS_init(reinterpret_cast<char*>(gn), n, reinterpret_cast<char*>(pb));
}

strstreambuf::strstreambuf(char const* gn, streamsize n):
  _CS_m_chunk_size(0),
  _CS_m_allocated(0),
  _CS_m_constant(1),
  _CS_m_dynamic(0),
  _CS_m_frozen(0),
  _CS_m_alloc(0),
  _CS_m_free(0)
{
  _CS_init(const_cast<char*>(gn), n, 0);
}

strstreambuf::strstreambuf(signed char const* gn, streamsize n):
  _CS_m_chunk_size(0),
  _CS_m_allocated(0),
  _CS_m_constant(1),
  _CS_m_dynamic(0),
  _CS_m_frozen(0),
  _CS_m_alloc(0),
  _CS_m_free(0)
{
  _CS_init(reinterpret_cast<char*>(const_cast<signed char*>(gn)), n, 0);
}

strstreambuf::strstreambuf(unsigned char const* gn, streamsize n):
  _CS_m_chunk_size(0),
  _CS_m_allocated(0),
  _CS_m_constant(1),
  _CS_m_dynamic(0),
  _CS_m_frozen(0),
  _CS_m_alloc(0),
  _CS_m_free(0)
{
  _CS_init(reinterpret_cast<char*>(const_cast<unsigned char*>(gn)), n, 0);
}

void
strstreambuf::_CS_init(char* gn, streamsize n, char* pb)
{
  if (n == 0)
    n = gn == 0? 0: strlen(gn);
  else if (n < 0)
    n = INT_MAX; 

  if (pb == 0)
    setg(gn, gn, gn + n);
  else
    {
      setg(gn, gn, pb);
      setp(pb, pb + n);
    }
}

void
strstreambuf::_CS_destroy()
{
  if (_CS_m_allocated != 0 && _CS_m_frozen == 0)
    if (_CS_m_free == 0)
      delete[] eback();
    else
      _CS_m_free(eback());
}

void 
strstreambuf::freeze(bool f)
{
  if (_CS_m_dynamic != 0)
    _CS_m_frozen = f? 1: 0;
}

char*
strstreambuf::str()
{
  if (_CS_m_dynamic != 0)
    _CS_m_frozen = 1;
  return gptr();
}

int
strstreambuf::pcount()
{
  return pptr() - pbase();
}

strstreambuf::int_type
strstreambuf::overflow (int_type c)
{
  if (c == char_traits<char>::eof())
    return ' ';

  if (pptr() == epptr())
    {
      if (_CS_m_dynamic == 0 || _CS_m_frozen == 1)
	return char_traits<char>::eof();

      streamsize n = epptr() - eback();
      char* tmp = _CS_m_alloc == 0
	? new char[n + _CS_m_chunk_size]
	: static_cast<char*>(_CS_m_alloc(n + _CS_m_chunk_size));
      char_traits<char>::copy(tmp, eback(), n);
      char* del = eback();
      setg(tmp, tmp + (gptr() - del), tmp + (egptr() - del));
      setp(tmp + (pptr() - del), tmp + n + _CS_m_chunk_size);
      if (_CS_m_allocated != 0)
	_CS_m_free == 0? delete[] del: _CS_m_free(del);

      _CS_m_allocated = 1;
    }

  *pptr() = static_cast<unsigned char>(c);
  pbump(1);
  return c;
}

strstreambuf::int_type
strstreambuf::pbackfail(int_type c)
{
  if (eback() == gptr())
    return char_traits<char>::eof();

  if (c != char_traits<char>::eof() && static_cast<char>(c) != gptr()[-1])
    if (_CS_m_constant == 1)
      return char_traits<char>::eof();
    else
      gptr()[-1] = static_cast<char>(c);
  else
    {
      gbump(-1);
      return 0;
    }

  gbump(-1);
  return c;
}

strstreambuf::int_type
strstreambuf::underflow()
{
  if (gptr() != egptr())
    return static_cast<unsigned char>(*gptr());
  else if (pptr() != 0 && egptr() != pptr())
    {
      setg(eback(), gptr(), pptr());
      return static_cast<unsigned char>(*gptr());
    }
    
  return char_traits<char>::eof();
}

strstreambuf::pos_type
strstreambuf::seekoff(off_type off, ios_base::seekdir dir, ios_base::openmode which)
{
  if (dir == ios_base::cur && (which & (ios_base::in | ios_base::out)) == (ios_base::in | ios_base::out)
      || (which & (ios_base::in | ios_base::out)) == 0)
    return pos_type(-1);

  off_type newoff = 0;
  if (dir == ios_base::cur)
    newoff = ((which & ios_base::in) != 0? gptr(): pptr()) - eback();
  else
    newoff = (epptr() == 0? egptr(): epptr()) - eback();

  if (newoff + off < gptr() - eback() || newoff + off >= epptr() - eback())
    return pos_type(-1);
  
  if (which & ios_base::in)
    if (gptr() == 0)
      return pos_type(-1);
    else
      setg(eback(), eback() + newoff + off, egptr());

  if (which & ios_base::out)
    if (pptr() == 0)
      return pos_type(-1);
    else
      setp(eback() + newoff + off, epptr());

  return newoff + off;
}

strstreambuf::pos_type
strstreambuf::seekpos(pos_type pos, ios_base::openmode which)
{
  if ((which & (ios_base::in | ios_base::out)) == 0
      || pos < 0 || (epptr() == 0? egptr(): epptr()) - gptr() < pos)
    return pos_type(-1);
    
  if (which & ios_base::in)
    if (gptr() == 0)
      return pos_type(-1);
    else
      setg(eback(), eback() + pos, egptr());
    
  if (which & ios_base::in)
    if (pptr() == 0)
      return pos_type(-1);
    else
      setp(eback() + pos, epptr());

  return pos;
}

streambuf*
strstreambuf::setbuf(char*, streamsize)
{
  //-dk:TODO ...but what?
  return this;
}

// -------------------------------------------------------------------------- 

_CS_strstream_pbase::_CS_strstream_pbase():
  _CS_m_sb()
{
}

_CS_strstream_pbase::_CS_strstream_pbase(char const* s, streamsize n):
  _CS_m_sb(s, n)
{
}

_CS_strstream_pbase::_CS_strstream_pbase(char* s, streamsize n):
  _CS_m_sb(s, n)
{
}

_CS_strstream_pbase::_CS_strstream_pbase(char* s, streamsize n, char* p):
  _CS_m_sb(s, n, p)
{
}

_CS_strstream_pbase::~_CS_strstream_pbase()
{
}

strstreambuf*
_CS_strstream_pbase::rdbuf() const
{
  return &_CS_m_sb;
}

char*
_CS_strstream_pbase::str()
{
  return rdbuf()->str();
}

// -------------------------------------------------------------------------- 

istrstream::istrstream(const char* s):
  _CS_strstream_pbase(s, 0),
  basic_ios<char>(&_CS_m_sb),
  basic_istream<char>(&_CS_m_sb)
{
}

istrstream::istrstream(char* s):
  _CS_strstream_pbase(s, 0),
  basic_ios<char>(&_CS_m_sb),
  basic_istream<char>(&_CS_m_sb)
{
}

istrstream::istrstream(const char* s, streamsize n):
  _CS_strstream_pbase(s, n),
  basic_ios<char>(&_CS_m_sb),
  basic_istream<char>(&_CS_m_sb)
{
}

istrstream::istrstream(char* s, streamsize n):
  _CS_strstream_pbase(s, n),
  basic_ios<char>(&_CS_m_sb),
  basic_istream<char>(&_CS_m_sb)
{
}

// -------------------------------------------------------------------------- 

ostrstream::ostrstream():
  _CS_strstream_pbase(),
  basic_ios<char>(&_CS_m_sb),
  basic_ostream<char>(&_CS_m_sb)
{
}

ostrstream::ostrstream(char* s, int n, ios_base::openmode mode):
  _CS_strstream_pbase(s, n, s + ((mode & app) == 0? 0: strlen(s))),
  basic_ios<char>(&_CS_m_sb),
  basic_ostream<char>(&_CS_m_sb)
{
}

void
ostrstream::freeze(bool f)
{
  _CS_strstream_pbase::rdbuf()->freeze(f);
}

int
ostrstream::pcount() const
{
  return _CS_strstream_pbase::rdbuf()->pcount();
}

// -------------------------------------------------------------------------- 

strstream::strstream():
  _CS_strstream_pbase(),
  basic_ios<char>(&_CS_m_sb),
  basic_iostream<char>(&_CS_m_sb)
{
}
strstream::strstream(char* s, int n, ios_base::openmode mode):
  _CS_strstream_pbase(s, n, s + ((mode & app) == 0? 0: strlen(s))),
  basic_ios<char>(&_CS_m_sb),
  basic_iostream<char>(&_CS_m_sb)
{
}

void
strstream::freeze(bool f)
{
  _CS_strstream_pbase::rdbuf()->freeze(f);
}

int
strstream::pcount() const
{
  return _CS_strstream_pbase::rdbuf()->pcount();
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
