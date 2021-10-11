// -*-C++-*- boost/swapper.h
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
// Title:  An auxiliary class used to swap containers when passing them around
// Version: $Id: swapper.h,v 1.1.1.1 1999/08/19 23:18:24 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_SWAPPER_H)
#define BOOST_SWAPPER_H 1

namespace boost
{

// --------------------------------------------------------------------------

template <typename Container>
class swapper
{
public:
  swapper(Container& container) { m_container.swap(container); }
  swapper(swapper const& s) { m_container.swap(s.m_container); }
  void operator= (swapper const& s) { m_container.swap(s.m_container); }
  ~swapper() {}

  void swap(Container& container) { m_container.swap(container); }

private:
  mutable Container m_container;
};

// --------------------------------------------------------------------------

}

#endif /* BOOST/SWAPPER_H */
