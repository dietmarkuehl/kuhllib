/*
 * Copyright (c) 1997
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

// 1999/8/7 dietmar.kuehl@claas-solutions.de: added declaration of fpos and
//          corresponding typedefs in char_traits

#ifndef __SGI_STL_CHAR_TRAITS_H
#define __SGI_STL_CHAR_TRAITS_H

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#include <stl_config.h>
#if !defined(__CSTRING__)
#  include <cstring>
#endif
#if !defined(__CWCHAR__)
#  include <cwchar>
#endif
#if !defined(_CXXRTCF_CTRAITS_FWD_H__)
#  include <cxxrt/ctraits_fwd.h>
#endif

#if defined(__STL_USE_NEW_IOSTREAMS) && !defined(__SGI_STL_IOSFWD)
#  if !defined(__IOSFWD__)
#    include <iosfwd>
#  endif
#endif /* use new iostreams */

__STL_BEGIN_NAMESPACE

// --- declaration of fpos --------------------------------------------------------------

template <class _CS_state>
class fpos
{
  typedef _CXXRT_off_t off_type;
  typedef _CS_state    state_type;
public:
#if 0
  explicit fpos(state_type _CS_s = _CS_state()):
    _CS_m_state(_CS_s),
    _CS_m_off(0)
    {
    }
#else
  explicit fpos():
    _CS_m_state(),
    _CS_m_off(0)
    {
    }
#endif
  fpos(off_type _CS_o):
    _CS_m_state(),
    _CS_m_off(_CS_o)
    {
    }
  fpos(state_type _CS_s, off_type _CS_o):
    _CS_m_state(_CS_s),
    _CS_m_off(_CS_o)
    {
    }

  operator off_type() const { return _CS_m_off; }

  bool operator== (fpos<_CS_state> const& _CS_p) const
  {
    return _CS_m_state == _CS_p._CS_m_state
    && _CS_m_off == _CS_p._CS_m_off;
  }
  bool operator!= (fpos<_CS_state> const& _CS_p) const { return !(*this == _CS_p); }

  fpos<_CS_state>& operator+= (off_type _CS_o) { _CS_m_off += _CS_o; return *this; }
  fpos<_CS_state>& operator-= (off_type _CS_o) { _CS_m_off -= _CS_o; return *this; }

  _CS_state state() const        { return _CS_m_state; }
  void       state(_CS_state _CS_s)  { _CS_m_state = _CS_s; } 
  off_type   off() const     { return _CS_m_off; }
  void       off(off_type _CS_o) { _CS_m_off = _CS_o; }
private:
  state_type _CS_m_state;
  off_type   _CS_m_off;
};

template <class _CS_state>
inline fpos<_CS_state>
operator+ (fpos<_CS_state> _CS_f, _CXXRT_off_t _CS_o)
{
  return _CS_f += _CS_o;
}

template <class _CS_state>
inline fpos<_CS_state>
operator- (fpos<_CS_state> _CS_f, _CXXRT_off_t _CS_o)
{
  return _CS_f -= _CS_o;
}
template <class _CS_state>
inline _CXXRT_off_t
operator- (fpos<_CS_state> const& _CS_f1, fpos<_CS_state> const& _CS_f2)
{
  return _CS_f1.off() - _CS_f2.off();
}

typedef fpos<mbstate_t> _CS_fpos;
typedef _CS_fpos     streampos;
typedef _CXXRT_off_t streamoff;

// -----------------------------------------------------------------------------

// Class __char_traits_base.

template <class _CharT, class _IntT> struct __char_traits_base {
  typedef _CharT char_type;
  typedef _IntT int_type;
#ifdef __STL_USE_NEW_IOSTREAMS
  typedef streamoff off_type;
  typedef streampos pos_type;
  typedef mbstate_t state_type;
#endif /* __STL_USE_NEW_IOSTREAMS */

  static void assign(char_type& __c1, const char_type& __c2) { __c1 = __c2; }
  static bool eq(const _CharT& __c1, const _CharT& __c2) 
    { return __c1 == __c2; }
  static bool lt(const _CharT& __c1, const _CharT& __c2) 
    { return __c1 < __c2; }

  static int compare(const _CharT* __s1, const _CharT* __s2, size_t __n) {
    for (size_t __i = 0; __i < __n; ++__i)
      if (!eq(__s1[__i], __s2[__i]))
        return __s1[__i] < __s2[__i] ? -1 : 1;
    return 0;
  }

  static size_t length(const _CharT* __s) {
    const _CharT __nullchar = _CharT();
    size_t __i;
    for (__i = 0; !eq(__s[__i], __nullchar); ++__i)
      {}
    return __i;
  }

  static const _CharT* find(const _CharT* __s, size_t __n, const _CharT& __c)
  {
    for ( ; __n > 0 ; ++__s, --__n)
      if (eq(*__s, __c))
        return __s;
    return 0;
  }

  static _CharT* move(_CharT* __s1, const _CharT* __s2, size_t __n) {
    memmove(__s1, __s2, __n * sizeof(_CharT));
    return __s1;
  }
    
  static _CharT* copy(_CharT* __s1, const _CharT* __s2, size_t __n) {
    memcpy(__s1, __s2, __n * sizeof(_CharT));
    return __s1;
  } 

  static _CharT* assign(_CharT* __s, size_t __n, _CharT __c) {
    for (size_t __i = 0; __i < __n; ++__i)
      __s[__i] = __c;
    return __s;
  }

  static int_type not_eof(const int_type& __c) {
    return !eq_int_type(__c, eof()) ? __c : 0;
  }

  static char_type to_char_type(const int_type& __c) {
    return static_cast<char_type>(__c);
  }

  static int_type to_int_type(const char_type& __c) {
    return static_cast<int_type>(__c);
  }

  static bool eq_int_type(const int_type& __c1, const int_type& __c2) {
    return __c1 == __c2;
  }

  static int_type eof() {
    return static_cast<int_type>(-1);
  }
};

// Generic char_traits class.  Note that this class is provided only
//  as a base for explicit specialization; it is unlikely to be useful
//  as is for any particular user-defined type.  In particular, it 
//  *will not work* for a non-POD type.

template <class _CharT> struct char_traits
  : public __char_traits_base<_CharT, _CharT>
{};

// Specialization for char.

__STL_TEMPLATE_NULL struct char_traits<char> 
  : public __char_traits_base<char, int>
{
  static int to_int_type(char const& __c) {
    return static_cast<int>(static_cast<unsigned char>(__c));
  }

  static int compare(const char* __s1, const char* __s2, size_t __n) 
    { return memcmp(__s1, __s2, __n); }
  
  static size_t length(const char* __s) { return strlen(__s); }

  static void assign(char& __c1, const char& __c2) { __c1 = __c2; }

  static char* assign(char* __s, size_t __n, char __c)
    { memset(__s, __c, __n); return __s; }
};

// Specialization for wchar_t.

__STL_TEMPLATE_NULL struct char_traits<wchar_t>
  : public __char_traits_base<wchar_t, wint_t>
{};

__STL_END_NAMESPACE

#endif /* __SGI_STL_CHAR_TRAITS_H */

// Local Variables:
// mode:C++
// End:

