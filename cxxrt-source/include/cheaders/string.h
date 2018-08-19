// -*-C++-*- string.h
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
// Title:   Wrapper for the C library's string.h
// Version: $Id: string.h,v 1.4 2003/04/13 19:35:59 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(__STRING_H_CXX__)
#define __STRING_H_CXX__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
extern "C"
{
#define strchr(_CS_x, _CS_y) _CS_strchr(_CS_x, _CS_y)
#define strrchr(_CS_x, _CS_y) _CS_strchr(_CS_x, _CS_y)
#define strpbrk(_CS_x, _CS_y) _CS_strpbrk(_CS_x, _CS_y)
#define strstr(_CS_x, _CS_y) _CS_strpbrk(_CS_x, _CS_y)

#include _CXXRT_C_STRING_H

#undef strchr
#undef strrchr
#undef strpbrk
#undef strstr
} // extern "C"

inline int    _CS_memcmp(void const* _CS_s1, void const* _CS_s2, size_t _CS_n)
{ return memcmp(_CS_s1, _CS_s2, _CS_n); }
inline void*  _CS_memcpy(void* _CS_d, void const* _CS_s, size_t _CS_n)
{ return memcpy(_CS_d, _CS_s, _CS_n); }
inline int    _CS_strcmp(char const* _CS_s1, char const* _CS_s2)
{ return strcmp(_CS_s1, _CS_s2); }
inline char*  _CS_strcpy(char* _CS_d, char const* _CS_s)
{ return strcpy(_CS_d, _CS_s); }
inline size_t _CS_strlen(char const* _CS_s)
{ return strlen(_CS_s); }

#if defined(memcmp)
#  undef memcmp
#endif
#if defined(memcpy)
#  undef memcpy
#endif
#if defined(strcmp)
#  undef strcmp
#endif
#if defined(strcpy)
#  undef strcpy
#endif
#if defined(strlen)
#  undef strlen
#endif
#if defined(__GNUG__)
#  define _CXXRT_STRFCT_THROW throw()
#else
#  define _CXXRT_STRFCT_THROW throw()
#endif

//extern void const* memchr(void const*, int, size_t);
// extern "C" void* memchr(void*, int, size_t);
namespace _CS_swamp
{
  extern "C" char* strchr(char const*, int);
  extern "C" char* strrchr(char const*, int);
  extern "C" char* strpbrk(char const*, char const*);
  extern "C" char* strstr(char const*, char const*);
}
inline char const* strchr(char const* _CS_x, int _CS_y) { return _CS_swamp::strchr(_CS_x, _CS_y); }
inline char*       strchr(char* _CS_x, int _CS_y)       { return _CS_swamp::strchr(_CS_x, _CS_y); }
inline char const* strrchr(char const* _CS_x, int _CS_y) { return _CS_swamp::strrchr(_CS_x, _CS_y); }
inline char*       strrchr(char*       _CS_x, int _CS_y) { return _CS_swamp::strrchr(_CS_x, _CS_y); }
inline char const* strpbrk(char const* _CS_x, char const* _CS_y) { return _CS_swamp::strpbrk(_CS_x, _CS_y); }
inline char*       strpbrk(char*       _CS_x, char const* _CS_y) { return _CS_swamp::strpbrk(_CS_x, _CS_y); }
inline char const* strstr(char const* _CS_x, char const* _CS_y) { return _CS_swamp::strstr(_CS_x, _CS_y); }
inline char*       strstr(char*       _CS_x, char const* _CS_y) { return _CS_swamp::strstr(_CS_x, _CS_y); }

extern void* memmove(void*, void const*, size_t) _CXXRT_STRFCT_THROW;
extern void* memset(void*, int, size_t) _CXXRT_STRFCT_THROW;

extern char*  strcat(char*, char const*) _CXXRT_STRFCT_THROW;
extern int    strcoll(char const*, char const*) _CXXRT_STRFCT_THROW;
extern size_t strcspn(char const*, char const*) _CXXRT_STRFCT_THROW;
extern char*  strerror(int) _CXXRT_STRFCT_THROW;
extern char*  strncat(char*, char const*, size_t) _CXXRT_STRFCT_THROW;
extern int    strncmp(char const*, char const*, size_t) _CXXRT_STRFCT_THROW;
extern char*  strncpy(char*, char const*, size_t) _CXXRT_STRFCT_THROW;
//extern char*  strspn(char const*, char const*);
extern char*  strtok(char*, char const*) _CXXRT_STRFCT_THROW;
//extern char*  strxfrm(char*, char const*, size_t);

extern int    memcmp(void const*, void const*, size_t) _CXXRT_STRFCT_THROW;
extern void*  memcpy(void*, void const*, size_t) _CXXRT_STRFCT_THROW;
extern int    strcmp(char const*, char const*) _CXXRT_STRFCT_THROW;
extern char*  strcpy(char*, char const*) _CXXRT_STRFCT_THROW;
extern size_t strlen(char const*) _CXXRT_STRFCT_THROW;

extern int    _CS_memcmp(void const*, void const*, size_t);
extern void*  _CS_memcpy(void*, void const*, size_t);
extern int    _CS_strcmp(char const*, char const*);
extern char*  _CS_strcpy(char*, char const*);
extern size_t _CS_strlen(char const*);

_CXXRT_NAMESPACE_BEGIN

  //using ::size_t;

  inline void const* memchr(void const* _CS_p, int _CS_c, size_t _CS_n)
    { return ::memchr(_CS_p, _CS_c, _CS_n); }
  inline void* memchr(void* _CS_p, int _CS_c, size_t _CS_n)
    { return const_cast<void*>(::memchr(_CS_p, _CS_c, _CS_n)); }

  using ::strchr;
  using ::strrchr;
  using ::strpbrk;
  using ::strstr;

  using ::memmove;
  using ::memset;

  using ::strcat;
  using ::strcoll;
  using ::strcspn;
  using ::strerror;
  using ::strncat;
  using ::strncmp;
  using ::strncpy;
  using ::strspn;
  using ::strtok;
  using ::strxfrm;

#if 0
  inline int    memcmp(void const* _CS_s1, void const* _CS_s2, size_t _CS_n)
    { return ::_CS_memcmp(_CS_s1, _CS_s2, _CS_n); }
  inline void*  memcpy(void* _CS_d, void const* _CS_s, size_t _CS_n)
    { return ::_CS_memcpy(_CS_d, _CS_s, _CS_n); }
  inline int    strcmp(char const* _CS_s1, char const* _CS_s2)
    { return ::_CS_strcmp(_CS_s1, _CS_s2); }
  inline char*  strcpy(char* _CS_d, char const* _CS_s)
    { return ::_CS_strcpy(_CS_d, _CS_s); }
  inline size_t strlen(char const* _CS_s)
    { return ::_CS_strlen(_CS_s); }
#else
  using ::memcmp;
  using ::memcpy;
  using ::strcmp;
  using ::strcpy;
  using ::strlen;
#endif

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#endif /* __STRING_H_CXX__ */
