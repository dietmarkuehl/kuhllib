/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996-1998
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

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_ITERATOR_H
#define __SGI_STL_INTERNAL_ITERATOR_H

#include <stl_reverse_iterator.h>

__STL_BEGIN_NAMESPACE


template <class _Container>
class back_insert_iterator {
protected:
  _Container* container;
public:
  typedef _Container          container_type;
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit back_insert_iterator(_Container& __x) : container(&__x) {}
  back_insert_iterator<_Container>&
  operator=(const typename _Container::value_type& __value) { 
    container->push_back(__value);
    return *this;
  }
  back_insert_iterator<_Container>& operator*() { return *this; }
  back_insert_iterator<_Container>& operator++() { return *this; }
  //-dk:TODO back_insert_iterator<_Container>& operator++(int) { return *this; }
  back_insert_iterator<_Container>  operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _Container>
inline output_iterator_tag
iterator_category(const back_insert_iterator<_Container>&)
{
  return output_iterator_tag();
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& __x) {
  return back_insert_iterator<_Container>(__x);
}

template <class _Container>
class front_insert_iterator {
protected:
  _Container* container;
public:
  typedef _Container          container_type;
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  explicit front_insert_iterator(_Container& __x) : container(&__x) {}
  front_insert_iterator<_Container>&
  operator=(const typename _Container::value_type& __value) { 
    container->push_front(__value);
    return *this;
  }
  front_insert_iterator<_Container>& operator*() { return *this; }
  front_insert_iterator<_Container>& operator++() { return *this; }
  //-dk:TODO front_insert_iterator<_Container>& operator++(int) { return *this; }
  front_insert_iterator<_Container> operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _Container>
inline output_iterator_tag
iterator_category(const front_insert_iterator<_Container>&)
{
  return output_iterator_tag();
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _Container>
inline front_insert_iterator<_Container> front_inserter(_Container& __x) {
  return front_insert_iterator<_Container>(__x);
}

template <class _Container>
class insert_iterator {
protected:
  _Container* container;
  typename _Container::iterator iter;
public:
  typedef _Container          container_type;
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  insert_iterator(_Container& __x, typename _Container::iterator __i) 
    : container(&__x), iter(__i) {}
  insert_iterator<_Container>&
  operator=(const typename _Container::value_type& __value) { 
    iter = container->insert(iter, __value);
    ++iter;
    return *this;
  }
  insert_iterator<_Container>& operator*() { return *this; }
  insert_iterator<_Container>& operator++() { return *this; }
  //-dk:TODO insert_iterator<_Container>& operator++(int) { return *this; }
  insert_iterator<_Container> operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _Container>
inline output_iterator_tag
iterator_category(const insert_iterator<_Container>&)
{
  return output_iterator_tag();
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _Container, class _Iterator>
inline 
insert_iterator<_Container> inserter(_Container& __x, _Iterator __i)
{
  typedef typename _Container::iterator __iter;
  return insert_iterator<_Container>(__x, __iter(__i));
}

// istream_iterator and ostream_iterator look very different if we're
// using new, templatized iostreams than if we're using the old cfront
// version.

#ifdef __STL_USE_NEW_IOSTREAMS

template <class _Tp, 
          class _CharT = char, class _Traits = char_traits<_CharT>,
          class _Dist = ptrdiff_t> 
class istream_iterator:
  public iterator<__STD::input_iterator_tag, _Tp, int, _Tp const*, _Tp const&> {
public:
  typedef _CharT                         char_type;
  typedef _Traits                        traits_type;
  typedef basic_istream<_CharT, _Traits> istream_type;

  typedef input_iterator_tag             iterator_category;
  typedef _Tp                            value_type;
  typedef _Dist                          difference_type;
  typedef const _Tp*                     pointer;
  typedef const _Tp&                     reference;

  istream_iterator() : _M_stream(0), _M_ok(false) {}
  istream_iterator(istream_type& __s) : _M_stream(&__s) { _M_read(); }
  istream_iterator(istream_iterator const& __it)
    : _M_stream(__it._M_stream)
    , _M_value(__it._M_value)
    , _M_ok(__it._M_ok)
    {}
  istream_iterator& operator= (istream_iterator const& __it) {
    _M_stream = __it._M_stream;
    _M_value = __it._M_value;
    _M_ok = __it._M_ok;
    return *this;
  }

  reference operator*() const { return _M_value; }
  pointer operator->() const { return &(operator*()); }

  istream_iterator& operator++() { 
    _M_read(); 
    return *this;
  }
  istream_iterator operator++(int)  {
    istream_iterator __tmp = *this;
    _M_read();
    return __tmp;
  }

  bool _M_equal(const istream_iterator& __x) const
    { return (_M_ok == __x._M_ok) && (!_M_ok || _M_stream == __x._M_stream); }

private:
  istream_type* _M_stream;
  _Tp _M_value;
  bool _M_ok;

  void _M_read() {
    _M_ok = (_M_stream && *_M_stream) ? true : false;
    if (_M_ok) {
      *_M_stream >> _M_value;
      _M_ok = *_M_stream ? true : false;
    }
  }
};

template <class _Tp, class _CharT, class _Traits, class _Dist>
inline bool 
operator==(const istream_iterator<_Tp, _CharT, _Traits, _Dist>& __x,
           const istream_iterator<_Tp, _CharT, _Traits, _Dist>& __y) {
  return __x._M_equal(__y);
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Tp, class _CharT, class _Traits, class _Dist>
inline bool 
operator!=(const istream_iterator<_Tp, _CharT, _Traits, _Dist>& __x,
           const istream_iterator<_Tp, _CharT, _Traits, _Dist>& __y) {
  return !__x._M_equal(__y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

template <class _Tp,
          class _CharT = char, class _Traits = char_traits<_CharT> >
class ostream_iterator {
public:
  typedef _CharT                         char_type;
  typedef _Traits                        traits_type;
  typedef basic_ostream<_CharT, _Traits> ostream_type;

  typedef output_iterator_tag            iterator_category;
  typedef void                           value_type;
  typedef void                           difference_type;
  typedef void                           pointer;
  typedef void                           reference;

  ostream_iterator(ostream_type& __s) : _M_stream(&__s), _M_string(0) {}
  ostream_iterator(ostream_type& __s, const _CharT* __c) 
    : _M_stream(&__s), _M_string(__c)  {}
  ostream_iterator<_Tp, _CharT, _Traits>& operator=(const _Tp& __value) { 
    *_M_stream << __value;
    if (_M_string) *_M_stream << _M_string;
    return *this;
  }
  ostream_iterator<_Tp, _CharT, _Traits>& operator*() { return *this; }
  ostream_iterator<_Tp, _CharT, _Traits>& operator++() { return *this; } 
  //-dk:TODO ostream_iterator<_Tp>& operator++(int) { return *this; } 
  ostream_iterator<_Tp, _CharT, _Traits> operator++(int) { return *this; } 
private:
  ostream_type* _M_stream;
  const _CharT* _M_string;
};

#else /* __STL_USE_NEW_IOSTREAMS */

template <class _Tp, class _Dist = ptrdiff_t> class istream_iterator;

template <class _Tp, class _Dist>
inline bool operator==(const istream_iterator<_Tp, _Dist>&,
                       const istream_iterator<_Tp, _Dist>&);

template <class _Tp, class _Dist>
class istream_iterator {
#ifdef __STL_MEMBER_TEMPLATES
  template <class _T1, class _D1>
  friend bool operator==(const istream_iterator<_T1, _D1>&,
                         const istream_iterator<_T1, _D1>&);
#else /* __STL_MEMBER_TEMPLATES */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const istream_iterator&,
                                   const istream_iterator&);
#endif /* __STL_MEMBER_TEMPLATES */

protected:
  istream* _M_stream;
  _Tp _M_value;
  bool _M_end_marker;
  void _M_read() {
    _M_end_marker = (*_M_stream) ? true : false;
    if (_M_end_marker) *_M_stream >> _M_value;
    _M_end_marker = (*_M_stream) ? true : false;
  }
public:
  typedef input_iterator_tag  iterator_category;
  typedef _Tp                 value_type;
  typedef _Dist               difference_type;
  typedef const _Tp*          pointer;
  typedef const _Tp&          reference;

  istream_iterator() : _M_stream(&cin), _M_end_marker(false) {}
  istream_iterator(istream& __s) : _M_stream(&__s) { _M_read(); }
  istream_iterator(istream_iterator const& __it): _M_stream(__it._M_stream) {}
  istream_iterator& operator=(istream_iterator const& __it) { _M_stream = __it._M_stream; return *this; }

  reference operator*() const { return _M_value; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
  pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */
  istream_iterator<_Tp, _Dist>& operator++() { 
    _M_read(); 
    return *this;
  }
  istream_iterator<_Tp, _Dist> operator++(int)  {
    istream_iterator<_Tp, _Dist> __tmp = *this;
    _M_read();
    return __tmp;
  }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _Tp, class _Dist>
inline input_iterator_tag 
iterator_category(const istream_iterator<_Tp, _Dist>&)
{
  return input_iterator_tag();
}

template <class _Tp, class _Dist>
inline _Tp* 
value_type(const istream_iterator<_Tp, _Dist>&) { return (_Tp*) 0; }

template <class _Tp, class _Dist>
inline _Dist* 
distance_type(const istream_iterator<_Tp, _Dist>&) { return (_Dist*)0; }

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _Tp, class _Distance>
inline bool operator==(const istream_iterator<_Tp, _Distance>& __x,
                       const istream_iterator<_Tp, _Distance>& __y) {
  return (__x._M_stream == __y._M_stream &&
          __x._M_end_marker == __y._M_end_marker) ||
         __x._M_end_marker == false && __y._M_end_marker == false;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Tp, class _Distance>
inline bool operator!=(const istream_iterator<_Tp, _Distance>& __x,
                       const istream_iterator<_Tp, _Distance>& __y) {
  return !(__x == __y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

template <class _Tp>
class ostream_iterator {
protected:
  ostream* _M_stream;
  const char* _M_string;
public:
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;

  ostream_iterator(ostream& __s) : _M_stream(&__s), _M_string(0) {}
  ostream_iterator(ostream& __s, const char* __c) 
    : _M_stream(&__s), _M_string(__c)  {}
  ostream_iterator<_Tp>& operator=(const _Tp& __value) { 
    *_M_stream << __value;
    if (_M_string) *_M_stream << _M_string;
    return *this;
  }
  ostream_iterator<_Tp>& operator*() { return *this; }
  ostream_iterator<_Tp>& operator++() { return *this; } 
  ostream_iterator<_Tp>& operator++(int) { return *this; } 
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _Tp>
inline output_iterator_tag 
iterator_category(const ostream_iterator<_Tp>&) {
  return output_iterator_tag();
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

#endif /* __STL_USE_NEW_IOSTREAMS */

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_ITERATOR_H */

// Local Variables:
// mode:C++
// End:
