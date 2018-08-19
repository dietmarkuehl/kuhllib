// -*-C++-*- wccvt.cpp
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
// Title:  Implementation of missing wide character conversion function
// Version: $Id: wccvt.cpp,v 1.1.1.1 2002/06/05 01:00:06 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include <cerrno>
#include <cstdio>
#include <cwchar>
#include <stdexcept>
#include <string>
#include <cxxrt/codecvt.h>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

static codecvt<wchar_t, char, mbstate_t> const* _CXXRT_cvt(bool reset = false)
{
  static codecvt<wchar_t, char, mbstate_t> const* rc = 0;
  if (rc == 0 || reset)
    rc = &use_facet<codecvt<wchar_t, char, mbstate_t> >(locale());
  return rc;
}

// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_HAS_BTOWC) || defined(_CXXRT_WCHAR_IMPL)
wint_t btowc(int c)
{
  if (c == EOF)
    return WEOF;

  mbstate_t   state = mbstate_t();
  char const  in = static_cast<unsigned char>(c);
  char const* inext;
  wchar_t     out;
  wchar_t*    onext;
  
  switch (_CXXRT_cvt()->in(state, &in, &in + 1, inext, &out, &out + 1, onext))
    {
    case codecvt_base::ok:
      return out;
    default:
      return WEOF;
    }
}
#endif

// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_HAS_WCTOB) || defined(_CXXRT_WCHAR_IMPL)
int wctob(wint_t c)
{
  if (c == WEOF)
    return EOF;

  mbstate_t      state = mbstate_t();
  wchar_t const  in = static_cast<wchar_t>(c);
  wchar_t const* inext;
  char           out;
  char*          onext;
  
  switch (_CXXRT_cvt()->out(state, &in, &in + 1, inext, &out, &out + 1, onext))
    {
    case codecvt_base::ok:
      return out;
    default:
      return EOF;
    }
}
#endif

// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_HAS_MBSINIT) || defined(_CXXRT_WCHAR_IMPL)
int mbsinit(mbstate_t const* state)
{
  return (state == 0 || *state == mbstate_t())? 1: 0;
}
#endif


// -------------------------------------------------------------------------- 

#if 0 && (!defined(_CXXRT_HAS_MBRLEN) || defined(_CXXRT_WCHAR_IMPL))
size_t mbrlen(char const* _CXXRT_RESTRICT s, size_t n,
	       mbstate_t* _CXXRT_RESTRICT state)
{
  static mbstate_t intern = mbstate_t();
  return _CXXRT_STD mbrtowc(0, s, n, state? state: &intern);
}
#endif

// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_HAS_MBRTOWC) || defined(_CXXRT_WCHAR_IMPL)
size_t mbrtowc(wchar_t* _CXXRT_RESTRICT to,
	       char const* _CXXRT_RESTRICT s, size_t n,
	       mbstate_t* _CXXRT_RESTRICT state)
{
  static mbstate_t intern = mbstate_t();
  if (s == 0)
    return _CXXRT_STD mbrtowc(0, "", 1, state);

  char const* fnext;
  wchar_t*    tnext;
  wchar_t     tmp;

  if (to == 0)
    to = &tmp;

  switch (_CXXRT_cvt()->in(state? *state: intern, s, s + n, fnext, to, to + 1, tnext))
    {
    case codecvt_base::ok:
      return to == wchar_t()? 0: fnext - s;
    case codecvt_base::partial:
      return size_t(-2);
    default:
      errno = EILSEQ;
      return size_t(-1);
    }
}
#endif

// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_HAS_WCTOMBR) || defined(_CXXRT_WCHAR_IMPL)
size_t wctombr(char* _CXXRT_RESTRICT to,
	       wchar_t from, mbstate_t* _CXXRT_RESTRICT state)
{
  static mbstate_t intern = mbstate_t();
  if (to == 0)
    {
      static char* buf = new char[MB_CUR_MAX]; //-dk:TODO
      return _CXXRT_STD wctombr(buf, L'\0', state);
    }

  wchar_t const* fnext;
  char*          tnext;

  switch (_CXXRT_cvt()->out(state? *state: intern, &from, &from + 1, fnext,
			    to, to + MB_CUR_MAX, tnext))
    {
    case codecvt_base::ok:
      return tnext - to;
    default:
      errno = EILSEQ;
      return size_t(-1);
    }
}
#endif

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
