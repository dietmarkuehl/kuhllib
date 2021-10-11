// -*-C++-*- boost/pimpl_ptr.hpp
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
// Title:  A referenced counted pointer for use in the "pimpl" idiom
// Version: $Id: pimpl_ptr.hpp,v 1.1 2000/04/30 17:59:11 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_PIMPL_PTR_HPP)
#define BOOST_PIMPL_PTR_HPP 1

namespace boost
{

// --------------------------------------------------------------------------

  struct pimpl_base
  {
    pimpl_base(): m_count(0) {}
    virtual ~pimpl_base() {}
    int m_count;
  };

  template <class Pimpl>
  class pimpl_ptr
  {
  public:
    pimpl_ptr(Pimpl* ptr): m_ptr(ptr) { ++(m_ptr->m_count); }
    pimpl_ptr(pimpl_ptr const& ptr): m_ptr(ptr.m_ptr) { ++(m_ptr->m_count); }
    ~pimpl_ptr() { if (--(m_ptr->m_count) == 0) delete m_ptr; }
    pimpl_ptr& operator= (pimpl_ptr const& ptr)
      {
	pimpl_base* del = m_ptr;
	m_ptr = ptr.m_ptr;
	++(m_ptr->m_count);
	if (--(del->m_count) == 0)
	  delete del;
      }

    Pimpl& operator*() const { return *static_cast<Pimpl*>(m_ptr); }
    Pimpl* operator->() const { return static_cast<Pimpl*>(m_ptr); }

    template <class Base>
    operator pimpl_ptr<Base> () const { return pimpl_ptr<Base>(static_cast<Pimpl*>(m_ptr)); }

  private:
    pimpl_base* m_ptr;
  };

// --------------------------------------------------------------------------

} // namespace boost

#endif /* BOOST_PIMPL_PTR_HPP */
