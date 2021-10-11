// -*-C++-*- boost/fixedstream.hpp
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
// Title:  Streams reading and writing to a fixed character array
// Version: $Id: fixedstream.hpp,v 1.1 2000/03/14 03:44:48 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_FIXEDSTREAM_HPP)
#define BOOST_FIXEDSTREAM_HPP 1

#include <streambuf>
#include <istream>
#include <ostream>

namespace boost
{

// --------------------------------------------------------------------------

template <typename cT, typename traits = std::char_traits<cT> >
class basic_fixedbuf: public std::basic_streambuf<cT, traits>
{
public:
  typedef typename std::basic_streambuf<cT, traits>::char_type   char_type;
  typedef typename std::basic_streambuf<cT, traits>::int_type    int_type;
  typedef typename std::basic_streambuf<cT, traits>::pos_type    pos_type;
  typedef typename std::basic_streambuf<cT, traits>::off_type    off_type;
  typedef typename std::basic_streambuf<cT, traits>::traits_type traits_type;

  basic_fixedbuf(cT const* str, std::streamsize len)
    {
      setg(const_cast<cT*>(str), const_cast<cT*>(str), const_cast<cT*>(str + len));
    }
  basic_fixedbuf(cT* str, std::streamsize len,
		 std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
    {
      if (mode & std::ios_base::in)
	setg(str, str, str + len);
      if (mode & std::ios_base::out)
	setp(str, str + len);
    }

  char const* str() const { return pbase() == 0? eback(): pbase(); }

private:
  std::basic_streambuf<cT, traits>* setbuf(cT* str, std::streamsize len)
    {
      if (gptr() != 0)
	setg(str, str, str + len);
      if (pptr() != 0)
	setp(str, str + len);
      return gptr() != 0 || pptr() != 0? this: 0;
    }

   
  pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)
    {
      pos_type rc(-1);

      switch (dir)
	{
	case std::ios_base::beg:
	  if (off >= 0)
	    {
	      if (which & std::ios_base::in && gptr() != 0
		  && off <= egptr() - eback())
		{
		  gbump(off - (gptr() - eback()));
		  rc = off;
		}
	      if (which & std::ios_base::out && pptr() != 0
		  && off <= epptr() - pbase())
		{
		  pbump(off - (pptr() - pbase()));
		  rc = off;
		}
	    }
	  break;
	case std::ios_base::cur:
	  if (gptr() == 0 || pptr() == 0 || off == 0)
	    {
	      if (which & std::ios_base::in && gptr() != 0
		  && off < egptr() - gptr() && -off < gptr() - eback())
		{
		  gbump(off);
		  rc = gptr() - eback();
		}
	      if (which & std::ios_base::out && pptr() != 0 &&
		  off < epptr() - pptr() && -off < pptr() - pbase())
		{
		  pbump(off);
		  rc = pptr() - pbase();
		}
	    }
	  break;
	case std::ios_base::end:
	  if (off <= 0)
	    {
	      if (which & std::ios_base::in && gptr() != 0
		  && egptr() - eback() >= -off)
		{
		  gbump(off + (egptr() - gptr()));
		  rc = gptr() - eback();
		}
	      if (which & std::ios_base::out && pptr() != 0
		  && epptr() - pbase() >= -off)
		{
		  pbump(off + (epptr() - pptr()));
		  rc = pptr() - pbase();
		}
	    }
	  break;
	}

      return rc;
    }

  pos_type seekpos(pos_type pos, std::ios_base::openmode which)
    {
      off_type off(pos);
      pos_type rc(-1);

      if (off >= 0)
	{
	  if (which & std::ios_base::in && gptr() != 0
	      && off <= egptr() - eback())
	    {
	      gbump(off - (gptr() - eback()));
	      rc = off;
	    }
	  if (which & std::ios_base::out && pptr() != 0
	      && off <= epptr() - pbase())
	    {
	      pbump(off - (pptr() - pbase()));
	      rc = off;
	    }
	}
      return rc;
    }

  int sync() { return 0; }
};

// --------------------------------------------------------------------------

template <typename cT, typename traits = std::char_traits<cT> >
class basic_ifixedstream: public std::basic_istream<cT, traits>
{
public:
  basic_ifixedstream(cT const* str, std::streamsize len):
    std::basic_ios<cT, traits>(&m_sbuf),
    std::basic_istream<cT, traits>(&m_sbuf),
    m_sbuf(str, len)
    {
    }

  cT const* str() const { return m_sbuf.str(); }

private:
  basic_fixedbuf<cT, traits> m_sbuf;
};

// --------------------------------------------------------------------------

template <typename cT, typename traits = std::char_traits<cT> >
class basic_ofixedstream: public std::basic_ostream<cT, traits>
{
public:
  basic_ofixedstream(cT* str, std::streamsize len):
    std::basic_ios<cT, traits>(&m_sbuf),
    std::basic_ostream<cT, traits>(&m_sbuf),
    m_sbuf(str, len, std::ios_base::out)
    {
    }

  cT const* str() const { return m_sbuf.str(); }

private:
  basic_fixedbuf<cT, traits> m_sbuf;
};

// --------------------------------------------------------------------------

template <typename cT, typename traits = std::char_traits<cT> >
class basic_fixedstream: public std::basic_iostream<cT, traits>
{
public:
  basic_fixedstream(cT* str, std::streamsize len):
    std::basic_ios<cT, traits>(&m_sbuf),
    std::basic_iostream<cT, traits>(&m_sbuf),
    m_sbuf(str, len, std::ios_base::in | std::ios_base::out)
    {
    }

  cT const* str() const { return m_sbuf.str(); }

private:
  basic_fixedbuf<cT, traits> m_sbuf;
};

// --------------------------------------------------------------------------

typedef basic_fixedbuf<char>     fixedbuf;
typedef basic_ifixedstream<char> ifixedstream;
typedef basic_ofixedstream<char> ofixedstream;
typedef basic_fixedstream<char>  fixedstream;

typedef basic_fixedbuf<wchar_t>     wfixedbuf;
typedef basic_ifixedstream<wchar_t> wifixedstream;
typedef basic_ofixedstream<wchar_t> wofixedstream;
typedef basic_fixedstream<wchar_t>  wfixedstream;

// --------------------------------------------------------------------------

} // namespace boost

#endif /* BOOST_FIXEDSTREAM_HPP */
