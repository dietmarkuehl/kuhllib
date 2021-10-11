// -*-C++-*- boost/mtree.h
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
// Title:  A simple tree data structure with arbitrary number of nodes
// Version: $Id: mtree.h,v 1.1.1.1 1999/08/19 23:18:24 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_MTREE_H)
#define BOOST_MTREE_H 1

#include <list>

namespace boost
{

// --------------------------------------------------------------------------

template <class T>
class multi_tree
{
private:
  struct node;
  typedef typename std::list<node*>::iterator children_iterator;

  struct node
  {
    node*                               m_parent;
    typename std::list<node*>::iterator m_index;   // pos. in parent's list
    std::list<node*>                    m_children;
    T                                   m_data;

    node(T const& data): m_parent(0), m_data(data) {}
    ~node()
      {
	for (typename std::list<node*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	  delete *it;
      }
    void insert(node* n, typename std::list<node*>::iterator pos)
      {
	n->m_index  = m_children.insert(pos, n);
	n->m_parent = this;
      }
  };

public:
  class iterator
  {
    friend class multi_tree;

    iterator(node* n): m_node(n), m_it(n->m_children.begin()) {}
  public:
    iterator(): m_node(0), m_it() {}
    iterator(iterator const& it): m_node(it.m_node), m_it(it.m_it) {}
    iterator& operator= (iterator const& it) { m_node = it.m_node; m_it = it.m_it; return *this; }
    ~iterator() {}

    bool operator== (iterator const& it) const
      {
	if (m_node == 0)
	  return it.m_node == 0;
	else if (it.m_node == 0)
	  return false;
	else
	  return m_it == it.m_it;
      }
    bool operator!= (iterator const& it) const { return !operator==(it); }
    bool valid() const { return m_node != 0 && m_it != m_node->m_children.end(); }

    // pre condition: '*this != iterator()' is true
    iterator& operator++ () { ++m_it; return *this; }
    iterator  operator++ (int) { iterator rc; operator++(); return rc; }
    iterator  up() const { return m_node->m_parent; }
    T& operator*() const { return m_node->m_data; }
    T* operator->() const { return &(m_node->m_data); }

    // pre condition: 'this->valid()' is true
    iterator  down() const { return *m_it; }

  private:
    node*                               m_node;
    typename std::list<node*>::iterator m_it;
  };

  multi_tree(): m_root(0) {}
  ~multi_tree() { delete m_root; }

  iterator root() const { return m_root == 0? iterator(): iterator(m_root); }
  iterator insert(T const&, iterator = iterator());
  void     remove(iterator);

private:
  multi_tree(multi_tree const&);      //-dk: TODO
  void operator= (multi_tree const&); //-dk:TODO

  node* m_root;
};

template <class T>
multi_tree<T>::iterator
multi_tree<T>::insert(T const& data, iterator loc)
{
  node* n = new node(data);
  if (loc.m_node == 0)
    {
      if (m_root != 0)
	n->insert(m_root, n->m_children.begin());
      m_root = n;
    }
  else
    loc.m_node->insert(n, loc.m_it);

  return n;
}

template <class T>
void
multi_tree<T>::remove(iterator del)
{
  if (del.m_node == 0)
    return;
  if (!del.m_node->m_children.empty())
    {
      node* np = del.m_node->m_parent;
      typename std::list<node*>::iterator pos;
      if (np == 0)
	{
	  np = *del.m_node->m_children.begin();
	  del.m_node->m_children.pop_front();
	  m_root = np;
	  pos = np->m_children.end();
	}
      else
	pos = del.m_node->m_index;

      for (typename std::list<node*>::iterator it = del.m_node->m_children.begin();
	   it != del.m_node->m_children.end(); ++it)
	(*it)->m_parent = np;
      np->m_children.splice(pos, del.m_node->m_children);

      for (typename std::list<node*>::iterator it = np->m_children.begin(); it != np->m_children.end(); ++it)
	if ((*it)->m_parent == np)
	  (*it)->m_index  = it;
    }

  if (del.m_node->m_parent != 0)
    del.m_node->m_parent->m_children.erase(del.m_node->m_index);
  delete del.m_node;
}

// --------------------------------------------------------------------------

} // namespace boost

#endif /* BOOST_MTREE_H */
