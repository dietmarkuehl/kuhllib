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

#ifndef _CXXRT_STRING_HASH_H__
#define _CXXRT_STRING_HASH_H__

#include <stl_config.h>
#include <stl_string_fwd.h>
#include <stl_hash_fun.h>

__STL_BEGIN_NAMESPACE

template <class _CharT, class _Traits, class _Alloc>
size_t __stl_string_hash(const basic_string<_CharT,_Traits,_Alloc>& __s) {
  unsigned long __h = 0;
  for (typename basic_string<_CharT,_Traits,_Alloc>::const_iterator __i = __s.begin();
       __i != __s.end();
       ++__i)
    __h = 5*__h + *__i;
  return size_t(__h);
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _CharT, class _Traits, class _Alloc>
struct hash<basic_string<_CharT,_Traits,_Alloc> > {
  size_t operator()(const basic_string<_CharT,_Traits,_Alloc>& __s) const
    { return __stl_string_hash(__s); }
};

#else

__STL_TEMPLATE_NULL struct hash<string> {
  size_t operator()(const string& __s) const
    { return __stl_string_hash(__s); }
};

__STL_TEMPLATE_NULL struct hash<wstring> {
  size_t operator()(const wstring& __s) const
    { return __stl_string_hash(__s); }
};

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

__STL_END_NAMESPACE

#endif /* _CXXRT_STRING_HASH_H__ */


// Local Variables:
// mode:C++
// End:

