// -*-C++-*- wctraits.cc
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
// Title:   Implementation of char_traits<wchar_t> members 
// Version: $Id: wctraits.cpp,v 1.1.1.1 2002/06/05 01:00:08 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ctraits.h>
#include <cxxrt/algorithm.h>
#include <cwchar>

// -------------------------------------------------------------------------- 

#if 0
int
_CXXRT_STD_NAME:: char_traits<wchar_t>::compare(wchar_t const *s1,
			   wchar_t const *s2,
			   size_t n)
{
  return wcsncmp(s1, s2, n);
}

size_t
_CXXRT_STD_NAME:: char_traits<wchar_t>::length(wchar_t const *s)
{
  return wcslen(s);
}

wchar_t const *
_CXXRT_STD_NAME:: char_traits<wchar_t>::find(wchar_t const *s, size_t n, wchar_t const &c)
{
  wchar_t* rc = _CS_find(s, s + n, c);
  return rc == s + n? 0: rc;
}

wchar_t *
_CXXRT_STD_NAME:: char_traits<wchar_t>::move(wchar_t *s1, wchar_t const *s2, size_t n)
{
  return static_cast<wchar_t*>(wmemmove(s1, s2, n));
}

wchar_t *
_CXXRT_STD_NAME:: char_traits<wchar_t>::copy(wchar_t *s1, wchar_t const *s2, size_t n)
{
  return wcsncpy(s1, s2, n);
}

wchar_t *
_CXXRT_STD_NAME:: char_traits<wchar_t>::assign(wchar_t *s, size_t n, wchar_t const &c)
{
  return _CS_fill_n(s, s + n, c);
}

// --------------------------------------------------------------------------


int _CXXRT_STD_NAME:: char_traits<wchar_t>::not_eof(int const &i)
{
	return i == -1? 0: i;
}

wchar_t _CXXRT_STD_NAME:: char_traits<wchar_t>::to_char_type(int const &c)
{
	return wchar_t(c);
}

int _CXXRT_STD_NAME:: char_traits<wchar_t>::to_int_type(wchar_t const &c)
{
	return int(c);
}
#endif
