// -*-C++-*- boost/pstream.h
// <!!----------------------------------------------------------------------> 
// <!! Copyright (C) 1999 Dietmar Kuehl, Claas Solutions GmbH > 
// <!!> 
// <!! Permission to use, copy, modify, distribute and sell this > 
// <!! software for any purpose is hereby granted without fee, provided > 
// <!! that the above copyright notice appears in all copies and that > 
// <!! both that copyright notice and this permission notice appear in > 
// <!! supporting documentation. Dietmar Kuehl and Claas Solutions make no > 
// <!! representations about the suitability of this software for any > 
// <!! purpose. It is provided "as is" without express or implied warranty. > 
// <!!----------------------------------------------------------------------> 

// Author: Dietmar Kuehl dietmar.kuehl@claas-solutions.de 
// Title:  Streams using POSIX file descriptors
// Version: $Id: pstream.h,v 1.2 2001/06/04 05:28:20 kuehl Exp $ 

// Note: Currently these classes are restricted to character type 'char'
//       because no code convertion facilities are incorporated into the
//       code. Still, everything is templatized for smooth transition.
//       The current implementation consists completely of specialized
//       template functions for character type char and traits type
//       STD char_traits<char>

// -------------------------------------------------------------------------- 

#if !defined(BOOST_PSTREAM_H)
#define BOOST_PSTREAM_H 1

// -------------------------------------------------------------------------- 

#include <streambuf>
#include <istream>
#include <ostream>
#include <iostream>

// -------------------------------------------------------------------------- 

// A hack to find the correct namespace even if my IOStream implementation
// is used but lives in a different namespace than std:
#if defined(_CXXRT_STD_NAME)
#  define STD _CXXRT_STD_NAME::
#else
#  define STD std::
#endif

#define BOOST_NAMESPACE_BEGIN namespace boost {
#define BOOST_NAMESPACE_END   }

BOOST_NAMESPACE_BEGIN

// --------------------------------------------------------------------------
// The heart of the implementation, the stream buffer:

template <class cT, class traits = STD char_traits<cT> >
class basic_posixbuf;

template <>
class basic_posixbuf<char, STD char_traits<char> >:
  public STD basic_streambuf<char, STD char_traits<char> >
{
public:
  typedef char                  char_type;
  typedef STD char_traits<char> traits_type;
  typedef traits_type::int_type int_type;
  typedef traits_type::pos_type pos_type;
  typedef traits_type::off_type off_type;

  basic_posixbuf();
  basic_posixbuf(char const*, STD ios_base::openmode = STD ios_base::in | STD ios_base::out);
  basic_posixbuf(int);
  ~basic_posixbuf();

  bool open(char const*, STD ios_base::openmode = STD ios_base::in | STD ios_base::out);
  bool attach(int);
  bool close();

  bool is_open() const { return m_fd != -1; }
  int fd() const { return m_fd; }
  
private:
  void install_buffer();
  void release_buffer(); // only called from close()

  int_type underflow();
  int_type overflow(int_type);
  int      sync();

  void prepare_seek();
  pos_type seekpos(pos_type, STD ios_base::openmode);
  pos_type seekoff(off_type, STD ios_base::seekdir, STD ios_base::openmode);
  
  int                    m_fd;
  bool                   m_owner;
  unsigned long          m_bufsiz;
  char*                  m_buffer;
  char*                  m_current;
  char*                  m_end;
  STD ios_base::openmode m_mode;
};

// --------------------------------------------------------------------------
// The following auxiliary class is used to guarantee proper initialization
// and destruction: Virtual bases classes are constructed first in a left
// to right DFS order. Thus, this can be used to construct a member prior
// to all base classes.

template <class cT, class traits>
struct basic_posixbuf_pbase
{
  basic_posixbuf_pbase();
  basic_posixbuf_pbase(char const*, STD ios_base::openmode);
  basic_posixbuf_pbase(int fd);

  basic_posixbuf<cT, traits> m_sbuf;
};

// --------------------------------------------------------------------------
// Convenience classes for easy construction and easy access:

template <class cT, class traits = STD char_traits<cT> >
class basic_iposixstream: private virtual basic_posixbuf_pbase<cT, traits>,
                          public STD basic_istream<cT, traits>
{
public:
  basic_iposixstream();
  basic_iposixstream(char const*, STD ios_base::openmode = STD ios_base::in);
  basic_iposixstream(int);

  basic_posixbuf<cT, traits>* rdbuf() { return &m_sbuf; }

  basic_iposixstream<cT, traits>& open(char const*, STD ios_base::openmode = STD ios_base::in);
  basic_iposixstream<cT, traits>& attach(int);
  basic_iposixstream<cT, traits>& close();

  bool is_open() const { return m_sbuf.is_open(); }
  int fd() const { return m_sbuf.fd(); }
};

template <class cT, class traits = STD char_traits<cT> >
class basic_oposixstream: private virtual basic_posixbuf_pbase<cT, traits>,
                          public STD basic_ostream<cT, traits>
{
public:
  basic_oposixstream();
  basic_oposixstream(char const*, STD ios_base::openmode = STD ios_base::out);
  basic_oposixstream(int);

  basic_posixbuf<cT, traits>* rdbuf() { return &m_sbuf; }

  basic_oposixstream<cT, traits>& open(char const*, STD ios_base::openmode = STD ios_base::out);
  basic_oposixstream<cT, traits>& attach(int);
  basic_oposixstream<cT, traits>& close();

  bool is_open() const { return m_sbuf.is_open(); }
  int fd() const { return m_sbuf.fd(); }
};

template <class cT, class traits = STD char_traits<cT> >
class basic_ioposixstream: private virtual basic_posixbuf_pbase<cT, traits>,
                           public STD basic_iostream<cT, traits>
{
public:
  basic_ioposixstream();
  basic_ioposixstream(char const*, STD ios_base::openmode = STD ios_base::in | STD ios_base::out);
  basic_ioposixstream(int);

  basic_posixbuf<cT, traits>* rdbuf() { return &m_sbuf; }

  basic_ioposixstream<cT, traits>& open(char const*,
                                        STD ios_base::openmode = STD ios_base::in | STD ios_base::out);
  basic_ioposixstream<cT, traits>& attach(int);
  basic_ioposixstream<cT, traits>& close();

  bool is_open() const { return m_sbuf.is_open(); }
  int fd() const { return m_sbuf.fd(); }
};

// --------------------------------------------------------------------------

typedef basic_posixbuf<char, STD char_traits<char> > posixbuf;
typedef basic_iposixstream<char, STD char_traits<char> > iposixstream;
typedef basic_oposixstream<char, STD char_traits<char> > oposixstream;
typedef basic_ioposixstream<char, STD char_traits<char> > ioposixstream;

// --------------------------------------------------------------------------

BOOST_NAMESPACE_END

#endif /* BOOST_PSTREAM_H */
