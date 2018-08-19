// -*-C++-*- stdiobuf.hpp
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
// Title:   A starter version of a "stdiobuf", ie. a stream buffer forwarding to a FILE*
// Version: $Id: stdiobuf.cpp,v 1.1.1.1 2002/06/05 01:00:09 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(STDIOBUF_HPP)
#define STDIOBUF_HPP

#include <streambuf>

// -------------------------------------------------------------------------- 

namespace boost {

  class stdiobuf: public std::basic_streambuf<char, std::char_traits<char> >
  {
  public:
    stdiobuf(std::FILE *f);
    
  protected:
    int_type   underflow();
    int_type   overflow(int_type c);
    int        sync();
    
  private:
    stdiobuf(stdiobuf const&);
    void operator= (stdiobuf const&);
    
    std::FILE* m_file;
    char       m_char[8192];
  };
  
  inline
    stdiobuf::cstdiobuf(std::FILE *f):
    m_file(f)
    {
      setg(m_char, m_char + sizeof(m_char), m_char + sizeof(m_char));
    }
  
  inline
    std::char_traits<char>::int_type stdiobuf::underflow()
    {
      if (eback() == egptr())
	return std::char_traits<char>::eof();
      
      std::streamsize sz = std::fread(m_char, 1, sizeof(m_char), stdin);
      setg(m_char, m_char, m_char + sz);
      return sz > 0? std::char_traits<char>::to_int_type(m_char[0]): std::char_traits<char>::eof();
    }


  inline
    std::char_traits<char>::int_type cstdiobuf::overflow(std::ostream::int_type c)
    {
      if (!traits_type::eq_int_type(c, traits_type::eof()))
	{
	  *pptr() = std::char_traits<char>::to_char_type(c);
	  pbump(1);
	}

      return flush()? traits_type::not_eof(c): traits_type::eof();
    }

  int cstdiobuf::sync()
    {
      if (!flush())
	return -1;
      return std::fflush(m_file);
    }
  
// -------------------------------------------------------------------------- 

#endif /* STDIOBUF_HPP */
