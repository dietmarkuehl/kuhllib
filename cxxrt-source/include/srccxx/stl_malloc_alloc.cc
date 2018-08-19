/*
 * Copyright (c) 1996-1997
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

#ifndef __SRCCXX_STL_MALLOC_ALLOC_CC__
#define __SRCCXX_STL_MALLOC_ALLOC_CC__

#if !defined(__SGI_STL_INTERNAL_ALLOC_H)
#  include <stl_alloc.h>
#endif
#include <cstdlib>
#include <cstring>
#include <cassert>

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#endif

// malloc_alloc out-of-memory handling

#ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
template <int __inst>
void (* __malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;
#endif

template <int __inst>
void*
__malloc_alloc_template<__inst>::_S_oom_malloc(_CXXRT_STD size_t __n)
{
    void (* __my_malloc_handler)();
    void* __result;

    for (;;) {
        __my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*__my_malloc_handler)();
        __result = _CXXRT_STD malloc(__n);
        if (__result) return(__result);
    }
}

template <int __inst>
void* __malloc_alloc_template<__inst>::_S_oom_realloc(void* __p, _CXXRT_STD size_t __n)
{
    void (* __my_malloc_handler)();
    void* __result;

    for (;;) {
        __my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*__my_malloc_handler)();
        __result = _CXXRT_STD realloc(__p, __n);
        if (__result) return(__result);
    }
}

template <int __inst>
void* __malloc_alloc_template<__inst>::allocate(_CXXRT_STD size_t __n)
{
  void* __result = _CXXRT_STD malloc(__n);
  if (0 == __result) __result = _S_oom_malloc(__n);
  return __result;
}

template <int __inst>
void __malloc_alloc_template<__inst>::deallocate(void* __p, _CXXRT_STD size_t /* __n */)
{
  _CXXRT_STD free(__p);
}

template <int __inst>
void* __malloc_alloc_template<__inst>::reallocate(void* __p, _CXXRT_STD size_t /* old_sz */, _CXXRT_STD size_t __new_sz)
{
  void* __result = _CXXRT_STD realloc(__p, __new_sz);
  if (0 == __result) __result = _S_oom_realloc(__p, __new_sz);
  return __result;
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#endif

__STL_END_NAMESPACE

#undef __PRIVATE

#endif /* __SRCCXX_STL_MALLOC_ALLOC_CC__ */

// Local Variables:
// mode:C++
// End:
