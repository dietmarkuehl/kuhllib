// -*-C++-*- boost/trial_heap.h
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
// Title:  Declaration for Fibonacci Heaps
// Version: $Id: trial_heap.h,v 1.1.1.1 1999/08/19 23:18:24 kuehl Exp $ 

// -------------------------------------------------------------------------- 
// This implementation is based on the description found in "Network Flow",
// R.K.Ahuja, T.L.Magnanti, J.B.Orlin, Prentice Hall. The algorithmic stuff
// should be basically identical to this description, however, the actual
// representation is not.
// -------------------------------------------------------------------------- 

#if !defined(BOOST_TRIAL_HEAP_H)
#define BOOST_TRIAL_HEAP_H 1

namespace trial
{

// --------------------------------------------------------------------------
// The template class fibonacci_heap_base defines everything which does
// not depend on the compare type. Thus, the actual algorithmic stuff
// is in the template class fibonacci_heap. However, the defintion of
// the nodes and the iterator type is in fibonacci_heap_base.

template <class T>
class fibonacci_heap_base
{
  struct node;

public:
  typedef T                                      value_type;
  typedef T&                                     reference;
  typedef T const&                               const_reference;
  typedef typename std::vector<node*>::size_type size_type;
  typedef node*                                  pointer;
  class                                          iterator;
  typedef iterator                               const_iterator;

  friend class iterator;

  enum node_state
  {
    cut_none     = 0,  // internal node with no child lost
    cut_one      = 1,  // internal node with one child lost
    cut_two      = 2,  // internal node with two child lost
    clean        = 4,  // root
    root_cut_one = 5,  // root lost a child => move in the m_roots array
    cut_self     = 8   // node currently in the list to be added to the roots
  };

protected:
  class node
  {
    friend class iterator;

  public:
    node(T const& data): m_parent(0), m_state(clean), m_data(data) { m_children.reserve(8); }
    
    node*      join(node* tree); // add tree as a child
    void       cut(node* child); // remove the child
    node_state state() const { return m_state; }
    void       set_root()        { m_state = clean; }
    void       set_cut()         { m_state = cut_self; }
    void       set_node()        { m_state = cut_none; }
    void       clear()           { m_parent = 0; m_state = clean; }
    size_type  rank() const      { return m_children.size(); }
    node*      parent() const    { return m_parent; }
    void       remove_all()      { m_children.erase(m_children.begin(), m_children.end()); }

    T const& data() const        { return m_data; }
    void     data(T const& data) { m_data = data; }

    typename std::vector<node*>::const_iterator begin() const { return m_children.begin(); }
    typename std::vector<node*>::const_iterator end() const   { return m_children.end(); }

  private:
    size_type          m_index;  // index of the object in the parent's vector
    node*              m_parent; // pointer to the parent node
    std::vector<node*> m_children;
    node_state         m_state;
    T                  m_data;
  };

  class iterator
    // : public std::iterator<std::forward_iterator_tag, T const, size_type, T const*, T const&>
  {
  public:
    iterator(): m_heap(0), m_node(0) {}
    iterator(fibonacci_heap_base const* h);
    iterator(fibonacci_heap_base const* h, node const* n): m_heap(h), m_node(n) {}

    // generated copy ctor, copy assignment, dtor are appropriate

    bool operator== (iterator const& it) const { return m_node == it.m_node; }
    bool operator!= (iterator const& it) const { return m_node != it.m_node; }
    T const& operator* () const  { return m_node->data(); }
    T const* operator-> () const { return &m_node->data(); }

    iterator &operator++ ();
    iterator operator++ (int);

  private:
    node const* find_leaf(node const*) const;
    node const* find_root(typename std::vector<node*>::const_iterator) const;

    fibonacci_heap_base const* m_heap;
    node const*                m_node;
  };

public:
  fibonacci_heap_base(): m_size(0) {}

  bool      empty() const { return m_size == 0; }
  size_type size() const  { return m_size; }

  iterator begin() const { return iterator(this); }
  iterator end() const   { return iterator(this, 0); }

protected:
  mutable std::vector<node*> m_roots;
  mutable std::vector<node*> m_cut_nodes;
  size_type          m_size;
};

// --------------------------------------------------------------------------
// The template class fibonacci_heap implements all algorithmic ingredients
// for an Fibonacci heap.

template <class T, class Comp = std::less<T> >
class fibonacci_heap: public fibonacci_heap_base<T>
{
  typedef typename fibonacci_heap_base<T>::node           node;

public:
  typedef typename fibonacci_heap_base<T>::size_type      size_type;
  typedef node*                                           pointer;
  typedef T                                               value_type;
  typedef T&                                              reference;
  typedef T const&                                        const_reference;
  typedef typename fibonacci_heap_base<T>::iterator       iterator;
  typedef typename fibonacci_heap_base<T>::const_iterator const_iterator;
  typedef Comp                                            compare_type;


  fibonacci_heap(Comp const& comp = Comp()): m_compare(comp) {}

  pointer  push(T const& data);
  void     pop();
  T const& top() const;
  void     change_top(T const& data);

  void     change(pointer, T const&);
  void     decrease(pointer, T const&);
  void     increase(pointer, T const&);
  void     remove(pointer);

private:
  void add_root(node* n) const;
  void find_min() const;
  void cut(node* n);

  Comp  m_compare;
  mutable node* m_min;
};

// --------------------------------------------------------------------------

} // boost

#if !defined(BOOST_TRIAL_HEAP_CC)
#  include "trial_heap.cc"
#endif

#endif /* BOOST_TRIAL_HEAP_H */
