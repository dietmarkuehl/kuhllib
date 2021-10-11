// -*-C++-*- boost/trial_heap.cc
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
// Title:  Implementation of a Fibonacci heap
// Version: $Id: trial_heap.cc,v 1.1.1.1 1999/08/19 23:18:24 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_TRIAL_HEAP_CC)
#define BOOST_TRIAL_HEAP_CC 1

#if !defined(BOOST_TRIAL_HEAP_H)
#  include "trial_heap.h"
#endif

namespace trial
{

// --- implementation of the iterator class for Fibonacci heaps -------------

template <class T>
inline fibonacci_heap_base<T>::node const*
fibonacci_heap_base<T>::iterator::find_root(typename std::vector<node*>::const_iterator it) const
{
  for (; it != m_heap->m_roots.end(); ++it)
    if ((*it) != 0)
      return (*it);
  return 0;
}

template <class T>
inline fibonacci_heap_base<T>::node const*
fibonacci_heap_base<T>::iterator::find_leaf(node const* n) const
{
  if (n == 0)
    return 0;

  while (n->m_children.size() != 0)
    n = n->m_children[0];

  return n;
}

template <class T>
inline
fibonacci_heap_base<T>::iterator::iterator(fibonacci_heap_base<T> const* h):
  m_heap(h),
  m_node(0)
{
  if (m_heap->m_size == 0)
    return;

  m_node = find_leaf(find_root(m_heap->m_roots.begin()));
}

template <class T>
inline fibonacci_heap_base<T>::iterator&
fibonacci_heap_base<T>::iterator::operator++ ()
{
  if (m_node->is_root())
    m_node = find_leaf(find_root(m_heap->m_roots.begin() + m_node->rank() + 1));
  else if (m_node->m_parent->m_children.size() == m_node->m_index + 1)
    m_node = m_node->m_parent;
  else
    m_node = find_leaf(m_node->m_parent->m_children[m_node->m_index + 1]);

  return *this;
}

template <class T>
inline fibonacci_heap_base<T>::iterator
fibonacci_heap_base<T>::iterator::operator++ (int)
{
  iterator rc(*this);
  operator++ ();
  return rc;
}

// --- implementation of Fibonacci heap's node member functions ----------------

template <class T>
inline fibonacci_heap_base<T>::node*
fibonacci_heap_base<T>::node::join(node* tree)
{
  tree->m_index  = m_children.size();
  tree->m_parent = this;
  m_children.push_back(tree);
  m_state = clean;

  return this;
}

template <class T>
inline void
fibonacci_heap_base<T>::node::cut(node* child)
{
  size_type index = child->m_index;
  if (m_children.size() > index + 1)
    {
      m_children[index] = m_children[m_children.size() - 1];
      m_children[index]->m_index = index;
    }
  m_children.pop_back();
  m_state = node_state(m_state + 1);
  // m_state = static_cast<node_state>(m_state + 1);
}

// --- implementation of auxiliary the Fibonacci heap member functions ---------

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::add_root(node* n) const
{
  size_type rank = n->rank();
  if (m_roots.size() <= rank)
    {
      while (m_roots.size() < rank)
        m_roots.push_back(0);
      m_roots.push_back(n);
      n->clear();
    }
  else if (m_roots[rank] == 0)
    {
      m_roots[rank] = n;
      n->clear();
    }
  else
    {
      node* r = m_roots[rank];
      r->set_node();
      m_roots[rank] = 0;
      if (m_compare(r->data(), n->data()))
	{
	  n->clear();
	  add_root(r->join(n));
	}
      else
	{
	  r->clear();
	  add_root(n->join(r));
	}
    }
}

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::find_min() const
{
  if (m_size == 0)
    m_min = 0;
  else
    {
      typename std::vector<node*>::const_iterator end = m_cut_nodes.end();
      typename std::vector<node*>::const_iterator it = m_cut_nodes.begin();

      for (; it != end; ++it)
	add_root(*it);
      m_cut_nodes.erase(m_cut_nodes.begin(), m_cut_nodes.end());

      end = m_roots.end();
      it = m_roots.begin();
      while (*it == 0)
        ++it;
      m_min = *it;

      for (++it; it != end; ++it)
        if (*it != 0 && m_compare((*it)->data(), m_min->data()))
          m_min = *it;
    }
}

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::cut(node* n)
{
  node* p = n->parent();
  p->cut(n);
  switch (p->state())
    {
    case clean:
      assert("clean after loosing a child!" == 0);
      break;
    case root_cut_one:
      m_roots[p->rank() + 1] = 0;
      p->set_cut();
      m_cut_nodes.push_back(p);
      m_min = 0;
      break;
    case cut_one:
      // nothing to do
      break;
    case cut_two:
      cut(p);
      p->set_cut();
      m_cut_nodes.push_back(p);
      break;
    default:
      // we are in the cut state...
      break;
    }
}

// --- implementation of Fibonacci heap's modifying member functions -----------

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::increase(pointer n, T const& data)
{
  n->data(data);
  typename std::vector<node*>::const_iterator it = n->begin(); 
  typename std::vector<node*>::const_iterator end = n->end(); 
  for (; it != end; ++it)
    if (m_compare((*it)->data(), data))
      {
	if (n->state() == clean)
	  m_roots[n->rank()] = 0;
	else
	  cut(n);

	for (it = n->begin(); it != end; ++it)
	  {
	    (*it)->set_cut();
	    m_cut_nodes.push_back(*it);
	  }
	n->remove_all();

	m_cut_nodes.push_back(*it);
	break;
      }

  if (m_min == n)
    m_min = 0;
}

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::decrease(pointer n, T const& data)
{
  n->data(data);
  if (n->state() < clean && m_compare(data, n->parent()->data()))
    {
      cut(n);
      m_cut_nodes.push_back(n);
      m_min = 0;
    }
}

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::change(pointer n, T const& data)
{
  if (m_compare(data, n->data()))
    decrease(n, data);
  else
    increase(n, data);
}

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::change_top(T const& data)
{
  if (m_min == 0)
    find_min();
  change(m_min, data);
}

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::remove(pointer n)
{
  if (n->state() == clean)
    m_roots[n->rank()] = 0;
  else
    cut(n);

  typename std::vector<node*>::const_iterator it = n->begin(); 
  typename std::vector<node*>::const_iterator end = n->end(); 
  for (it = n->begin(); it != end; ++it)
    {
      (*it)->set_cut();
      m_cut_nodes.push_back(*it);
    }

  m_min = 0;
  delete n;
}

// --- implementation of basic heap interface for Fibonacci heaps --------------

template <class T, class Comp>
inline T const&
fibonacci_heap<T, Comp>::top() const
{
  if (m_min == 0)
    find_min();
  return m_min->data();
}

template <class T, class Comp>
inline typename fibonacci_heap<T, Comp>::pointer
fibonacci_heap<T, Comp>::push(T const& data)
{
  node* n = new node(data);
  ++m_size;
  n->set_cut();
  m_cut_nodes.push_back(n);

  m_min = 0;
  return n;
}

template <class T, class Comp>
inline void
fibonacci_heap<T, Comp>::pop()
{
  if (m_min == 0)
    find_min();

  node* del = m_min;
  m_roots[m_min->rank()] = 0;
  
  typename std::vector<node*>::const_iterator end = del->end();

  for (typename std::vector<node*>::const_iterator it = del->begin(); it != end; ++it)
    {
      (*it)->set_cut();
      m_cut_nodes.push_back(*it);
    }
  --m_size;

  delete del;
  m_min = 0;
}

// -----------------------------------------------------------------------------

} // namespace boost

#endif /* BOOST_F_HEAP_CC */
