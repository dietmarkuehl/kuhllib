// -*-C++-*- src/cctraits.cc
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
// Title:   Implementation of char_traits<char> members 
// Version: $Id: cctraits.cpp,v 1.1.1.1 2002/06/05 01:00:14 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ctraits.h>
#include <cstring>

// -------------------------------------------------------------------------- 

#if 0
int
_CXXRT_STD_NAME:: char_traits<char>::compare(char const *s1,
			   char const *s2,
			   size_t n)
{
  return strncmp(s1, s2, n);
}

size_t
_CXXRT_STD_NAME:: char_traits<char>::length(char const *s)
{
  return strlen(s);
}

#if 0
char const *
_CXXRT_STD_NAME:: char_traits<char>::find(char const *s, size_t n, char const &c)
{
	char const* rc = static_cast<char*>(memchr(s, c, n));
	return rc? rc: s + n;
}
#endif

char *
_CXXRT_STD_NAME:: char_traits<char>::move(char *s1, char const *s2, size_t n)
{
  return static_cast<char*>(memmove(s1, s2, n));
}

#if 0
char *
_CXXRT_STD_NAME:: char_traits<char>::copy(char *s1, char const *s2, size_t n)
{
	return static_cast<char*>(memcpy(s1, s2, n));
}
#endif

char *
_CXXRT_STD_NAME:: char_traits<char>::assign(char *s, size_t n, char const &c)
{
	memset(s, c, n);
	return s + n;
}

char *
_CXXRT_STD_NAME:: char_traits<char>::set(char *s, size_t n, char const &c)
{
  s[n] = c;
  return s;
}
#endif
