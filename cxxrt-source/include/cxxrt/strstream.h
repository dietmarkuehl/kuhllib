// -*-C++-*- cxxrt/strstream.h
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
// Title:   Declaration for the deprecated strstream classes
// Version: $Id: strstream.h,v 1.1.1.1 2002/06/05 01:01:57 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_STRSTREAM_H__)
#define _CXXRT_STRSTREAM_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif
#if !defined(_CXXRT_IOSTREAM_H__)
#  include <cxxrt/iostream.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

class strstreambuf: public basic_streambuf<char>
{
public:
  explicit strstreambuf(streamsize = 0);
  strstreambuf(void* (*)(size_t), void (*)(void*));
  strstreambuf(char*, streamsize, char* = 0);
  strstreambuf(char const*, streamsize);

  strstreambuf(signed char*, streamsize, signed char* = 0);
  strstreambuf(signed const char*, streamsize);
  strstreambuf(unsigned char*, streamsize, unsigned char* = 0);
  strstreambuf(unsigned const char*, streamsize);

  virtual ~strstreambuf()
    {
      _CXXRT_DEPRECATED(strstreambuf__is__deprecated,use__stringbuf__instead)
      _CS_destroy();
    }

  void  freeze(bool = true);
  char* str();
  int   pcount();

protected:
  virtual int_type   overflow (int_type c = _CXXRT_STD char_traits<char>::eof());
  virtual int_type   pbackfail(int_type c = _CXXRT_STD char_traits<char>::eof());
  virtual int_type   underflow();
  virtual pos_type   seekoff(off_type, ios_base::seekdir, ios_base::openmode = ios_base::in | ios_base::out);
  virtual pos_type   seekpos(pos_type, ios_base::openmode = ios_base::in | ios_base::out);
  virtual streambuf* setbuf(char*, streamsize);

private:
  strstreambuf(strstreambuf const&);   // deliberately not accessible
  void operator=(strstreambuf const&); // deliberately not accessible

  void _CS_init(char*, streamsize, char*);
  void _CS_destroy();

  streamsize   _CS_m_chunk_size;
  unsigned int _CS_m_allocated:1;
  unsigned int _CS_m_constant:1;
  unsigned int _CS_m_dynamic:1;
  unsigned int _CS_m_frozen:1;

  void* (*_CS_m_alloc)(size_t);
  void  (*_CS_m_free)(void*);
};

// --------------------------------------------------------------------------

struct _CS_strstream_pbase
{
public:
  friend class istrstream;
  friend class ostrstream;
  friend class strstream;

  virtual ~_CS_strstream_pbase();

  _CS_strstream_pbase();
  _CS_strstream_pbase(char const*, streamsize);
  _CS_strstream_pbase(char*, streamsize);
  _CS_strstream_pbase(char*, streamsize, char*);
  strstreambuf* rdbuf() const;
  char* str();

private:
  mutable strstreambuf _CS_m_sb;
};

// --------------------------------------------------------------------------

class istrstream: public virtual _CS_strstream_pbase,
                  public basic_istream<char>
{
public:
  explicit istrstream(char const*);
  explicit istrstream(char*);
  istrstream(char const*, streamsize);
  istrstream(char*, streamsize);

  virtual ~istrstream()
    {
      _CXXRT_DEPRECATED(istrstream__is__deprecated,use__istringstream__instead)
    }

private:
  istrstream(istrstream const&);     // deliberately made inaccessible
  void operator=(istrstream const&); // deliberately made inaccessible
};

// --------------------------------------------------------------------------

class ostrstream:
  public virtual _CS_strstream_pbase,
  public basic_ostream<char>
{
public:
  ostrstream();
  ostrstream(char*, int, ios_base::openmode = ios_base::out);
  virtual ~ostrstream()
    {
      _CXXRT_DEPRECATED(ostrstream__is__deprecated,use__ostringstream__instead)
    }

  void freeze(bool freezefl = true);
  int pcount() const;

private:
  ostrstream(ostrstream const&);     // deliberately made inaccessible
  void operator=(ostrstream const&); // deliberately made inaccessible
};

// --------------------------------------------------------------------------

class strstream:
  public virtual _CS_strstream_pbase,
  public basic_iostream<char>
{
public:
  typedef char                        char_type;
  typedef char_traits<char>::int_type int_type;
  typedef char_traits<char>::pos_type pos_type;
  typedef char_traits<char>::off_type off_type;

  strstream();
  strstream(char*, int, ios_base::openmode = ios_base::in|ios_base::out);
  virtual ~strstream()
    {
      _CXXRT_DEPRECATED(strstream__is__deprecated,use__stringstream__instead)
    }

  void freeze(bool = true);
  int pcount() const;

private:
  strstream(strstream const&);     // deliberately made inaccessible
  void operator=(strstream const&); // deliberately made inaccessible
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_STRSTREAM_H__ */
