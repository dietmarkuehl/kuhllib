// -*-C++-*- boost/otabstream_iterator.hpp
// <!!----------------------------------------------------------------------> 
// <!! Copyright (C) 2000 Dietmar Kuehl, Claas Solutions GmbH > 
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
// Title:  Output Iterator for Streams Setting the Width
// Version: $Id: otabstream_iterator.hpp,v 1.1 2000/05/24 01:54:12 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_OTABSTREAM_ITERATOR_HPP)
#define BOOST_OTABSTREAM_ITERATOR_HPP 1

#include <ostream>
#include <iterator>
#include <string>

namespace boost
{

  template <typename T, typename cT = char,
	    typename traits = std::char_traits<cT> >
  class otabstream_iterator:
    public std::iterator<std::output_iterator_tag,
			 void, void, void, void>
  {
  public:
    otabstream_iterator(std::basic_ostream<cT, traits>& s,
                        int w, cT const* str = ""):
      m_stream(s), m_width(w), m_str(str) {}

    otabstream_iterator& operator= (T const& val) {
      m_stream.width(m_width);
      m_stream << val << m_str;
      return *this;
    }
    otabstream_iterator& operator* () { return *this; }
    otabstream_iterator& operator++() { return *this; }
    otabstream_iterator& operator++(int) { return *this; }
    
  private:
    std::basic_ostream<cT, traits>& m_stream;
    int                             m_width;
    std::basic_string<cT>           m_str;
  };

} // namespace boost

#endif /* BOOST_OTABSTREAM_ITERATOR_HPP */
