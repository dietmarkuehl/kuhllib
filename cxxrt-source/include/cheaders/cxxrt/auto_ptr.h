// -*-C++-*- cxxrt/auto_ptr.h
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
// Title:   auto_ptr - use with care!
// Version: $Id: auto_ptr.h,v 1.1.1.1 2002/06/05 01:01:52 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_AUTO_PTR_H__)
#define _CXXRT_AUTO_PTR_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_T>
class auto_ptr
{
private:
  template <class _CS_S>
  struct auto_ptr_ref
  {
    mutable auto_ptr<_CS_S>& _CS_m_aptr;
    auto_ptr_ref(auto_ptr<_CS_S>& _CS_ap): _CS_m_aptr(_CS_ap) {}
    auto_ptr_ref(auto_ptr_ref const& _CS_ap): _CS_m_aptr(_CS_ap._CS_m_aptr) {}
  };

public:
  explicit auto_ptr(_CS_T* _CS_p = 0) throw(): _CS_m_ptr(_CS_p) {}
  auto_ptr(auto_ptr& _CS_ap) throw(): _CS_m_ptr(_CS_ap.release()) {}
  template <class _CS_S>
  auto_ptr(auto_ptr<_CS_S>& _CS_ap) throw(): _CS_m_ptr(_CS_ap.release()) {}
  
  auto_ptr(auto_ptr_ref<_CS_T> _CS_apr) throw():
    _CS_m_ptr(_CS_apr._CS_m_aptr.release())
    {}
  
  auto_ptr& operator= (auto_ptr& _CS_ap) throw()
    {
      if (&_CS_ap != this)
        reset(_CS_ap.release());
      return *this;
    }
  template <class _CS_S>
  auto_ptr& operator= (auto_ptr<_CS_S>& _CS_ap) throw()
    {
      if (&_CS_ap != this)
        reset(_CS_ap.release());
      return *this;
    }
  
  ~auto_ptr() throw() { delete _CS_m_ptr; }
  
  _CS_T& operator* () const throw() { return *_CS_m_ptr; }
  _CS_T* operator-> () const throw() { return _CS_m_ptr; }
  _CS_T* get() const throw() { return _CS_m_ptr; }
  _CS_T* release() throw()
    {
      _CS_T* _CS_rc = _CS_m_ptr;
      _CS_m_ptr = 0;
      return _CS_rc;
    }
  void reset(_CS_T* p) throw() { delete _CS_m_ptr; _CS_m_ptr = p; }
  
  template <class _CS_S>
  operator auto_ptr_ref<_CS_S> () throw() { return auto_ptr_ref<_CS_S>(*this); }
  
private:
  _CS_T* _CS_m_ptr;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_AUTO_PTR_H__ */
