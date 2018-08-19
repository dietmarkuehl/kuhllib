// -*-C++-*- include/testiterator.hpp
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
// Title:   
// Version: $Id: testiterator.hpp,v 1.1.1.1 2002/06/05 01:03:51 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(INCLUDE_TESTITERATOR_HPP)
#define INCLUDE_TESTITERATOR_HPP 1

#include <iterator>

#if defined(_ESTD_STD)
#  define STD _ESTD_STD
#elif defined(_CXXRT_STD)
#  define STD _CXXRT_STD
#else
#  define STD ::std::
#endif

namespace tst
{

  struct test_class
  {
  };

  // ------------------------------------------------------------------------

  template <class Category>
    struct print_iterator
    {
      typedef int difference_type;
      typedef Category iterator_category;
      typedef test_class value_type;
      typedef test_class* pointer;
      typedef test_class& reference;

      bool operator== (print_iterator const&) const { return false; }
      bool operator!= (print_iterator const&) const { return true; }
      bool operator< (print_iterator const&) const { return true; }
      bool operator<= (print_iterator const&) const { return true; }
      bool operator> (print_iterator const&) const { return true; }
      bool operator>= (print_iterator const&) const { return true; }
      
      test_class& operator[](int) { static test_class rc; return rc; }
      test_class& operator* () { static test_class rc; return rc; }
      test_class* operator->() { static test_class rc; return &rc; }
      
      print_iterator& operator++() { STD cout << "++ "; return *this; }
      print_iterator  operator++(int) { STD cout << "++p "; print_iterator rc(*this); operator++(); return rc; }
      print_iterator& operator--() { STD cout << "-- "; return *this; }
      print_iterator  operator--(int) { STD cout << "--p "; print_iterator rc(*this); operator--(); return rc; }
      
      print_iterator& operator+= (int) { STD cout << "+= "; return *this; }
      print_iterator& operator-= (int) { STD cout << "-= "; return *this; }
      
      int operator- (print_iterator const&) const { return 0; }
    };
  
  template <class Category>
    print_iterator<Category>
    operator+ (print_iterator<Category> it, int diff) { return it += diff; }
  
  template <class Category>
    print_iterator<Category>
    operator+ (int diff, print_iterator<Category> it) { return it += diff; }
  
  template <class Category>
    print_iterator<Category>
    operator- (print_iterator<Category> it, int diff) { return it += -diff; }

  // ------------------------------------------------------------------------

  template <class T, int sz>
    int size(T (&)[sz]) { return sz; }
  template <class T, int sz>
    T* begin(T (&array)[sz]) { return array; }
  template <class T, int sz>
    T* end(T (&array)[sz]) { return array + sz; }

  // ------------------------------------------------------------------------

  template <class T, class Diff = int>
    class input_iterator
    {
    public:
      typedef Diff                   difference_type;
      typedef STD input_iterator_tag iterator_category;
      typedef T                      value_type;
      typedef T*                     pointer;
      typedef T&                     reference;

      explicit input_iterator(T* ptr): m_ptr(ptr) {}
      input_iterator(input_iterator const& it): m_ptr(it.m_ptr) {}
      input_iterator& operator= (input_iterator const& it) { m_ptr = it.m_ptr; return *this; }
      ~input_iterator() {}

      bool operator== (input_iterator const& it) const { return m_ptr == it.m_ptr; }
      bool operator!= (input_iterator const& it) const { return m_ptr != it.m_ptr; }

      T& operator* () const { return *m_ptr; }
      T* operator->() const { return m_ptr; }

      input_iterator& operator++()    { ++m_ptr; return *this; }
      input_iterator  operator++(int) { return input_iterator(m_ptr++); }

    private:
      T* m_ptr;
    };

  template <class T, int sz>
    input_iterator<T> begin_input(T (&array)[sz]) { return input_iterator<T>(begin(array)); }
  template <class T, int sz>
    input_iterator<T> end_input(T (&array)[sz]) { return input_iterator<T>(end(array)); }

  // ------------------------------------------------------------------------

  template <class T, class Diff = int>
    class output_iterator
    {
    public:
      typedef void                    difference_type;
      typedef STD output_iterator_tag iterator_category;
      typedef void                    value_type;
      //typedef void                    pointer;
      //typedef void                    reference;

      explicit output_iterator(T* ptr): m_ptr(ptr) {}
      output_iterator(output_iterator const& it): m_ptr(it.m_ptr) {}
      output_iterator& operator= (output_iterator const& it) { m_ptr = it.m_ptr; return *this; }
      ~output_iterator() {}

      T& operator* () const { return *m_ptr; }

      output_iterator& operator++()    { ++m_ptr; return *this; }
      output_iterator  operator++(int) { return output_iterator(m_ptr++); }

    private:
      T* m_ptr;
    };

  template <class T, int sz>
    output_iterator<T> begin_output(T (&array)[sz]) { return output_iterator<T>(begin(array)); }

  // ------------------------------------------------------------------------

  template <class T, class Diff = int>
    class forward_iterator
    {
    public:
      typedef Diff                     difference_type;
      typedef STD forward_iterator_tag iterator_category;
      typedef T                        value_type;
      typedef T*                       pointer;
      typedef T&                       reference;

      explicit forward_iterator(T* ptr): m_ptr(ptr) {}
      forward_iterator(forward_iterator const& it): m_ptr(it.m_ptr) {}
      forward_iterator& operator= (forward_iterator const& it) { m_ptr = it.m_ptr; return *this; }
      ~forward_iterator() {}

      bool operator== (forward_iterator const& it) const { return m_ptr == it.m_ptr; }
      bool operator!= (forward_iterator const& it) const { return m_ptr != it.m_ptr; }

      T& operator* () const { return *m_ptr; }
      T* operator->() const { return m_ptr; }

      forward_iterator& operator++()    { ++m_ptr; return *this; }
      forward_iterator  operator++(int) { return forward_iterator(m_ptr++); }

    private:
      T* m_ptr;
    };

  template <class T, int sz>
    forward_iterator<T> begin_forward(T (&array)[sz]) { return forward_iterator<T>(begin(array)); }
  template <class T, int sz>
    forward_iterator<T> end_forward(T (&array)[sz]) { return forward_iterator<T>(end(array)); }

  // ------------------------------------------------------------------------

  template <class T, class Diff = int>
    class bidirectional_iterator
    {
    public:
      typedef Diff                           difference_type;
      typedef STD bidirectional_iterator_tag iterator_category;
      typedef T                              value_type;
      typedef T*                             pointer;
      typedef T&                             reference;

      explicit bidirectional_iterator(T* ptr): m_ptr(ptr) {}
      bidirectional_iterator(bidirectional_iterator const& it): m_ptr(it.m_ptr) {}
      bidirectional_iterator& operator= (bidirectional_iterator const& it) { m_ptr = it.m_ptr; return *this; }
      ~bidirectional_iterator() {}

      bool operator== (bidirectional_iterator const& it) const { return m_ptr == it.m_ptr; }
      bool operator!= (bidirectional_iterator const& it) const { return m_ptr != it.m_ptr; }

      T& operator* () const { return *m_ptr; }
      T* operator->() const { return m_ptr; }

      bidirectional_iterator& operator++()    { ++m_ptr; return *this; }
      bidirectional_iterator  operator++(int) { return bidirectional_iterator(m_ptr++); }
      bidirectional_iterator& operator--()    { --m_ptr; return *this; }
      bidirectional_iterator  operator--(int) { return bidirectional_iterator(m_ptr--); }

    private:
      T* m_ptr;
    };

  template <class T, int sz>
    bidirectional_iterator<T> begin_bidirectional(T (&array)[sz]) { return bidirectional_iterator<T>(begin(array)); }
  template <class T, int sz>
    bidirectional_iterator<T> end_bidirectional(T (&array)[sz]) { return bidirectional_iterator<T>(end(array)); }

  // ------------------------------------------------------------------------

  template <class T, class Diff = int>
    class random_access_iterator
    {
    public:
      typedef Diff                           difference_type;
      typedef STD random_access_iterator_tag iterator_category;
      typedef T                              value_type;
      typedef T*                             pointer;
      typedef T&                             reference;

      explicit random_access_iterator(T* ptr): m_ptr(ptr) {}
      random_access_iterator(random_access_iterator const& it): m_ptr(it.m_ptr) {}
      random_access_iterator& operator= (random_access_iterator const& it) { m_ptr = it.m_ptr; return *this; }
      ~random_access_iterator() {}

      bool operator== (random_access_iterator const& it) const { return m_ptr == it.m_ptr; }
      bool operator<= (random_access_iterator const& it) const { return m_ptr <= it.m_ptr; }
      bool operator< (random_access_iterator const& it) const { return m_ptr < it.m_ptr; }
      bool operator>= (random_access_iterator const& it) const { return m_ptr >= it.m_ptr; }
      bool operator> (random_access_iterator const& it) const { return m_ptr > it.m_ptr; }
      bool operator!= (random_access_iterator const& it) const { return m_ptr != it.m_ptr; }

      T& operator[] (difference_type n) const { return m_ptr[n]; }
      T& operator* () const { return *m_ptr; }
      T* operator->() const { return m_ptr; }

      random_access_iterator& operator++()    { ++m_ptr; return *this; }
      random_access_iterator  operator++(int) { return random_access_iterator(m_ptr++); }
      random_access_iterator& operator--()    { --m_ptr; return *this; }
      random_access_iterator  operator--(int) { return random_access_iterator(m_ptr--); }

      random_access_iterator& operator+= (difference_type n) { m_ptr += n; return *this; }
      random_access_iterator& operator-= (difference_type n) { m_ptr -= n; return *this; }

      difference_type operator- (random_access_iterator const& it) const { return m_ptr - it.m_ptr; }
							       
    private:
      T* m_ptr;
    };

  template <class T, class Diff>
    random_access_iterator<T, Diff> operator+ (Diff n, random_access_iterator<T, Diff> it)
    {
      return it += n;
    }

  template <class T, class Diff>
    random_access_iterator<T, Diff> operator+ (random_access_iterator<T, Diff> it, Diff n)
    {
      return it += n;
    }

  template <class T, class Diff>
    random_access_iterator<T, Diff> operator- (random_access_iterator<T, Diff> it, Diff n)
    {
      return it -= n;
    }

  template <class T, int sz>
    random_access_iterator<T> begin_random_access(T (&array)[sz]) { return random_access_iterator<T>(begin(array)); }
  template <class T, int sz>
    random_access_iterator<T> end_random_access(T (&array)[sz]) { return random_access_iterator<T>(end(array)); }


} // namespace tst

// --------------------------------------------------------------------------

#endif /* INCLUDE_TESTITERATOR_HPP */
