// -*-C++-*- testalloc.hpp
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
// Author: Dietmar Kuehl <http://www.dietmar-kuehl.de/>
// Title:  A test allocator
// Version: $Id: testalloc.hpp,v 1.1.1.1 2002/06/05 01:03:52 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(TESTALLOC_HPP)
#define TESTALLOC_HPP 1

#include <iostream>
#include <new>

namespace mystd
{
#if defined(_ESTD_STD)
#  define STD _ESTD_STD
  using STD size_t;
  using STD ptrdiff_t;
#elif defined(_CXXRT_STD)
#  define STD _CXXRT_STD
  using STD size_t;
  using STD ptrdiff_t;
#else
#  define STD ::std::
  using namespace std;
#endif
}

// -------------------------------------------------------------------------- 

namespace tst {

  bool set(bool& val) { return val = true; }

  bool used = false;
  int  allocs = 0;
  int  deallocs = 0;
  int  constructed = 0;
  int  destructed = 0;

  template <class T> class allocator;

  template <>
    class allocator<void>
    {
    public:
      typedef void*       pointer;
      typedef const void* const_pointer;
      typedef void        value_type;
      template <class U>
      struct rebind
      {
	typedef allocator<U> other;
      };
    };
  
  template <class T>
    class allocator
    {
    public:
      static void report()
	{
	  if (used)
	    {
	      if (allocs == deallocs && constructed == destructed)
		STD cout << "allocator successfully used\n";
	      else
		{
		  if (allocs < deallocs)
		    STD cout << "more memory released than allocated with allocator (" << allocs << " vs. " << deallocs << ")\n";
		  else if (allocs > deallocs)
		    STD cout << "memory leak when using allocator (" << allocs << " vs. " << deallocs << ")\n";
		  
		  if (constructed < destructed)
		    STD cout << "more objects destructed than constructed with allocator (" << constructed << " vs. " << destructed << ")\n";
		  else if (constructed > destructed)
		    STD cout << "objects not destructed with allocator (" << constructed << " vs. " << destructed << ")\n";
		}
	    }
	  else
	    STD cout << "allocator was never used\n";
	}

      typedef mystd::size_t    size_type;
      typedef mystd::ptrdiff_t difference_type;
      typedef T*        pointer;
      typedef T const*  const_pointer;
      typedef T&        reference;
      typedef T const&  const_reference;
      typedef T         value_type;

      template <class U>
      struct rebind
      {
	typedef allocator<U> other;
      };

      allocator(bool f = true) throw(): m_flag(f) {}
      allocator(allocator const& a) throw(): m_flag(a.m_flag) {}
      template <class U>
      allocator(allocator<U> const& a) throw(): m_flag(a.m_flag) {}
      ~allocator() throw()                         {}

      pointer address(reference ref) const             { return &ref; }
      const_pointer address(const_reference ref) const { return &ref; }

      pointer allocate(size_type sz, void const* = 0)
	{
	  if (m_flag && set(used))
	    ++allocs;
	  return static_cast<pointer>(operator new(sz));
	}
      void deallocate(pointer ptr, size_type)
	{
	  if (m_flag && set(used))
	    ++deallocs;
	  operator delete(ptr);
	}

      size_type max_size() const throw() { return size_type(-1); }
      
      void construct(pointer ptr, T const& obj)
	{
	  if (m_flag && set(used))
	    ++constructed;
	  new(ptr) T(obj);
	  
	}
      void destroy(pointer ptr)
	{
	  if (m_flag && set(used))
	    ++destructed;
	  ptr->~T();
	}

      bool m_flag;
    };

  // ------------------------------------------------------------------------

  template <class T1, class T2>
    bool
    operator== (allocator<T1> const&, allocator<T2> const&)
  {
    return true;
  }

  template <class T1, class T2>
    bool
    operator!= (allocator<T1> const&, allocator<T2> const&)
  {
    return false;
  }

}

// -------------------------------------------------------------------------- 

#endif /* TESTALLOC_HPP */
