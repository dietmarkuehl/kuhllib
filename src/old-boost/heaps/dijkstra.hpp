// -*-C++-*- boost/dijkstra.hpp
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
// Title:  Generic implementation of Dijkstra's algorithm (for shortest paths in graphs)
// Version: $Id: dijkstra.hpp,v 1.1.1.1 1999/08/19 23:18:24 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_DIJKSTRA_HPP)
#define BOOST_DIJKSTRA_HPP 1

#include <functional>
#include "boost/d_heap.h"

namespace boost
{

// --- Algorithm iterator class ---------------------------------------------

template <
  typename AdjIt,     // the graph's structure
  typename AdjDA,     // get hold of adjacent nodes
  typename DistDA,    // [current] distance of a node
  typename LengthDA,  // length of an edge
  typename HeapPtrDA, // auxiliary data need for efficient lookup in the heap
  typename Comp = std::less<typename DistDA::value_type>,
  typename Heap = d_heap<AdjIt, dijkstra_compare<AdjIt, DistDA, Comp> > >
class dijkstra_iterator
{
public:
  dijkstra_iterator(AdjDA const& adj_da = AdjDA(),
		    DistDA const& dist_da = DistDA(),
		    LengthDA const& length_da = LengthDA(),
		    HeapPtrDA const& heap_da = HeapPtrDA(),
		    Comp const& comp = Comp()):
    m_adj_da(adj_da),
    m_dist_da(dist_da),
    m_length_da(length_da),
    m_heap_da(heap_da),
    m_comp(comp)
    {
    }

  dijkstra_iterator(AdjIt root,
		    AdjDA const& adj_da = AdjDA(),
		    DistDA const& dist_da = DistDA(),
		    LengthDA const& length_da = LengthDA(),
		    HeapPtrDA const& heap_ptr_da = HeapPtrDA(),
		    Comp const& comp = Comp()):
    m_adj_da(adj_da),
    m_dist_da(dist_da),
    m_length_da(length_da),
    m_heap_ptr_da(heap_ptr_da),
    m_comp(comp)
    {
      set(m_dist_da, root, typename DistDA::value_type());
      add_adjacent_nodes(root);
    }

private:
  void add_adjacent_noes(AdjIt node)
    {
      typename DistDA::value_type dist = get(m_dist_da, node);

      for (; node.valid(); node.next())
      {
	typename DistDA::value_type ndist = dist + get(m_length_da, node);
	AdjIt current = get(m_adj_da, node);
	typename HeapPtrDA::value_type const& ptr = get(m_heap_ptr_da, current);

	if (ptr == typename HeapPtrDA::value_type())
	{
	  set(m_dist_da, current, ndist);
	  m_heap.push(node);
	}
	else if (m_comp(ndist, get(m_dist_da, current)))
	{
	  set(m_dist_da, current, ndist);
	  m_heap.change(ptr, node)
	}
      }
    }
};


// --------------------------------------------------------------------------

} // namespace boost

#endif /* BOOST_DIJKSTRA_HPP */
