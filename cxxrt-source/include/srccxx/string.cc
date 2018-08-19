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

#ifndef __SRCCXX_STRING_CC__
#define __SRCCXX_STRING_CC__

#include <stl_config.h>
#include <cxxrt/string.h>
#include <stl_string_fwd.h>
#include <cctype>
#include <functional>
#include <cxxrt/stdexcept.h>
#include <stl_iterator_base.h>
#include <memory>
#include <algorithm>
#include <char_traits_helper.h>
#include <cxxrt/stdexcept.h>

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

// A helper class to use a char_traits as a function object.

template <class _Traits>
struct _Not_within_traits
  : public unary_function<typename _Traits::char_type, bool>
{
  typedef const typename _Traits::char_type* _Pointer;
  const _Pointer _M_first;
  const _Pointer _M_last;

  _Not_within_traits(_Pointer __f, _Pointer __l)
    : _M_first(__f), _M_last(__l) {}
  _Not_within_traits(_Not_within_traits const& __nwt):
    _M_first(__nwt._M_first),
    _M_last(__nwt._M_last)
    {}
  _Not_within_traits& operator=(_Not_within_traits const& __nwt)
    {
      _M_first = __nwt._M_first;
      _M_last = __nwt._M_last;
      return *this;
    }

  bool operator()(const typename _Traits::char_type& __x) const {
    return find_if(_M_first, _M_last,
                   bind1st(_Eq_traits<_Traits>(), __x)) == _M_last;
  }
};

// ------------------------------------------------------------

// Helper functions for exception handling.
template <class _Tp, class _Alloc>
void _String_base<_Tp,_Alloc>::_M_throw_length_error() const {
  __STL_THROW(_CXXRT_STD length_error("basic_string"));
}

template <class _Tp, class _Alloc>
void _String_base<_Tp, _Alloc>::_M_throw_out_of_range() const {
  __STL_THROW(_CXXRT_STD out_of_range("basic_string"));
}

// ------------------------------------------------------------
// Non-inline declarations.

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type const
basic_string<_CharT,_Traits,_Alloc>::npos
  = (typename basic_string<_CharT,_Traits,_Alloc>::size_type) -1;

// Change the string's capacity so that it is large enough to hold
//  at least __res_arg elements, plus the terminating null.  Note that,
//  if __res_arg < capacity(), this member function may actually decrease
//  the string's capacity.
template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT,_Traits,_Alloc>::reserve(size_type __res_arg) {
  if (__res_arg > max_size())
    _M_throw_length_error();

  size_type __n = max(__res_arg, size()) + 1;
  pointer __new_start = _M_allocate(__n);
  pointer __new_finish = __new_start;

  __STL_TRY {
    __new_finish = uninitialized_copy(_M_start, _M_finish, __new_start);
    _M_construct_null(__new_finish);
  }
  __STL_UNWIND((destroy(__new_start, __new_finish),
                _M_deallocate(__new_start, __n)));

  destroy(_M_start, _M_finish + 1);
  _M_deallocate_block();
  _M_start = __new_start;
  _M_finish = __new_finish;
  _M_end_of_storage = __new_start + __n;
}

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT,_Traits,_Alloc>&
basic_string<_CharT,_Traits,_Alloc>::append(size_type __n, _CharT __c) {
  if (__n > max_size() || size() > max_size() - __n)
    _M_throw_length_error();
  if (size() + __n > capacity())
    reserve(size() + max(size(), __n));
  if (__n > 0) {
    uninitialized_fill_n(_M_finish + 1, __n - 1, __c);
    __STL_TRY {
      _M_construct_null(_M_finish + __n);
    }
    __STL_UNWIND(destroy(_M_finish + 1, _M_finish + __n));
    _Traits::assign(*_M_finish, __c);
    _M_finish += __n;
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT,_Traits,_Alloc>&
basic_string<_CharT,_Traits,_Alloc>::assign(size_type __n, _CharT __c) {
  if (__n <= size()) {
    _Traits::assign(_M_start, __n, __c);
    erase(_M_start + __n, _M_finish);
  }
  else {
    _Traits::assign(_M_start, size(), __c);
    append(__n - size(), __c);
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT,_Traits,_Alloc>&
basic_string<_CharT,_Traits,_Alloc>::assign(const _CharT* __f,
                                            const _CharT* __l)
{
  const ptrdiff_t __n = __l - __f;
  if (static_cast<size_type>(__n) <= size()) {
    _Traits::copy(_M_start, __f, __n);
    erase(_M_start + __n, _M_finish);
  }
  else {
    _Traits::copy(_M_start, __f, size());
    append(__f + size(), __l);
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::iterator
basic_string<_CharT,_Traits,_Alloc>
  ::_M_insert_aux(typename basic_string<_CharT,_Traits,_Alloc>::iterator __p,
                  _CharT __c)
{
  iterator __new_pos = __p;
  if (_M_finish + 1 < _M_end_of_storage) {
    _M_construct_null(_M_finish + 1);
    _Traits::move(__p + 1, __p, _M_finish - __p);
    _Traits::assign(*__p, __c);
    ++_M_finish;
  }
  else {
    const size_type __old_len = size();
    const size_type __len = __old_len +
                            max(__old_len, static_cast<size_type>(1)) + 1;
    iterator __new_start = _M_allocate(__len);
    iterator __new_finish = __new_start;
    __STL_TRY {
      __new_pos = uninitialized_copy(_M_start, __p, __new_start);
      construct(__new_pos, __c);
      __new_finish = __new_pos + 1;
      __new_finish = uninitialized_copy(__p, _M_finish, __new_finish);
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
  return __new_pos;
}

template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT,_Traits,_Alloc>
  ::insert(basic_string<_CharT,_Traits,_Alloc>::iterator __position,
           size_t __n, _CharT __c)
{
  if (__n != 0) {
    if (size_type(_M_end_of_storage - _M_finish) >= __n + 1) {
      const size_type __elems_after = _M_finish - __position;
      iterator __old_finish = _M_finish;
      if (__elems_after >= __n) {
        uninitialized_copy((_M_finish - __n) + 1, _M_finish + 1,
                           _M_finish + 1);
        _M_finish += __n;
        _Traits::move(__position + __n,
                      __position, (__elems_after - __n) + 1);
        _Traits::assign(__position, __n, __c);
      }
      else {
        uninitialized_fill_n(_M_finish + 1, __n - __elems_after - 1, __c);
        _M_finish += __n - __elems_after;
        __STL_TRY {
          uninitialized_copy(__position, __old_finish + 1, _M_finish);
          _M_finish += __elems_after;
        }
        __STL_UNWIND((destroy(__old_finish + 1, _M_finish),
                      _M_finish = __old_finish));
        _Traits::assign(__position, __elems_after + 1, __c);
      }
    }
    else {
      const size_type __old_size = size();
      const size_type __len = __old_size + max(__old_size, __n) + 1;
      iterator __new_start = _M_allocate(__len);
      iterator __new_finish = __new_start;
      __STL_TRY {
        __new_finish = uninitialized_copy(_M_start, __position, __new_start);
        __new_finish = uninitialized_fill_n(__new_finish, __n, __c);
        __new_finish = uninitialized_copy(__position, _M_finish,
                                          __new_finish);
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

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT,_Traits,_Alloc>&
basic_string<_CharT,_Traits,_Alloc>
  ::replace(iterator __first, iterator __last, size_type __n, _CharT __c)
{
  const size_type __len = static_cast<size_type>(__last - __first);
  if (__len >= __n) {
    _Traits::assign(__first, __n, __c);
    erase(__first + __n, __last);
  }
  else {
    _Traits::assign(__first, __len, __c);
    insert(__last, __n - __len, __c);
  }
  return *this;
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::find(const _CharT* __s, size_type __pos, size_type __n) const
{
  if (__pos >= size())
    return npos;
  else {
    const const_iterator __result =
      search(_M_start + __pos, _M_finish,
             __s, __s + __n, _Eq_traits<_Traits>());
    return __result != _M_finish ? __result - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::find(_CharT __c, size_type __pos) const
{
  if (__pos >= size())
    return npos;
  else {
    const const_iterator __result =
      find_if(_M_start + __pos, _M_finish,
              bind2nd(_Eq_traits<_Traits>(), __c));
    return __result != _M_finish ? __result - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::rfind(const _CharT* __s, size_type __pos, size_type __n) const
{
  const size_t __len = size();

  if (__n > __len)
    return npos;
  else if (__n == 0)
    return min(__len, __pos);
  else {
    const const_iterator __last = begin() + min(__len - __n, __pos) + __n;
    const const_iterator __result = find_end(begin(), __last,
                                           __s, __s + __n,
                                           _Eq_traits<_Traits>());
    return __result != __last ? __result - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::rfind(_CharT __c, size_type __pos) const
{
  const size_type __len = size();

  if (__len < 1)
    return npos;
  else {
    const const_iterator __last = begin() + min(__len - 1, __pos) + 1;
    const_reverse_iterator __rresult =
      find_if(const_reverse_iterator(__last), rend(),
              bind2nd(_Eq_traits<_Traits>(), __c));
    return __rresult != rend() ? (__rresult.base() - 1) - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::find_first_of(const _CharT* __s, size_type __pos, size_type __n) const
{
  if (__pos >= size())
    return npos;
  else {
    const_iterator __result = __STD::find_first_of(begin() + __pos, end(),
                                                   __s, __s + __n,
                                                   _Eq_traits<_Traits>());
    return __result != _M_finish ? __result - begin() : npos;
  }
}


template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::find_last_of(const _CharT* __s, size_type __pos, size_type __n) const
{
  const size_type __len = size();

  if (__len < 1)
    return npos;
  else {
    const const_iterator __last = _M_start + min(__len - 1, __pos) + 1;
    const const_reverse_iterator __rresult =
      __STD::find_first_of(const_reverse_iterator(__last), rend(),
                           __s, __s + __n,
                           _Eq_traits<_Traits>());
    return __rresult != rend() ? (__rresult.base() - 1) - _M_start : npos;
  }
}


template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::find_first_not_of(const _CharT* __s, size_type __pos, size_type __n) const
{
  if (__pos > size())
    return npos;
  else {
    const_iterator __result = find_if(_M_start + __pos, _M_finish,
                                _Not_within_traits<_Traits>(__s, __s + __n));
    return __result != _M_finish ? __result - _M_start : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::find_first_not_of(_CharT __c, size_type __pos) const
{
  if (__pos > size())
    return npos;
  else {
    const_iterator __result
      = find_if(begin() + __pos, end(),
                not1(bind2nd(_Eq_traits<_Traits>(), __c)));
    return __result != _M_finish ? __result - begin() : npos;
  }
}

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>
  ::find_last_not_of(const _CharT* __s, size_type __pos, size_type __n) const
{

  const size_type __len = size();

  if (__len < 1)
    return npos;
  else {
    const const_iterator __last = begin() + min(__len - 1, __pos) + 1;
    const const_reverse_iterator __rresult =
      find_if(const_reverse_iterator(__last), rend(),
              _Not_within_traits<_Traits>(__s, __s + __n));
    return __rresult != rend() ? (__rresult.base() - 1) - begin() : npos;
  }
}

template <class _Tp, class _Traits, class _Alloc>
typename basic_string<_Tp, _Traits, _Alloc>::size_type
basic_string<_Tp, _Traits, _Alloc>
  ::find_last_not_of(_Tp __c, size_type __pos) const
{
  const size_type __len = size();

  if (__len < 1)
    return npos;
  else {
    const const_iterator __last = begin() + min(__len - 1, __pos) + 1;
    const_reverse_iterator __rresult =
      find_if(const_reverse_iterator(__last), rend(),
              not1(bind2nd(_Eq_traits<_Traits>(), __c)));
    return __rresult != rend() ? (__rresult.base() - 1) - begin() : npos;
  }
}

// ------------------------------------------------------------
// Non-member functions.

// Operator+

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const basic_string<_CharT,_Traits,_Alloc>& __y)
{
  typedef basic_string<_CharT,_Traits,_Alloc> _Str;
  typedef typename _Str::_Reserve_t _Reserve_t;
  _Reserve_t __reserve;
  _Str __result(__reserve, __x.size() + __y.size(), __x.get_allocator());
  __result.append(__x);
  __result.append(__y);
  return __result;
}

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const _CharT* __s,
          const basic_string<_CharT,_Traits,_Alloc>& __y) {
  typedef basic_string<_CharT,_Traits,_Alloc> _Str;
  typedef typename _Str::_Reserve_t _Reserve_t;
  _Reserve_t __reserve;
  const size_t __n = _Traits::length(__s);
  _Str __result(__reserve, __n + __y.size());
  __result.append(__s, __s + __n);
  __result.append(__y);
  return __result;
}

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(_CharT __c,
          const basic_string<_CharT,_Traits,_Alloc>& __y) {
  typedef basic_string<_CharT,_Traits,_Alloc> _Str;
  typedef typename _Str::_Reserve_t _Reserve_t;
  _Reserve_t __reserve;
  _Str __result(__reserve, 1 + __y.size());
  __result.push_back(__c);
  __result.append(__y);
  return __result;
}

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT* __s) {
  typedef basic_string<_CharT,_Traits,_Alloc> _Str;
  typedef typename _Str::_Reserve_t _Reserve_t;
  _Reserve_t __reserve;
  const size_t __n = _Traits::length(__s);
  _Str __result(__reserve, __x.size() + __n, __x.get_allocator());
  __result.append(__x);
  __result.append(__s, __s + __n);
  return __result;
}

template <class _CharT, class _Traits, class _Alloc>
inline basic_string<_CharT,_Traits,_Alloc>
operator+(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT __c) {
  typedef basic_string<_CharT,_Traits,_Alloc> _Str;
  typedef typename _Str::_Reserve_t _Reserve_t;
  _Reserve_t __reserve;
  _Str __result(__reserve, __x.size() + 1, __x.get_allocator());
  __result.append(__x);
  __result.push_back(__c);
  return __result;
}

// Operator== and operator!=

template <class _CharT, class _Traits, class _Alloc>
bool
operator==(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return __x.size() == __y.size() &&
         _Traits::compare(__x.data(), __y.data(), __x.size()) == 0;
}

template <class _CharT, class _Traits, class _Alloc>
bool
operator==(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  size_t __n = _Traits::length(__s);
  return __n == __y.size() && _Traits::compare(__s, __y.data(), __n) == 0;
}

template <class _CharT, class _Traits, class _Alloc>
bool
operator==(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s) {
  size_t __n = _Traits::length(__s);
  return __x.size() == __n && _Traits::compare(__x.data(), __s, __n) == 0;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator!=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator!=(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return !(__s == __y);
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator!=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s) {
  return !(__x == __s);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

// Operator< (and also >, <=, and >=).

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return basic_string<_CharT,_Traits,_Alloc>
    ::_M_compare(__x.begin(), __x.end(), __y.begin(), __y.end()) < 0;
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<(const _CharT* __s,
          const basic_string<_CharT,_Traits,_Alloc>& __y) {
  size_t __n = _Traits::length(__s);
  return basic_string<_CharT,_Traits,_Alloc>
    ::_M_compare(__s, __s + __n, __y.begin(), __y.end()) < 0;
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT* __s) {
  size_t __n = _Traits::length(__s);
  return basic_string<_CharT,_Traits,_Alloc>
    ::_M_compare(__x.begin(), __x.end(), __s, __s + __n) < 0;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return __y < __x;
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>(const _CharT* __s,
          const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return __y < __s;
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>(const basic_string<_CharT,_Traits,_Alloc>& __x,
          const _CharT* __s) {
  return __s < __x;
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<=(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return !(__y < __s);
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator<=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s) {
  return !(__s < __x);
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return !(__x < __y);
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>=(const _CharT* __s,
           const basic_string<_CharT,_Traits,_Alloc>& __y) {
  return !(__s < __y);
}

template <class _CharT, class _Traits, class _Alloc>
inline bool
operator>=(const basic_string<_CharT,_Traits,_Alloc>& __x,
           const _CharT* __s) {
  return !(__x < __s);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

// I/O.

__STL_END_NAMESPACE

#if defined(_CXXRT_IOSTREAM_IMPL) || defined(_CXXRT_STRINGIO_IMPL)

#ifdef __STL_USE_NEW_IOSTREAMS
#include <iostream>
#include <cxxrt/ctype.h>
#include <cxxrt/streambuf.h>
#else
#include <iostream.h>
#endif /* __STL_USE_NEW_IOSTREAMS */

#endif

__STL_BEGIN_NAMESPACE

template <class _CharT, class _Traits, class _Alloc>
basic_string<_CharT, _Traits, _Alloc>::basic_string(size_type __n, _CharT __c,
                                                    const allocator_type& __a)
  : _Base(__a, __n + 1)
{
  _M_finish = uninitialized_fill_n(_M_start, __n, __c);
  _M_terminate_string();
}

template <class _CharT, class _Traits, class _Alloc>
#if !defined(_CXXRT_STRING_INSTANTIATION)
inline
#endif
basic_string<_CharT, _Traits, _Alloc>::~basic_string()
{
  destroy(_M_start, _M_finish + 1);
}

template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_construct_null(_CharT* __p)
{
  construct(__p);
#   ifdef __STL_DEFAULT_CONSTRUCTOR_BUG
  __STL_TRY {
    *__p = (_CharT) 0;
  }
  __STL_UNWIND(destroy(__p));
#   endif
}

template <class _CharT, class _Traits, class _Alloc>
void basic_string<_CharT, _Traits, _Alloc>::_M_terminate_string()
{
  __STL_TRY {
    _M_construct_null(_M_finish);
  }
  __STL_UNWIND(destroy(_M_start, _M_finish));
}

#if 0
#ifdef __STL_MEMBER_TEMPLATES
template <class _CharT, class _Traits, class _Alloc>
template <class _ForwardIter>
void basic_string<_CharT, _Traits, _Alloc>::_M_range_initialize(_ForwardIter __f, _ForwardIter __l,
                                                                forward_iterator_tag)
{
  difference_type __n = 0;
  distance(__f, __l, __n);
  _M_allocate_block(__n + 1);
  _M_finish = uninitialized_copy(__f, __l, _M_start);
  _M_terminate_string();
}
#endif /* __STL_MEMBER_TEMPLATES */
#endif

#if defined(_CXXRT_STRINGIO_IMPL) || defined(_CXXRT_IOSTREAM_IMPL)

#ifdef __STL_USE_NEW_IOSTREAMS

template <class _CharT, class _Traits>
inline bool
__sgi_string_fill(basic_ostream<_CharT, _Traits>& __os,
                  basic_streambuf<_CharT, _Traits>* __buf,
                  size_t __n)
{
  _CharT __f = __os.fill();
  size_t __i;
  bool __ok = true;

  for (__i = 0; __i < __n; __i++)
    __ok = __ok && !_Traits::eq_int_type(__buf->sputc(__f), _Traits::eof());
  return __ok;
}

template <class _CharT, class _Traits, class _Alloc>
basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os,
           const basic_string<_CharT,_Traits,_Alloc>& __s)
{
  typename basic_ostream<_CharT, _Traits>::sentry __sentry(__os);
  bool __ok = false;

  if (__sentry) {
    __ok = true;
    size_t __n = __s.size();
    size_t __pad_len = 0;
    const bool __left = (__os.flags() & ios::left) != 0;
    const size_t __w = __os.width(0);
    basic_streambuf<_CharT, _Traits>* __buf = __os.rdbuf();

    if (__w > 0) {
      __n = min(__w, __n);
      __pad_len = __w - __n;
    }

    if (!__left)
      __ok = __sgi_string_fill(__os, __buf, __pad_len);

    __ok = __ok && __buf->sputn(__s.data(), __n) == streamsize(__n);

    if (__left)
      __ok = __ok && __sgi_string_fill(__os, __buf, __pad_len);
  }

  if (!__ok)
    __os.setstate(ios_base::failbit);

  return __os;
}

template <class _CS_charT, class _CS_Ctype>
inline _CS_charT*
_CS_find_space(_CS_charT* _CS_beg, _CS_charT* _CS_end, _CS_Ctype const& _CS_ct)
{
  while (_CS_beg != _CS_end && !_CS_ct.is(ctype_base::space, *_CS_beg))
    ++_CS_beg;
  return _CS_beg;
}

inline char*
_CS_find_space(char* _CS_beg, char* _CS_end, ctype<char> const& _CS_ct)
{
  ctype_base::mask const* _CS_table = _CS_ct._CS_table();

  while (_CS_beg != _CS_end && (_CS_table[static_cast<unsigned char>(*_CS_beg)] & ctype_base::space) == 0)
    ++_CS_beg;
  return _CS_beg;
}

template <class _CharT, class _Traits, class _Alloc>
basic_istream<_CharT, _Traits>&
operator>>(basic_istream<_CharT, _Traits>& __is,
           basic_string<_CharT,_Traits,_Alloc>& __s)
{
  typename basic_istream<_CharT, _Traits>::sentry __sentry(__is);

  if (__sentry) {
    basic_streambuf<_CharT, _Traits>* __buf = __is.rdbuf();
    const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__is.getloc());

    __s.clear();
    size_t __n = __is.width(0);
    if (__n == 0)
      __n = static_cast<size_t>(-1);
    else
      __s.reserve(__n);

    if (__buf->_CS_end() - __buf->_CS_begin() > 1)
        while (__n > 0)
        {
            _CharT* _CS_beg = __buf->_CS_begin();
            _CharT* _CS_end = __buf->_CS_end();
            if (size_t(_CS_end - _CS_beg) > __n)
              _CS_end = _CS_beg + __n;
            __n -= _CS_end - _CS_beg;

            _CS_beg = _CS_find_space(_CS_beg, _CS_end, __ctype);

            __s.append(__buf->_CS_begin(), _CS_beg);
            __buf->_CS_commit(_CS_beg);

            if (_CS_beg != _CS_end)
              break;
            else
              if (_Traits::eq_int_type(__buf->_CS_underflow(), _Traits::eof()))
                {
                  __is.setstate(ios_base::eofbit);
                  break;
                }
        }
    else
      while (__n-- > 0) {
        typename _Traits::int_type __c1 = __buf->sbumpc();
        if (_Traits::eq_int_type(__c1, _Traits::eof())) {
          __is.setstate(ios_base::eofbit);
          break;
        }
        else {
          _CharT __c = _Traits::to_char_type(__c1);

          if (__ctype.is(ctype_base::space, __c)) {
            if (_Traits::eq_int_type(__buf->sputbackc(__c), _Traits::eof()))
            __is.setstate(ios_base::failbit);
            break;
          }
          else
            __s.push_back(__c);
        }
      }

    // If we have read no characters, then set failbit.
    if (__s.size() == 0)
      __is.setstate(ios_base::failbit);
  }
  else
    __is.setstate(ios_base::failbit);

  return __is;
}

template <class _CharT, class _Traits, class _Alloc>
basic_istream<_CharT, _Traits>&
getline(basic_istream<_CharT, _Traits>& __is,
        basic_string<_CharT,_Traits,_Alloc>& __s,
        _CharT __delim)
{
  size_t __nread = 0;
  typename basic_istream<_CharT, _Traits>::sentry __sentry(__is, true);
  if (__sentry) {
    basic_streambuf<_CharT, _Traits>* __buf = __is.rdbuf();
    __s.clear();

    if (__buf->_CS_end() - __buf->_CS_begin() > 1)
      while (__nread < __s.max_size())
      {
          _CharT* _CS_end = const_cast<_CharT*>(_Traits::find(__buf->_CS_begin(), __buf->_CS_end() - __buf->_CS_begin(),__delim));
          if (_CS_end == 0)
              {
                  __s.append(__buf->_CS_begin(), __buf->_CS_end());
                  __nread += __buf->_CS_end() - __buf->_CS_begin();
                  __buf->_CS_commit(__buf->_CS_end());
                  if (_Traits::eq_int_type(__buf->_CS_underflow(), _Traits::eof()))
                      {
                          __is.setstate(ios_base::eofbit);
                          break;
                      }
              }
          else
              {
                  __s.append(__buf->_CS_begin(), _CS_end);
                  ++_CS_end;
                  __nread += _CS_end - __buf->_CS_begin();
                  __buf->_CS_commit(_CS_end);
                  break;
              }
      }
    else
      while (__nread < __s.max_size()) {
        typename _Traits::int_type __c1 = __buf->sbumpc();
        if (_Traits::eq_int_type(__c1, _Traits::eof())) {
          __is.setstate(ios_base::eofbit);
          break;
        }
        else {
          ++__nread;
          _CharT __c = _Traits::to_char_type(__c1);
          if (!_Traits::eq(__c, __delim))
            __s.push_back(__c);
          else
            break;              // Character is extracted but not appended.
        }
      }
  }
  if (__nread == 0 || __nread >= __s.max_size())
    __is.setstate(ios_base::failbit);

  return __is;
}

template <class _CharT, class _Traits, class _Alloc>
basic_istream<_CharT, _Traits>&
getline(basic_istream<_CharT, _Traits>& __is,
        basic_string<_CharT,_Traits,_Alloc>& __s)
{
  return getline(__is, __s, __is.widen('\n'));
}

#else /* __STL_USE_NEW_IOSTREAMS */

inline void __sgi_string_fill(ostream& __os, streambuf* __buf, size_t __n)
{
  char __f = __os.fill();
  size_t __i;

  for (__i = 0; __i < __n; __i++) __buf->sputc(__f);
}

template <class _CharT, class _Traits, class _Alloc>
ostream& operator<<(ostream& __os,
                    const basic_string<_CharT,_Traits,_Alloc>& __s)
{
  streambuf* __buf = __os.rdbuf();
  if (__buf) {
    size_t __n = __s.size();
    size_t __pad_len = 0;
    const bool __left = (__os.flags() & ios::left) != 0;
    const size_t __w = __os.width();

    if (__w > 0) {
      __n = min(__w, __n);
      __pad_len = __w - __n;
    }

    if (!__left)
      __sgi_string_fill(__os, __buf, __pad_len);

    const size_t __nwritten = __buf->sputn(__s.data(), __n);

    if (__left)
      __sgi_string_fill(__os, __buf, __pad_len);

    if (__nwritten != __n)
      __os.clear(__os.rdstate() | ios::failbit);

    __os.width(0);
  }
  else
    __os.clear(__os.rdstate() | ios::badbit);

  return __os;
}

template <class _CharT, class _Traits, class _Alloc>
istream& operator>>(istream& __is, basic_string<_CharT,_Traits,_Alloc>& __s)
{
  if (!__is)
    return __is;

  streambuf* __buf = __is.rdbuf();
  if (__buf) {

#ifdef __USLC__
/* Jochen Schlick '1999  - operator >> modified. Work-around to get the
 *                         output buffer flushed (necessary when using
 *                         "cout" (without endl or flushing) followed by
 *                         "cin >>" ...)
 */
    if (__is.flags() & ios::skipws) {
      _CharT __c;
      do
         __is.get(__c);
      while (__is && isspace(__c));
      if (__is)
         __is.putback(__c);
    }
#else
    if (__is.flags() & ios::skipws) {
      _CharT __c;
      do {
        int __c1 = __buf->sbumpc();
        if (__c1 == EOF) {
          __is.clear(__is.rdstate() | ios::eofbit | ios::failbit);
          break;
        }
        else
          __c = _Traits::to_char_type(__c1);
      }
      while (isspace((unsigned char) __c));

      if (__buf->sputbackc(__c) == EOF)
        __is.clear(__is.rdstate() | ios::failbit);
    }
#endif

    // If we arrive at end of file (or fail for some other reason) while
    // still discarding whitespace, then we don't try to read the string.
    if (__is) {
      __s.clear();

      size_t __n = __is.width();
      if (__n == 0)
        __n = static_cast<size_t>(-1);
      else
        __s.reserve(__n);

      while (__n-- > 0) {
        int __c1 = __buf->sbumpc();
        if (__c1 == EOF) {
          __is.clear(__is.rdstate() | ios::eofbit);
          break;
        }
        else {
          _CharT __c = _Traits::to_char_type(__c1);

          if (isspace((unsigned char) __c)) {
            if (__buf->sputbackc(__c) == EOF)
              __is.clear(__is.rdstate() | ios::failbit);
            break;
          }
          else
            __s.push_back(__c);
        }
      }

      // If we have read no characters, then set failbit.
      if (__s.size() == 0)
        __is.clear(__is.rdstate() | ios::failbit);
    }

    __is.width(0);
  }
  else                          // We have no streambuf.
    __is.clear(__is.rdstate() | ios::badbit);

  return __is;
}

template <class _CharT, class _Traits, class _Alloc>
istream& getline(istream& __is,
                 basic_string<_CharT,_Traits,_Alloc>& __s,
                 _CharT __delim)
{
  streambuf* __buf = __is.rdbuf();
  if (__buf) {
    size_t __nread = 0;
    if (__is) {
      __s.clear();

      while (__nread < __s.max_size()) {
        int __c1 = __buf->sbumpc();
        if (__c1 == EOF) {
          __is.clear(__is.rdstate() | ios::eofbit);
          break;
        }
        else {
          ++__nread;
          _CharT __c = _Traits::to_char_type(__c1);
          if (!_Traits::eq(__c, __delim))
            __s.push_back(__c);
          else
            break;              // Character is extracted but not appended.
        }
      }
    }

    if (__nread == 0 || __nread >= __s.max_size())
      __is.clear(__is.rdstate() | ios::failbit);
  }
  else
    __is.clear(__is.rdstate() | ios::badbit);

  return __is;
}

template <class _CharT, class _Traits, class _Alloc>
inline istream&
getline(istream& __is, basic_string<_CharT,_Traits,_Alloc>& __s)
{
  return getline(__is, __s, '\n');
}

#endif /* __STL_USE_NEW_IOSTREAMS */

#endif /* defined(_CXXRT_IOSTREAM_IMPL) || defined(_CXXRT_STRINGIO_IMPL) */

template <class _CharT, class _Traits, class _Alloc>
void _S_string_copy(const basic_string<_CharT,_Traits,_Alloc>& __s,
                    _CharT* __buf,
                    size_t __n)
{
  if (__n > 0) {
    __n = min(__n - 1, __s.size());
    copy(__s.begin(), __s.begin() + __n, __buf);
    _Traits::assign(__buf[__n],
                    basic_string<_CharT,_Traits,_Alloc>::_M_null());
  }
}

extern const char* __get_c_string(const string& __s);

// ------------------------------------------------------------
// Typedefs

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#pragma reset woff 1375
#endif

__STL_END_NAMESPACE

#endif /* __SRCCXX_STRING_CC__ */


// Local Variables:
// mode:C++
// End:

