// -*-C++-*- wchar.h
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
// Title:   Compatibility header wchar.h
// Version: $Id: wchar.h,v 1.4 2003/04/13 19:35:59 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#define _CXXRT_WCHAR_H_PROCESSING 1

// -------------------------------------------------------------------------- 

#if defined(__GNUG__) && !defined(_CXXRT_WCHAR_H_AUX)
#define _CXXRT_WCHAR_H_AUX
extern "C"
{
#ifndef __wcstoll_internal_defined
__extension__
extern long long int __wcstoll_internal (__const wchar_t *__restrict __nptr,
                                         wchar_t **__restrict __endptr,
                                         int __base, int __group) throw();
# define __wcstoll_internal_defined     1
#endif
#ifndef __wcstoull_internal_defined
__extension__
extern unsigned long long int __wcstoull_internal (__const wchar_t *
                                                   __restrict __nptr,
                                                   wchar_t **
                                                   __restrict __endptr,
                                                   int __base,
                                                   int __group) throw();
# define __wcstoull_internal_defined    1
#endif
}
#endif

// --------------------------------------------------------------------------

extern "C"
{
#define __wcstoll_internal_defined 1
#define __wcstoull_internal_defined 1

#define wcschr(_CS_x, _CS_y) _CS_wcsrchr(_CS_x, _CS_y)
#define wcspbrk(_CS_x, _CS_y) _CS_wcspbrk(_CS_x, _CS_y)
#define wcsrchr(_CS_x, _CS_y) _CS_wcsrchr(_CS_x, _CS_y)
#define wcsstr(_CS_x, _CS_y) _CS_wcsstr(_CS_x, _CS_y)
#define wmemchr(_CS_x,_CS_y,_CS_z) _CS_wmemchr(_CS_x,_CS_y,_CS_z)

#include _CXXRT_C_WCHAR_H

#undef wcschr
#undef wcspbrk
#undef wcsrchr
#undef wcsstr
#undef wmemchr
} // extern "C"

// -------------------------------------------------------------------------- 

#if !defined(__WCHAR_H_CXX__) && !defined(_CXXRT_IN_STDIO_H)
#define __WCHAR_H_CXX__ 1

// -------------------------------------------------------------------------- 

#if _CXXRT_HAS_MBSTATE_T == 0
  struct mbstate_t
  {
    int    count;
    wint_t value;
  };
#endif

#if _CXXRT_HAS_MBSTATE_T_COMPARE == 0
    //-dk:TODO hack!
    typedef __mbstate_t mbstate_t;
  inline bool operator== (mbstate_t, mbstate_t) { return true; }
#endif

// -------------------------------------------------------------------------- 

namespace _CS_swamp
{
  extern "C" wchar_t* wcschr(wchar_t const*, wchar_t);
}
inline wchar_t const* wcschr(wchar_t const* _CS_x, wchar_t _CS_y) { return _CS_swamp::wcschr(_CS_x, _CS_y); }
inline wchar_t*       wcschr(wchar_t*       _CS_x, wchar_t _CS_y) { return _CS_swamp::wcschr(_CS_x, _CS_y); }

// -------------------------------------------------------------------------- 

namespace _CS_swamp
{
  extern "C" wchar_t* wcspbrk(wchar_t const*, wchar_t const*);
}
inline wchar_t const* wcspbrk(wchar_t const* _CS_x, wchar_t const* _CS_y) { return _CS_swamp::wcspbrk(_CS_x, _CS_y); }
inline wchar_t*       wcspbrk(wchar_t*       _CS_x, wchar_t const* _CS_y) { return _CS_swamp::wcspbrk(_CS_x, _CS_y); }

// -------------------------------------------------------------------------- 

namespace _CS_swamp
{
  extern "C" wchar_t* wcsrchr(wchar_t const*, wchar_t);
}
inline wchar_t const* wcsrchr(wchar_t const* _CS_x, wchar_t _CS_y) { return _CS_swamp::wcsrchr(_CS_x, _CS_y); }
inline wchar_t*       wcsrchr(wchar_t*       _CS_x, wchar_t _CS_y) { return _CS_swamp::wcsrchr(_CS_x, _CS_y); }

// -------------------------------------------------------------------------- 

namespace _CS_swamp
{
  extern "C" wchar_t* wcsstr(wchar_t const*, wchar_t const*);
}
inline wchar_t const* wcsstr(wchar_t const* _CS_x, wchar_t const* _CS_y) { return _CS_swamp::wcsstr(_CS_x, _CS_y); }
inline wchar_t*       wcsstr(wchar_t*       _CS_x, wchar_t const* _CS_y) { return _CS_swamp::wcsstr(_CS_x, _CS_y); }

// -------------------------------------------------------------------------- 

namespace _CS_swamp
{
  extern "C" wchar_t* wmemchr(wchar_t const*, wchar_t, size_t);
}
inline wchar_t const* wmemchr(wchar_t const* _CS_x, wchar_t _CS_y, size_t _CS_z) { return _CS_swamp::wmemchr(_CS_x, _CS_y, _CS_z); }
inline wchar_t*       wmemchr(wchar_t*       _CS_x, wchar_t _CS_y, size_t _CS_z) { return _CS_swamp::wmemchr(_CS_x, _CS_y, _CS_z); }

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

  using ::mbstate_t;
//  using ::size_t;
  using ::wint_t;

#if defined(_CXXRT_HAS_BTOWC) && !defined(_CXXRT_WCHAR_IMPL)
  using ::btowc;
#else
  extern "C" wint_t btowc(int) throw();
#endif

#if defined(_CXXRT_HAS_WCTOB) && !defined(_CXXRT_WCHAR_IMPL)
  using ::wctob;
#else
  extern "C" int wctob(wint_t) throw();
#endif

#if defined(_CXXRT_HAS_MBSINIT) && !defined(_CXXRT_WCHAR_IMPL)
  using ::mbsinit;
#else
  extern "C" int mbsinit(const mbstate_t*) throw();
#endif

#if defined(_CXXRT_HAS_MBRLEN) && !defined(_CXXRT_WCHAR_IMPL)
  using ::mbrlen;
#else
  extern "C" size_t mbrlen(const char* _CXXRT_RESTRICT, size_t,
                           mbstate_t* _CXXRT_RESTRICT) throw();
#endif

#if defined(_CXXRT_HAS_MBRTOWC) && !defined(_CXXRT_WCHAR_IMPL)
  using ::mbrtowc;
#else
  extern "C" size_t mbrtowc(wchar_t* _CXXRT_RESTRICT,
                            const char* _CXXRT_RESTRICT, size_t,
                            mbstate_t* _CXXRT_RESTRICT) throw();
#endif

#if defined(_CXXRT_HAS_WCTOMBR) && !defined(_CXXRT_WCHAR_IMPL)
  using ::wctombr;
#else
  extern "C" size_t wctombr(char* _CXXRT_RESTRICT,
                            wchar_t, mbstate_t* _CXXRT_RESTRICT);
#endif

#if 1
  //-dk:TODO add specific tests. for now this stuff is simply disabled!
  //using ::FILE;

  //using ::fgetwc;
  //using ::fgetws;
  //using ::fputwc;
  //using ::fputws;
  //using ::fwide;
  //using ::fwprintf;
  //using ::fwscanf;
  //using ::getwc;
  //using ::getwchar;
  using ::mbsrtowcs;
  //using ::putwc;
  //using ::putwchar;
  //using ::swprintf;
  //using ::swscanf;
  //using ::ungetwc;
  //using ::vfwprintf;
  //using ::vswprintf;
  //using ::vwprintf;
  using ::wcrtomb;
  using ::wcscat;
  using ::wcschr;
  using ::wcscmp;
  using ::wcscoll;
  using ::wcscpy;
  using ::wcscspn;
  //using ::wcsftime;
  using ::wcslen;
  using ::wcsncat;
  using ::wcsncmp;
  using ::wcsncpy;
  using ::wcspbrk;
  using ::wcsrchr;
  using ::wcsrtombs;
  using ::wcsspn;
  using ::wcsstr;
  using ::wcstod;
  using ::wcstok;
  using ::wcstol;
  using ::wcstoul;
  using ::wcsxfrm;
  using ::wctob;
  using ::wmemchr;
  using ::wmemcmp;
  using ::wmemcpy;
  using ::wmemmove;
  using ::wmemset;
  //using ::wprintf;
  //using ::wscanf; 
#endif

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#endif /* __WCHAR_H_CXX__ */
#undef _CXXRT_WCHAR_H_PROCESSING
