/*
 * Copyright (c) 1997-1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */ 

#ifndef _CXXRT_STRING_H__
#define _CXXRT_STRING_H__

#include <stl_config.h>
#ifdef __STL_USE_NEW_IOSTREAMS
#include <char_traits.h>
#endif
#include <cctype>        
#include <stl_string_fwd.h>
#include <stl_uninitialized_copy.h>
#include <type_traits.h>
#include <stdexcept>      
#include <stl_iterator_base.h>
#include <stl_reverse_iterator.h>
#include <stl_algobase.h>
#include <stl_alloc.h>

#ifdef __STL_USE_NEW_IOSTREAMS
#include <iosfwd>
#else /* __STL_USE_NEW_IOSTREAMS */
#include <char_traits.h>
#endif /* __STL_USE_NEW_IOSTREAMS */

// Standard C++ string class.  This class has performance
// characteristics very much like vector<>, meaning, for example, that
// it does not perform reference-count or copy-on-write, and that
// concatenation of two strings is an O(N) operation. 

// There are three reasons why basic_string is not identical to
// vector.  First, basic_string always stores a null character at the
// end; this makes it possible for c_str to be a fast operation.
// Second, the C++ standard requires basic_string to copy elements
// using char_traits<>::assign, char_traits<>::copy, and
// char_traits<>::move.  This means that all of vector<>'s low-level
// operations must be rewritten.  Third, basic_string<> has a lot of
// extra functions in its interface that are convenient but, strictly
// speaking, redundant.

// Additionally, the C++ standard imposes a major restriction: according
// to the standard, the character type _CharT must be a POD type.  This
// implementation weakens that restriction, and allows _CharT to be a
// a user-defined non-POD type.  However, _CharT must still have a
// default constructor.

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#pragma set woff 1375
#endif

// ------------------------------------------------------------
// Class _String_base.  

// _String_base is a helper class that makes it it easier to write an
// exception-safe version of basic_string.  The constructor allocates,
// but does not initialize, a block of memory.  The destructor
// deallocates, but does not destroy elements within, a block of
// memory.  The destructor assumes that _M_start either is null, or else
// points to a block of memory that was allocated using _String_base's 
// allocator and whose size is _M_end_of_storage - _M_start.

// Additionally, _String_base encapsulates the difference between
// old SGI-style allocators and standard-conforming allocators.

#ifdef __STL_USE_STD_ALLOCATORS

// General base class.
template <class _Tp, class _Alloc, bool _S_instanceless>
class _String_alloc_base {
public:
  typedef typename _Alloc_traits<_Tp, _Alloc>::allocator_type allocator_type;
  allocator_type get_allocator() const { return _M_data_allocator; }

  _String_alloc_base(const allocator_type& __a)
    : _M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {}
  _String_alloc_base(_String_alloc_base const& __sab):
    _M_start(__sab._M_start),
    _M_finish(__sab._M_finish),
    _M_end_of_storage(__sab._M_end_of_storage)
  {}
  _String_alloc_base& operator= (_String_alloc_base const& __sab)
    {
      _M_start          = __sab._M_start;
      _M_finish         = __sab._M_finish;
      _M_end_of_storage = __sab._M_end_of_storage;
      return *this;
    }
  ~_String_alloc_base() {}

protected:
  _Tp* _M_allocate(size_t __n)
    { return _M_data_allocator.allocate(__n); }
  void _M_deallocate(_Tp* __p, size_t __n) {
    if (__p)
      _M_data_allocator.deallocate(__p, __n); 
  }

protected:
  allocator_type _M_data_allocator;

  _Tp* _M_start;
  _Tp* _M_finish;
  _Tp* _M_end_of_storage;
};

// Specialization for instanceless allocators.
template <class _Tp, class _Alloc>
class _String_alloc_base<_Tp,_Alloc,true> {
public:
  typedef typename _Alloc_traits<_Tp, _Alloc>::allocator_type allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

  _String_alloc_base(const allocator_type&)
    : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
  _String_alloc_base(_String_alloc_base const& __sab)
    : _M_start(__sab._M_start) , _M_finish(__sab._M_finish)
    , _M_end_of_storage(__sab._M_end_of_storage) {}
  _String_alloc_base& operator= (_String_alloc_base const& __sab)
    {
      _M_start = __sab._M_start;
      _M_finish = __sab._M_finish;
      _M_end_of_storage = __sab._M_end_of_storage;
      return *this;
    }

protected:
  typedef typename _Alloc_traits<_Tp, _Alloc>::_Alloc_type _Alloc_type;
  _Tp* _M_allocate(size_t __n)
    { return _Alloc_type::allocate(__n); }
  void _M_deallocate(_Tp* __p, size_t __n)
    { _Alloc_type::deallocate(__p, __n); }

protected:
  _Tp* _M_start;
  _Tp* _M_finish;
  _Tp* _M_end_of_storage;
};

template <class _Tp, class _Alloc>
class _String_base 
  : public _String_alloc_base<_Tp, _Alloc,
                              _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
{
protected:
  typedef _String_alloc_base<_Tp, _Alloc,
                             _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
          _Base;
  typedef typename _Base::allocator_type allocator_type;

  void _M_allocate_block(size_t __n) { 
    if (__n <= max_size()) {
      _Base::_M_start  = _Base::_M_allocate(__n);
      _Base::_M_finish = _Base::_M_start;
      _Base::_M_end_of_storage = _Base::_M_start + __n;
    }
    else
      _M_throw_length_error();
  }

  void _M_deallocate_block() 
    { _Base::_M_deallocate(_Base::_M_start, _Base::_M_end_of_storage - _Base::_M_start); }
  
  size_t max_size() const { return (size_t(-1) / sizeof(_Tp)) - 1; }

#if 0
//-dk_TODO
  _String_base(const allocator_type& __a) : _Base(__a) { }
  
  _String_base(const allocator_type& __a, size_t __n) : _Base(__a)
    { _M_allocate_block(__n); }
#else
  _String_base(const allocator_type& __a, size_t __n = 0) : _Base(__a)
    { if (__n != 0) _M_allocate_block(__n); }
#endif

  virtual ~_String_base() { _M_deallocate_block(); } //-dk:TODO think about the 'virtual'...

  void _M_throw_length_error() const;
  void _M_throw_out_of_range() const;
};

#else /* __STL_USE_STD_ALLOCATORS */

template <class _Tp, class _Alloc> class _String_base {
public:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

protected:
  typedef simple_alloc<_Tp, _Alloc> _Alloc_type;

  _Tp* _M_start;
  _Tp* _M_finish;
  _Tp* _M_end_of_storage;
                                // Precondition: 0 < __n <= max_size().

  _Tp* _M_allocate(size_t __n) { return _Alloc_type::allocate(__n); }
  void _M_deallocate(_Tp* __p, size_t __n) {
    if (__p)
      _Alloc_type::deallocate(__p, __n); 
  }

  void _M_allocate_block(size_t __n) { 
    if (__n <= max_size()) {
      _M_start  = _M_allocate(__n);
      _M_finish = _M_start;
      _M_end_of_storage = _M_start + __n;
    }
    else
      _M_throw_length_error();
  }

  void _M_deallocate_block() 
    { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }
  
  size_t max_size() const { return (size_t(-1) / sizeof(_Tp)) - 1; }

  _String_base(const allocator_type&)
    : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
  
  _String_base(const allocator_type&, size_t __n)
    : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    { _M_allocate_block(__n); }

  ~_String_base() { _M_deallocate_block(); }

  void _M_throw_length_error() const;
  void _M_throw_out_of_range() const;
};

#endif /* __STL_USE_STD_ALLOCATORS */

// ------------------------------------------------------------
// Class basic_string.  

// Class invariants:
// (1) [start, finish) is a valid range.
// (2) Each iterator in [start, finish) points to a valid object
//     of type value_type.
// (3) *finish is a valid object of type value_type; in particular,
//     it is value_type().
// (4) [finish + 1, end_of_storage) is a valid range.
// (5) Each iterator in [finish + 1, end_of_storage) points to 
//     unininitialized memory.

// Note one important consequence: a string of length n must manage
// a block of memory whose size is at least n + 1.  


template <class _CharT, class _Traits, class _Alloc> 
class basic_string : private _String_base<_CharT,_Alloc> {
public:
  typedef _CharT value_type;
  typedef _Traits traits_type;

  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef const value_type*                const_iterator;
  typedef value_type*                      iterator;

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator>       reverse_iterator;
#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */
  typedef reverse_iterator<const_iterator, value_type, const_reference, 
                           difference_type>  
          const_reverse_iterator;
  typedef reverse_iterator<iterator, value_type, reference, difference_type>
          reverse_iterator; 
#endif /* __STL_PARTIAL_SPECIALIZATION */

  static const size_type npos;

  typedef _String_base<_CharT,_Alloc> _Base;

public:                         // Constructor, destructor, assignment.
  typedef typename _Base::allocator_type allocator_type;
  allocator_type get_allocator() const { return _Base::get_allocator(); }

  explicit basic_string(const allocator_type& __a = allocator_type())
    : _Base(__a, 8) { _M_terminate_string(); }

  struct _Reserve_t {};
  basic_string(_Reserve_t, size_t __n,
               const allocator_type& __a = allocator_type())
    : _Base(__a, __n + 1) { _M_terminate_string(); }

  basic_string(const basic_string& __s) : _Base(__s.get_allocator()) 
    { _M_range_initialize(__s.begin(), __s.end()); }

  basic_string(const basic_string& __s, size_type __pos, size_type __n = npos,
               const allocator_type& __a = allocator_type()) 
    : _Base(__a) {
    if (__pos > __s.size())
      _M_throw_out_of_range();
    else
      _M_range_initialize(__s.begin() + __pos,
                          __s.begin() + __pos + min(__n, __s.size() - __pos));
  }

  basic_string(const _CharT* __s, size_type __n,
               const allocator_type& __a = allocator_type()) 
    : _Base(__a) 
    { _M_range_initialize(__s, __s + __n); }

  basic_string(const _CharT* __s,
               const allocator_type& __a = allocator_type())
    : _Base(__a) 
    { _M_range_initialize(__s, __s + _Traits::length(__s)); }

  basic_string(size_type __n, _CharT __c,
               const allocator_type& __a = allocator_type());

  // Check to see if _InputIterator is an integer type.  If so, then
  // it can't be an iterator.
#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  basic_string(_InputIterator __f, _InputIterator __l,
               const allocator_type& __a = allocator_type())
    : _Base(__a)
  {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_initialize_dispatch(__f, __l, _Integral());
  }
#else /* __STL_MEMBER_TEMPLATES */
  basic_string(const _CharT* __f, const _CharT* __l,
               const allocator_type& __a = allocator_type())
    : _Base(__a)
  {
    _M_range_initialize(__f, __l);
  }
#endif

  ~basic_string();
    
  basic_string& operator=(const basic_string& __s) {
    if (&__s != this) 
      assign(__s.begin(), __s.end());
    return *this;
  }

  basic_string& operator=(const _CharT* __s) 
    { assign(__s, __s + _Traits::length(__s)); return *this; }

  basic_string& operator=(_CharT __c)
    { assign(static_cast<size_type>(1), __c); return *this; }

private:                        // Protected members inherited from base.
#ifdef __STL_HAS_NAMESPACES
  using _Base::_M_allocate;
  using _Base::_M_deallocate;
  using _Base::_M_allocate_block;
  using _Base::_M_deallocate_block;
  using _Base::_M_throw_length_error;
  using _Base::_M_throw_out_of_range;

  using _Base::_M_start;
  using _Base::_M_finish;
  using _Base::_M_end_of_storage;
#endif /* __STL_HAS_NAMESPACES */

private:                        // Helper functions used by constructors
                                // and elsewhere.
  void _M_construct_null(_CharT* __p);
  static _CharT _M_null() {
#   ifndef __STL_DEFAULT_CONSTRUCTOR_BUG
    return _CharT();
#   else
    return (_CharT) 0;
#   endif
  }

private:                        
  // Helper functions used by constructors.  It is a severe error for
  // any of them to be called anywhere except from within constructors.

  void _M_terminate_string();

#ifdef __STL_MEMBER_TEMPLATES
    
  template <class _InputIter>
  void _M_range_initialize(_InputIter __f, _InputIter __l,
                           input_iterator_tag) {
    _M_allocate_block(8);
    _M_construct_null(_M_finish);
    __STL_TRY {
      append(__f, __l);
    }
    __STL_UNWIND(destroy(_M_start, _M_finish + 1));
  }

  template <class _ForwardIter>
  void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, 
                           forward_iterator_tag) {
    difference_type __n = 0;
    distance(__f, __l, __n);
    _M_allocate_block(__n + 1);
    _M_finish = uninitialized_copy(__f, __l, _M_start);
    _M_terminate_string();
  }

  template <class _InputIter>
  void _M_range_initialize(_InputIter __f, _InputIter __l) {
    typedef typename iterator_traits<_InputIter>::iterator_category _Category;
    _M_range_initialize(__f, __l, _Category());
  }

  template <class _Integer>
  void _M_initialize_dispatch(_Integer __n, _Integer __x, __true_type) {
    _M_allocate_block(__n + 1);
    _M_finish = uninitialized_fill_n(_M_start, __n, __x);
    _M_terminate_string();
  }

  template <class _InputIter>
  void _M_initialize_dispatch(_InputIter __f, _InputIter __l, __false_type) {
     _M_range_initialize(__f, __l);
  }
    
#else /* __STL_MEMBER_TEMPLATES */

  void _M_range_initialize(const _CharT* __f, const _CharT* __l) {
    ptrdiff_t __n = __l - __f;
    _M_allocate_block(__n + 1);
    _M_finish = uninitialized_copy(__f, __l, _M_start);
    _M_terminate_string();
  }

#endif /* __STL_MEMBER_TEMPLATES */

public:                         // Iterators.
  iterator begin()             { return _M_start; }
  iterator end()               { return _M_finish; }
  const_iterator begin() const { return _M_start; }
  const_iterator end()   const { return _M_finish; }  

  reverse_iterator rbegin()             
    { return reverse_iterator(_M_finish); }
  reverse_iterator rend()               
    { return reverse_iterator(_M_start); }
  const_reverse_iterator rbegin() const 
    { return const_reverse_iterator(_M_finish); }
  const_reverse_iterator rend()   const 
    { return const_reverse_iterator(_M_start); }

public:                         // Size, capacity, etc.
  size_type size() const { return _M_finish - _M_start; }
  size_type length() const { return size(); }

  size_t max_size() const { return _Base::max_size(); }


  void resize(size_type __n, _CharT __c) {
    if (__n <= size())
      erase(begin() + __n, end());
    else
      append(__n - size(), __c);
  }

  void resize(size_type __n) { resize(__n, _M_null()); }

  void reserve(size_type = 0);

  size_type capacity() const { return (_M_end_of_storage - _M_start) - 1; }

  void clear() {
    if (!empty()) {
      _Traits::assign(*_M_start, _M_null());
      destroy(_M_start+1, _M_finish+1);
      _M_finish = _M_start;
    }
  } 

  bool empty() const { return _M_start == _M_finish; }    

public:                         // Element access.

  const_reference operator[](size_type __n) const
    { return *(_M_start + __n); }
  reference operator[](size_type __n)
    { return *(_M_start + __n); }

  const_reference at(size_type __n) const {
    if (__n >= size())
      _M_throw_out_of_range();
    return *(_M_start + __n);
  }

  reference at(size_type __n) {
    if (__n >= size())
      _M_throw_out_of_range();
    return *(_M_start + __n);
  }

public:                         // Append, operator+=, push_back.

  basic_string& operator+=(const basic_string& __s) { return append(__s); }
  basic_string& operator+=(const _CharT* __s) { return append(__s); }
  basic_string& operator+=(_CharT __c) { push_back(__c); return *this; }

  basic_string& append(const basic_string& __s) 
    { return append(__s.begin(), __s.end()); }

  basic_string& append(const basic_string& __s,
                       size_type __pos, size_type __n)
  {
    if (__pos > __s.size())
      _M_throw_out_of_range();
    return append(__s.begin() + __pos,
                  __s.begin() + __pos + min(__n, __s.size() - __pos));
  }

  basic_string& append(const _CharT* __s, size_type __n) 
    { return append(__s, __s+__n); }

  basic_string& append(const _CharT* __s) 
    { return append(__s, __s + _Traits::length(__s)); }

  basic_string& append(size_type __n, _CharT __c);

#ifdef __STL_MEMBER_TEMPLATES

  // Check to see if _InputIterator is an integer type.  If so, then
  // it can't be an iterator.
  template <class _InputIter>
  basic_string& append(_InputIter __first, _InputIter __last) {
    typedef typename _Is_integer<_InputIter>::_Integral _Integral;
    return _M_append_dispatch(__first, __last, _Integral());
  }

#else /* __STL_MEMBER_TEMPLATES */

  basic_string& append(const _CharT* __first, const _CharT* __last);

#endif /* __STL_MEMBER_TEMPLATES */

  void push_back(_CharT __c) {
    if (_M_finish + 1 == _M_end_of_storage)
      reserve(size() + max(size(), static_cast<size_type>(1)));
    // _M_construct_null(_M_finish + 1);
    _Traits::assign(*_M_finish, __c);
    ++_M_finish;
  }

  void pop_back() {
    _Traits::assign(*(_M_finish - 1), _M_null());
    destroy(_M_finish);
    --_M_finish;
  }

private:                        // Helper functions for append.

#ifdef __STL_MEMBER_TEMPLATES

  template <class _InputIter>
  basic_string& append(_InputIter __f, _InputIter __l, input_iterator_tag);

  template <class _ForwardIter>
  basic_string& append(_ForwardIter __f, _ForwardIter __l, 
                       forward_iterator_tag);

  template <class _Integer>
  basic_string& _M_append_dispatch(_Integer __n, _Integer __x, __true_type) {
    return append((size_type) __n, (_CharT) __x);
  }

  template <class _InputIter>
  basic_string& _M_append_dispatch(_InputIter __f, _InputIter __l,
                                   __false_type) {
    typedef typename iterator_traits<_InputIter>::iterator_category _Category;
    return append(__f, __l, _Category());
  }

#endif /* __STL_MEMBER_TEMPLATES */

public:                         // Assign
  
  basic_string& assign(const basic_string& __s) 
    { return assign(__s.begin(), __s.end()); }

  basic_string& assign(const basic_string& __s, 
                       size_type __pos, size_type __n) {
    if (__pos > __s.size())
      _M_throw_out_of_range();
    return assign(__s.begin() + __pos, 
                  __s.begin() + __pos + min(__n, __s.size() - __pos));
  }

  basic_string& assign(const _CharT* __s, size_type __n)
    { return assign(__s, __s + __n); }

  basic_string& assign(const _CharT* __s)
    { return assign(__s, __s + _Traits::length(__s)); }

  basic_string& assign(size_type __n, _CharT __c);

#ifdef __STL_MEMBER_TEMPLATES

  // Check to see if _InputIterator is an integer type.  If so, then
  // it can't be an iterator.
  template <class _InputIter>
  basic_string& assign(_InputIter __first, _InputIter __last) {
    typedef typename _Is_integer<_InputIter>::_Integral _Integral;
    return _M_assign_dispatch(__first, __last, _Integral());
  }

#endif  /* __STL_MEMBER_TEMPLATES */

  basic_string& assign(const _CharT* __f, const _CharT* __l);

private:                        // Helper functions for assign.

#ifdef __STL_MEMBER_TEMPLATES

  template <class _Integer>
  basic_string& _M_assign_dispatch(_Integer __n, _Integer __x, __true_type) {
    return assign((size_type) __n, (_CharT) __x);
  }

  template <class _InputIter>
  basic_string& _M_assign_dispatch(_InputIter __f, _InputIter __l,
                                   __false_type);

#endif  /* __STL_MEMBER_TEMPLATES */

public:                         // Insert

  basic_string& insert(size_type __pos, const basic_string& __s) {
    if (__pos > size())
      _M_throw_out_of_range();
    if (size() > max_size() - __s.size())
      _M_throw_length_error();
    insert(_M_start + __pos, __s.begin(), __s.end());
    return *this;
  }

  basic_string& insert(size_type __pos, const basic_string& __s,
                       size_type __beg, size_type __n) {
    if (__pos > size() || __beg > __s.size())
      _M_throw_out_of_range();
    size_type __len = min(__n, __s.size() - __beg);
    if (size() > max_size() - __len)
      _M_throw_length_error();
    insert(_M_start + __pos,
           __s.begin() + __beg, __s.begin() + __beg + __len);
    return *this;
  }

  basic_string& insert(size_type __pos, const _CharT* __s, size_type __n) {
    if (__pos > size())
      _M_throw_out_of_range();
    if (size() > max_size() - __n)
      _M_throw_length_error();
    insert(_M_start + __pos, __s, __s + __n);
    return *this;
  }

  basic_string& insert(size_type __pos, const _CharT* __s) {
    if (__pos > size())
      _M_throw_out_of_range();
    size_type __len = _Traits::length(__s);
    if (size() > max_size() - __len)
      _M_throw_length_error();
    insert(_M_start + __pos, __s, __s + __len);
    return *this;
  }
    
  basic_string& insert(size_type __pos, size_type __n, _CharT __c) {
    if (__pos > size())
      _M_throw_out_of_range();
    if (size() > max_size() - __n)
      _M_throw_length_error();
    insert(_M_start + __pos, __n, __c);
    return *this;
  }

  iterator insert(iterator __p, _CharT __c) {
    if (__p == _M_finish) {
      push_back(__c);
      return _M_finish - 1;
    }
    else
      return _M_insert_aux(__p, __c);
  }

  void insert(iterator __p, size_t __n, _CharT __c);

#ifdef __STL_MEMBER_TEMPLATES

  // Check to see if _InputIterator is an integer type.  If so, then
  // it can't be an iterator.
  template <class _InputIter>
  void insert(iterator __p, _InputIter __first, _InputIter __last) {
    typedef typename _Is_integer<_InputIter>::_Integral _Integral;
    _M_insert_dispatch(__p, __first, __last, _Integral());
  }

#else /* __STL_MEMBER_TEMPLATES */

  void insert(iterator __p, const _CharT* __first, const _CharT* __last);

#endif /* __STL_MEMBER_TEMPLATES */

private:                        // Helper functions for insert.

#ifdef __STL_MEMBER_TEMPLATES

  template <class _InputIter>
  void insert(iterator __p, _InputIter, _InputIter, input_iterator_tag);

  template <class _ForwardIter>
  void insert(iterator __p, _ForwardIter, _ForwardIter, forward_iterator_tag);


  template <class _Integer>
  void _M_insert_dispatch(iterator __p, _Integer __n, _Integer __x,
                          __true_type) {
    insert(__p, (size_type) __n, (_CharT) __x);
  }

  template <class _InputIter>
  void _M_insert_dispatch(iterator __p, _InputIter __first, _InputIter __last,
                          __false_type) {
    typedef typename iterator_traits<_InputIter>::iterator_category _Category;
    insert(__p, __first, __last, _Category());
  }

  template <class _InputIterator>
  void 
  _M_copy(_InputIterator __first, _InputIterator __last, iterator __result) {
    for ( ; __first != __last; ++__first, ++__result)
      _Traits::assign(*__result, *__first);
  }

#endif /* __STL_MEMBER_TEMPLATES */

  iterator _M_insert_aux(iterator, _CharT);

  void 
  _M_copy(const _CharT* __first, const _CharT* __last, _CharT* __result) {
    _Traits::copy(__result, __first, __last - __first);
  }

public:                         // Erase.

  basic_string& erase(size_type __pos = 0, size_type __n = npos) {
    if (__pos > size())
      _M_throw_out_of_range();
    erase(_M_start + __pos, _M_start + __pos + min(__n, size() - __pos));
    return *this;
  }  

  iterator erase(iterator __position) {
                                // The move includes the terminating null.
    _Traits::move(__position, __position + 1, _M_finish - __position);
    destroy(_M_finish);
    --_M_finish;
    return __position;
  }

  iterator erase(iterator __first, iterator __last) {
    if (__first != __last) {
                                // The move includes the terminating null.
      _Traits::move(__first, __last, (_M_finish - __last) + 1);
      const iterator __new_finish = _M_finish - (__last - __first);
      destroy(__new_finish + 1, _M_finish + 1);
      _M_finish = __new_finish;
    }
    return __first;
  }

public:                         // Replace.  (Conceptually equivalent
                                // to erase followed by insert.)
  basic_string& replace(size_type __pos, size_type __n, 
                        const basic_string& __s) {
    if (__pos > size())
      _M_throw_out_of_range();
    const size_type __len = min(__n, size() - __pos);
    if (size() - __len >= max_size() - __s.size())
      _M_throw_length_error();
    return replace(_M_start + __pos, _M_start + __pos + __len, 
                   __s.begin(), __s.end());
  }

  basic_string& replace(size_type __pos1, size_type __n1,
                        const basic_string& __s,
                        size_type __pos2, size_type __n2) {
    if (__pos1 > size() || __pos2 > __s.size())
      _M_throw_out_of_range();
    const size_type __len1 = min(__n1, size() - __pos1);
    const size_type __len2 = min(__n2, __s.size() - __pos2);
    if (size() - __len1 >= max_size() - __len2)
      _M_throw_length_error();
    return replace(_M_start + __pos1, _M_start + __pos1 + __len1,
                   __s._M_start + __pos2, __s._M_start + __pos2 + __len2);
  }

  basic_string& replace(size_type __pos, size_type __n1,
                        const _CharT* __s, size_type __n2) {
    if (__pos > size())
      _M_throw_out_of_range();
    const size_type __len = min(__n1, size() - __pos);
    if (__n2 > max_size() || size() - __len >= max_size() - __n2)
      _M_throw_length_error();
    return replace(_M_start + __pos, _M_start + __pos + __len,
                   __s, __s + __n2);
  }

  basic_string& replace(size_type __pos, size_type __n1,
                        const _CharT* __s) {
    if (__pos > size())
      _M_throw_out_of_range();
    const size_type __len = min(__n1, size() - __pos);
    const size_type __n2 = _Traits::length(__s);
    if (__n2 > max_size() || size() - __len >= max_size() - __n2)
      _M_throw_length_error();
    return replace(_M_start + __pos, _M_start + __pos + __len,
                   __s, __s + _Traits::length(__s));
  }

  basic_string& replace(size_type __pos, size_type __n1,
                        size_type __n2, _CharT __c) {
    if (__pos > size())
      _M_throw_out_of_range();
    const size_type __len = min(__n1, size() - __pos);
    if (__n2 > max_size() || size() - __len >= max_size() - __n2)
      _M_throw_length_error();
    return replace(_M_start + __pos, _M_start + __pos + __len, __n2, __c);
  }

  basic_string& replace(iterator __first, iterator __last, 
                        const basic_string& __s) 
    { return replace(__first, __last, __s.begin(), __s.end()); }

  basic_string& replace(iterator __first, iterator __last,
                        const _CharT* __s, size_type __n) 
    { return replace(__first, __last, __s, __s + __n); }

  basic_string& replace(iterator __first, iterator __last,
                        const _CharT* __s) {
    return replace(__first, __last, __s, __s + _Traits::length(__s));
  }

  basic_string& replace(iterator __first, iterator __last, 
                        size_type __n, _CharT __c);

  // Check to see if _InputIterator is an integer type.  If so, then
  // it can't be an iterator.
#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIter>
  basic_string& replace(iterator __first, iterator __last,
                        _InputIter __f, _InputIter __l) {
    typedef typename _Is_integer<_InputIter>::_Integral _Integral;
    return _M_replace_dispatch(__first, __last, __f, __l,  _Integral());
  }
#else /* __STL_MEMBER_TEMPLATES */
  basic_string& replace(iterator __first, iterator __last,
                        const _CharT* __f, const _CharT* __l);
#endif /* __STL_MEMBER_TEMPLATES */

private:                        // Helper functions for replace.

#ifdef __STL_MEMBER_TEMPLATES

  template <class _Integer>
  basic_string& _M_replace_dispatch(iterator __first, iterator __last,
                                    _Integer __n, _Integer __x,
                                    __true_type) {
    return replace(__first, __last, (size_type) __n, (_CharT) __x);
  }

  template <class _InputIter>
  basic_string& _M_replace_dispatch(iterator __first, iterator __last,
                                    _InputIter __f, _InputIter __l,
                                    __false_type) {
    typedef typename iterator_traits<_InputIter>::iterator_category _Category;
    return replace(__first, __last, __f, __l, _Category());
  }

  template <class _InputIter>
  basic_string& replace(iterator __first, iterator __last,
                        _InputIter __f, _InputIter __l, input_iterator_tag);

  template <class _ForwardIter>
  basic_string& replace(iterator __first, iterator __last,
                        _ForwardIter __f, _ForwardIter __l, 
                        forward_iterator_tag);

#endif /* __STL_MEMBER_TEMPLATES */

public:                         // Other modifier member functions.

  size_type copy(_CharT* __s, size_type __n, size_type __pos = 0) const {
    if (__pos > size())
      _M_throw_out_of_range();
    const size_type __len = min(__n, size() - __pos);
    _Traits::copy(__s, _M_start + __pos, __len);
    return __len;
  }

  void swap(basic_string& __s) {
    __STD::swap(_M_start, __s._M_start);
    __STD::swap(_M_finish, __s._M_finish);
    __STD::swap(_M_end_of_storage, __s._M_end_of_storage);
  }

public:                         // Conversion to C string.

  const _CharT* c_str() const { return _M_start; }
  const _CharT* data()  const { return _M_start; }

public:                         // find.

  size_type find(const basic_string& __s, size_type __pos = 0) const 
    { return find(__s.begin(), __pos, __s.size()); }

  size_type find(const _CharT* __s, size_type __pos = 0) const 
    { return find(__s, __pos, _Traits::length(__s)); }

  size_type find(const _CharT* __s, size_type __pos, size_type __n) const;
  size_type find(_CharT __c, size_type __pos = 0) const;

public:                         // rfind.

  size_type rfind(const basic_string& __s, size_type __pos = npos) const 
    { return rfind(__s.begin(), __pos, __s.size()); }

  size_type rfind(const _CharT* __s, size_type __pos = npos) const 
    { return rfind(__s, __pos, _Traits::length(__s)); }

  size_type rfind(const _CharT* __s, size_type __pos, size_type __n) const;
  size_type rfind(_CharT __c, size_type __pos = npos) const;

public:                         // find_first_of
  
  size_type find_first_of(const basic_string& __s, size_type __pos = 0) const 
    { return find_first_of(__s.begin(), __pos, __s.size()); }

  size_type find_first_of(const _CharT* __s, size_type __pos = 0) const 
    { return find_first_of(__s, __pos, _Traits::length(__s)); }

  size_type find_first_of(const _CharT* __s, size_type __pos, 
                          size_type __n) const;

  size_type find_first_of(_CharT __c, size_type __pos = 0) const 
    { return find(__c, __pos); }

public:                         // find_last_of

  size_type find_last_of(const basic_string& __s,
                         size_type __pos = npos) const
    { return find_last_of(__s.begin(), __pos, __s.size()); }

  size_type find_last_of(const _CharT* __s, size_type __pos = npos) const 
    { return find_last_of(__s, __pos, _Traits::length(__s)); }

  size_type find_last_of(const _CharT* __s, size_type __pos, 
                         size_type __n) const;

  size_type find_last_of(_CharT __c, size_type __pos = npos) const {
    return rfind(__c, __pos);
  }

public:                         // find_first_not_of

  size_type find_first_not_of(const basic_string& __s, 
                              size_type __pos = 0) const 
    { return find_first_not_of(__s.begin(), __pos, __s.size()); }

  size_type find_first_not_of(const _CharT* __s, size_type __pos = 0) const 
    { return find_first_not_of(__s, __pos, _Traits::length(__s)); }

  size_type find_first_not_of(const _CharT* __s, size_type __pos,
                              size_type __n) const;

  size_type find_first_not_of(_CharT __c, size_type __pos = 0) const;

public:                         // find_last_not_of

  size_type find_last_not_of(const basic_string& __s, 
                             size_type __pos = npos) const
    { return find_last_not_of(__s.begin(), __pos, __s.size()); }

  size_type find_last_not_of(const _CharT* __s, size_type __pos = npos) const
    { return find_last_not_of(__s, __pos, _Traits::length(__s)); }

  size_type find_last_not_of(const _CharT* __s, size_type __pos,
                             size_type __n) const;

  size_type find_last_not_of(_CharT __c, size_type __pos = npos) const;

public:                         // Substring.

  basic_string substr(size_type __pos = 0, size_type __n = npos) const {
    if (__pos > size())
      _M_throw_out_of_range();
    return basic_string(_M_start + __pos, 
                        _M_start + __pos + min(__n, size() - __pos));
  }

public:                         // Compare

  int compare(const basic_string& __s) const 
    { return _M_compare(_M_start, _M_finish, __s._M_start, __s._M_finish); }

  int compare(size_type __pos1, size_type __n1,
              const basic_string& __s) const {
    if (__pos1 > size())
      _M_throw_out_of_range();
    return _M_compare(_M_start + __pos1, 
                      _M_start + __pos1 + min(__n1, size() - __pos1),
                      __s._M_start, __s._M_finish);
  }
    
  int compare(size_type __pos1, size_type __n1,
              const basic_string& __s,
              size_type __pos2, size_type __n2) const {
    if (__pos1 > size() || __pos2 > __s.size())
      _M_throw_out_of_range();
    return _M_compare(_M_start + __pos1, 
                      _M_start + __pos1 + min(__n1, size() - __pos1),
                      __s._M_start + __pos2, 
                      __s._M_start + __pos2 + min(__n2, size() - __pos2));
  }

  int compare(const _CharT* __s) const {
    return _M_compare(_M_start, _M_finish, __s, __s + _Traits::length(__s));
  }

  int compare(size_type __pos1, size_type __n1, const _CharT* __s) const {
    if (__pos1 > size())
      _M_throw_out_of_range();
    return _M_compare(_M_start + __pos1, 
                      _M_start + __pos1 + min(__n1, size() - __pos1),
                      __s, __s + _Traits::length(__s));
  }

  int compare(size_type __pos1, size_type __n1, const _CharT* __s,
              size_type __n2) const {
    if (__pos1 > size())
      _M_throw_out_of_range();
    return _M_compare(_M_start + __pos1, 
                      _M_start + __pos1 + min(__n1, size() - __pos1),
                      __s, __s + __n2);
  }

public:                        // Helper function for compare.
  static int _M_compare(const _CharT* __f1, const _CharT* __l1,
                        const _CharT* __f2, const _CharT* __l2) {
    const ptrdiff_t __n1 = __l1 - __f1;
    const ptrdiff_t __n2 = __l2 - __f2;
    const int cmp = _Traits::compare(__f1, __f2, min(__n1, __n2));
    return cmp != 0 ? cmp : (__n1 < __n2 ? -1 : (__n1 > __n2 ? 1 : 0));
  }
};



// ------------------------------------------------------------
// Non-inline declarations.

#ifdef __STL_MEMBER_TEMPLATES

template <class _Tp, class _Traits, class _Alloc> 
template <class _InputIterator>
basic_string<_Tp, _Traits, _Alloc>& 
basic_string<_Tp, _Traits, _Alloc>::append(_InputIterator __first, 
                                          _InputIterator __last,
                                          input_iterator_tag) {
  for ( ; __first != __last ; ++__first)
    push_back(*__first);
  return *this;
}

template <class _Tp, class _Traits, class _Alloc> 
template <class _ForwardIter>
basic_string<_Tp, _Traits, _Alloc>& 
basic_string<_Tp, _Traits, _Alloc>::append(_ForwardIter __first, 
                                           _ForwardIter __last,
                                           forward_iterator_tag) {
  if (__first != __last) {
    const size_type __old_size = size();
    difference_type __n = 0;
    distance(__first, __last, __n);
    if (static_cast<size_type>(__n) > max_size() ||
        __old_size > max_size() - static_cast<size_type>(__n))
      _M_throw_length_error();
    if (__old_size + static_cast<size_type>(__n) > capacity()) {
      const size_type __len = __old_size +
                            max(__old_size, static_cast<size_type>(__n)) + 1;
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      __STL_TRY {
        __new_finish = uninitialized_copy(_M_start, _M_finish, __new_start);
        __new_finish = uninitialized_copy(__first, __last, __new_finish);
        _M_construct_null(__new_finish);
      }
      __STL_UNWIND((destroy(__new_start,__new_finish),
                    _M_deallocate(__new_start,__len)));
      destroy(_M_start, _M_finish + 1);
      _M_deallocate_block();
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len; 
    }
    else {
      _ForwardIter __f1 = __first;
      ++__f1;
      uninitialized_copy(__f1, __last, _M_finish + 1);
      __STL_TRY {
        _M_construct_null(_M_finish + __n);
      }
      __STL_UNWIND(destroy(_M_finish + 1, _M_finish + __n));
      _Traits::assign(*_M_finish, *__first);
      _M_finish += __n;
    }
  }
  return *this;  
}

#else /* __STL_MEMBER_TEMPLATES */

template <class _Tp, class _Traits, class _Alloc> 
basic_string<_Tp, _Traits, _Alloc>& 
basic_string<_Tp, _Traits, _Alloc>::append(const _Tp* __first,
                                           const _Tp* __last)
{
  if (__first != __last) {
    const size_type __old_size = size();
    ptrdiff_t __n = __last - __first;
    if (__n > max_size() || __old_size > max_size() - __n)
      _M_throw_length_error();
    if (__old_size + __n > capacity()) {
      const size_type __len = __old_size + max(__old_size, (size_t) __n) + 1;
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      __STL_TRY {
        __new_finish = uninitialized_copy(_M_start, _M_finish, __new_start);
        __new_finish = uninitialized_copy(__first, __last, __new_finish);
        _M_construct_null(__new_finish);
      }
      __STL_UNWIND((destroy(__new_start,__new_finish),
                    _M_deallocate(__new_start,__len)));
      destroy(_M_start, _M_finish + 1);
      _M_deallocate_block();
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len; 
    }
    else {
      const _Tp* __f1 = __first;
      ++__f1;
      uninitialized_copy(__f1, __last, _M_finish + 1);
      __STL_TRY {
        _M_construct_null(_M_finish + __n);
      }
      __STL_UNWIND(destroy(_M_finish + 1, _M_finish + __n));
      _Traits::assign(*_M_finish, *__first);
      _M_finish += __n;
    }
  }
  return *this;  
}

#endif /* __STL_MEMBER_TEMPLATES */

#ifdef __STL_MEMBER_TEMPLATES

template <class _CharT, class _Traits, class _Alloc> 
template <class _InputIter>
basic_string<_CharT,_Traits,_Alloc>& basic_string<_CharT,_Traits,_Alloc>
  ::_M_assign_dispatch(_InputIter __f, _InputIter __l, __false_type)
{
  pointer __cur = _M_start;
  while (__f != __l && __cur != _M_finish) {
    _Traits::assign(*__cur, *__f);
    ++__f;
    ++__cur;
  }
  if (__f == __l)
    erase(__cur, _M_finish);
  else
    append(__f, __l);
  return *this;
}

#endif /* __STL_MEMBER_TEMPLATES */

#ifdef __STL_MEMBER_TEMPLATES

template <class _Tp, class _Traits, class _Alloc>
template <class _InputIter>
void basic_string<_Tp, _Traits, _Alloc>::insert(iterator __p,
                                                _InputIter __first, 
                                                _InputIter __last,
                                                input_iterator_tag)
{
  for ( ; __first != __last; ++__first) {
    __p = insert(__p, *__first);
    ++__p;
  }
}

template <class _CharT, class _Traits, class _Alloc>
template <class _ForwardIter>
void 
basic_string<_CharT,_Traits,_Alloc>::insert(iterator __position,
                                            _ForwardIter __first, 
                                            _ForwardIter __last,
                                            forward_iterator_tag)
{
  if (__first != __last) {
    difference_type __n = 0;
    distance(__first, __last, __n);
    if (_M_end_of_storage - _M_finish >= __n + 1) {
      const difference_type __elems_after = _M_finish - __position;
      iterator __old_finish = _M_finish;
      if (__elems_after >= __n) {
        uninitialized_copy((_M_finish - __n) + 1, _M_finish + 1,
                           _M_finish + 1);
        _M_finish += __n;
        _Traits::move(__position + __n,
                      __position, (__elems_after - __n) + 1);
        _M_copy(__first, __last, __position);
      }
      else {
        _ForwardIter __mid = __first;
        advance(__mid, __elems_after + 1);
        uninitialized_copy(__mid, __last, _M_finish + 1);
        _M_finish += __n - __elems_after;
        __STL_TRY {
          uninitialized_copy(__position, __old_finish + 1, _M_finish);
          _M_finish += __elems_after;
        }
        __STL_UNWIND((destroy(__old_finish + 1, _M_finish), 
                      _M_finish = __old_finish));
        _M_copy(__first, __mid, __position);
      }
    }
    else {
      const size_type __old_size = size();        
      const size_type __len
        = __old_size + max(__old_size, static_cast<size_type>(__n)) + 1;
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      __STL_TRY {
        __new_finish = uninitialized_copy(_M_start, __position, __new_start);
        __new_finish = uninitialized_copy(__first, __last, __new_finish);
        __new_finish
          = uninitialized_copy(__position, _M_finish, __new_finish);
        _M_construct_null(__new_finish);
      }
      __STL_UNWIND((destroy(__new_start,__new_finish),
                    _M_deallocate(__new_start,__len)));
      destroy(_M_start, _M_finish + 1);
      _M_deallocate_block();
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len; 
    }
  }
}

#else /* __STL_MEMBER_TEMPLATES */

template <class _CharT, class _Traits, class _Alloc>
void 
basic_string<_CharT,_Traits,_Alloc>::insert(iterator __position,
                                            const _CharT* __first, 
                                            const _CharT* __last)
{
  if (__first != __last) {
    const ptrdiff_t __n = __last - __first;
    if (_M_end_of_storage - _M_finish >= __n + 1) {
      const ptrdiff_t __elems_after = _M_finish - __position;
      iterator __old_finish = _M_finish;
      if (__elems_after >= __n) {
        uninitialized_copy((_M_finish - __n) + 1, _M_finish + 1,
                           _M_finish + 1);
        _M_finish += __n;
        _Traits::move(__position + __n,
                      __position, (__elems_after - __n) + 1);
        _M_copy(__first, __last, __position);
      }
      else {
        const _CharT* __mid = __first;
        advance(__mid, __elems_after + 1);
        uninitialized_copy(__mid, __last, _M_finish + 1);
        _M_finish += __n - __elems_after;
        __STL_TRY {
          uninitialized_copy(__position, __old_finish + 1, _M_finish);
          _M_finish += __elems_after;
        }
        __STL_UNWIND((destroy(__old_finish + 1, _M_finish), 
                      _M_finish = __old_finish));
        _M_copy(__first, __mid, __position);
      }
    }
    else {
      const size_type __old_size = size();        
      const size_type __len
        = __old_size + max(__old_size, static_cast<size_type>(__n)) + 1;
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      __STL_TRY {
        __new_finish = uninitialized_copy(_M_start, __position, __new_start);
        __new_finish = uninitialized_copy(__first, __last, __new_finish);
        __new_finish
          = uninitialized_copy(__position, _M_finish, __new_finish);
        _M_construct_null(__new_finish);
      }
      __STL_UNWIND((destroy(__new_start,__new_finish),
                    _M_deallocate(__new_start,__len)));
      destroy(_M_start, _M_finish + 1);
      _M_deallocate_block();
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len; 
    }
  }
}

#endif /* __STL_MEMBER_TEMPLATES */

#ifdef __STL_MEMBER_TEMPLATES

template <class _CharT, class _Traits, class _Alloc>
template <class _InputIter>
basic_string<_CharT,_Traits,_Alloc>&
basic_string<_CharT,_Traits,_Alloc>
  ::replace(iterator __first, iterator __last, _InputIter __f, _InputIter __l,
            input_iterator_tag) 
{
  for ( ; __first != __last && __f != __l; ++__first, ++__f)
    _Traits::assign(*__first, *__f);

  if (__f == __l)
    erase(__first, __last);
  else
    insert(__last, __f, __l);
  return *this;
}

template <class _CharT, class _Traits, class _Alloc>
template <class _ForwardIter>
basic_string<_CharT,_Traits,_Alloc>&
basic_string<_CharT,_Traits,_Alloc>
  ::replace(iterator __first, iterator __last,
            _ForwardIter __f, _ForwardIter __l,
            forward_iterator_tag) 
{
  difference_type __n = 0;
  distance(__f, __l, __n);
  const difference_type __len = __last - __first;
  if (__len >= __n) {
    _M_copy(__f, __l, __first);
    erase(__first + __n, __last);
  }
  else {
    _ForwardIter __m = __f;
    advance(__m, __len);
    _M_copy(__f, __m, __first);
    insert(__last, __m, __l);
  }
  return *this;
}

#else /* __STL_MEMBER_TEMPLATES */

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT,_Traits,_Alloc>&
basic_string<_CharT,_Traits,_Alloc>
  ::replace(iterator __first, iterator __last,
            const _CharT* __f, const _CharT* __l)
{
  const ptrdiff_t         __n = __l - __f;
  const difference_type __len = __last - __first;
  if (__len >= __n) {
    _M_copy(__f, __l, __first);
    erase(__first + __n, __last);
  }
  else {
    const _CharT* __m = __f + __len;
    _M_copy(__f, __m, __first);
    insert(__last, __m, __l);
  }
  return *this;
}

#endif /* __STL_MEMBER_TEMPLATES */

// ------------------------------------------------------------
// Non-member functions.

// Operator+

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const _CharT* __s,
          const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(_CharT __c,
          const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT* __s);

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT __c);

// Operator== and operator!=

template <class _CharT, class _Traits, class _Alloc>
bool
operator==(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
bool
operator==(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
bool
operator==(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s);

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator!=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator!=(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator!=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s);

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

// Operator< (and also >, <=, and >=).

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<(const _CharT* __s,
          const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT* __s);

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>(const _CharT* __s,
          const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT* __s);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<=(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>=(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y);

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s);

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

// Swap.

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _CharT, class _Traits, class _Alloc>
inline void swap(basic_string<_CharT,_Traits,_Alloc>& __x,
                 basic_string<_CharT,_Traits,_Alloc>& __y) {
  __x.swap(__y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

// I/O.  

__STL_END_NAMESPACE
#ifdef __STL_USE_NEW_IOSTREAMS 
#include <istream>
#include <ostream>
#include <cxxrt/ctype.h>
#include <cxxrt/streambuf.h>
#else
#include <iostream.h>
#endif /* __STL_USE_NEW_IOSTREAMS */
__STL_BEGIN_NAMESPACE

#ifdef __STL_USE_NEW_IOSTREAMS

template <class _CharT, class _Traits, class _Alloc>
basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, 
           const basic_string<_CharT,_Traits,_Alloc>& __s);

template <class _CharT, class _Traits, class _Alloc>
basic_istream<_CharT, _Traits>& 
operator>>(basic_istream<_CharT, _Traits>& __is,
           basic_string<_CharT,_Traits,_Alloc>& __s);

template <class _CharT, class _Traits, class _Alloc>    
basic_istream<_CharT, _Traits>& 
getline(basic_istream<_CharT, _Traits>& __is,
        basic_string<_CharT,_Traits,_Alloc>& __s,
        _CharT __delim);

template <class _CharT, class _Traits, class _Alloc>    
basic_istream<_CharT, _Traits>& 
getline(basic_istream<_CharT, _Traits>& __is,
        basic_string<_CharT,_Traits,_Alloc>& __s);

#else /* __STL_USE_NEW_IOSTREAMS */

template <class _CharT, class _Traits, class _Alloc>
ostream& operator<<(ostream& __os, 
                    const basic_string<_CharT,_Traits,_Alloc>& __s);

template <class _CharT, class _Traits, class _Alloc>
istream& operator>>(istream& __is, basic_string<_CharT,_Traits,_Alloc>& __s);

template <class _CharT, class _Traits, class _Alloc>    
istream& getline(istream& __is,
                 basic_string<_CharT,_Traits,_Alloc>& __s,
                 _CharT __delim);

template <class _CharT, class _Traits, class _Alloc>    
istream& 
getline(istream& __is, basic_string<_CharT,_Traits,_Alloc>& __s);

#endif /* __STL_USE_NEW_IOSTREAMS */

template <class _CharT, class _Traits, class _Alloc>
void _S_string_copy(const basic_string<_CharT,_Traits,_Alloc>& __s,
                    _CharT* __buf,
                    size_t __n);

extern const char* __get_c_string(const string& __s);

// ------------------------------------------------------------
// Typedefs

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#pragma reset woff 1375
#endif

__STL_END_NAMESPACE

#endif /* _CXXRT_STRING_H__ */


// Local Variables:
// mode:C++
// End:

