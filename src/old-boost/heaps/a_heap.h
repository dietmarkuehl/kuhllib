// -*-C++-*- boost/a_heap.h
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
// Title:  Administrative priority queue wrappers
// Version: $Id: a_heap.h,v 1.1.1.1 1999/08/19 23:18:24 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_A_HEAP_H)
#define BOOST_A_HEAP_H 1

#include <iosfwd>
#include <list>

namespace boost
{

// --------------------------------------------------------------------------

template <typename PQ>
class log_heap
{
  struct node
  {
    node(typename PQ::pointer, typename PQ::size_type);
    typename PQ::pointer   m_ptr;
    typename PQ::size_type m_index;
  };
public:
  typedef typename PQ::value_type      value_type;
  typedef typename PQ::size_type       size_type;
  typedef typename PQ::pointer         pointer;
  typedef typename PQ::const_iterator  const_iterator;
  typedef typename PQ::const_reference const_reference;
  typedef typename PQ::size_type       size_type;

  log_heap(compare_type const& comp = compare_type): m_out(std::cout), m_pq(comp) {}
  log_heap(std::ostream& out, compare_type const& comp = compare_type): m_out(out), m_pq(comp) {}
  
  pointer         push(value_type const& val);
  const_reference top() const;
  const_reference access(pointer) const;
  void            pop();
  void            remove();

  template <typename K> void change_top(K const&);
  template <typename K> void change(pointer, K const&);
  template <typename K> void decrease(pointer, K const&);
  template <typename K> void increase(pointer, K const&);

private:
  log_heap(log_heap const&);       // deliberately not implemented
  void operator=(log_heap const&); // deliberately not implemented

  std::ostream&   m_out;
  PQ              m_pq;
  std::list<node> m_nodes;
  size_type       m_index;
};

// --------------------------------------------------------------------------

} // namespace boost

#endif /* BOOST_A_HEAP_H */
