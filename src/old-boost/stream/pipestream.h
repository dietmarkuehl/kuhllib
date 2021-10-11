// -*-C++-*- pipestream.h
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
// Title:  A stream writing to and reading from a process
// Version: $Id: pipestream.h,v 1.1 1999/11/10 17:52:02 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(PIPESTREAM_H)
#define PIPESTREAM_H 1

#include <streambuf>
#include <ostream>
#include <istream>
#include <string>

namespace boost
{

// --------------------------------------------------------------------------

  class pipestream: public std::basic_streambuf<char, std::char_traits<char> >
  {
  public:
    typedef char                        char_type;
    typedef std::char_traits<char_type> traits_type;
    typedef traits_type::int_type       int_type;

    pipestream(char* const av[]);
    pipestream(std::string const& command);
    ~pipestream();

    void close();
    void close_in();
    void close_out();

  protected:
    int_type overflow(int_type);
    int_type underflow();
    
  private:
    pipestream(pipestream const&);     // deliberately unaccessible
    void operator=(pipestream const&); // deliberately unaccessible

    void create_process(char* const av[]);

    int  m_to;
    int  m_from;
    char m_buf[64];
  };

// --------------------------------------------------------------------------

} // namespace foo

#endif /* PIPESTREAM_H */
