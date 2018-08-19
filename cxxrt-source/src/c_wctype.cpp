// -*-C++-*- src/c_wctype.cc
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
// Title:   Definition of a low-level interface to the wide ctype facet
// Version: $Id: c_wctype.cpp,v 1.1.1.1 2002/06/05 01:00:15 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include "cxxrt/c_locale.h"

// -------------------------------------------------------------------------- 

_Locale_mask_t _Locale_wchar_ctype(struct _Locale_ctype *, wint_t)
{
	return _Locale_mask_t(); //-dk:TODO
}

wint_t _Locale_wchar_tolower(struct _Locale_ctype *, wint_t)
{
	return 0; //-dk:TODO
}

wint_t _Locale_wchar_toupper(struct _Locale_ctype *, wint_t)
{
	return 0; //-dk:TODO
}

wint_t _Locale_btowc(struct _Locale_ctype *, int)
{
	return 0; //-dk:TODO
}

int _Locale_wctob(struct _Locale_ctype *, wint_t)
{
	return 0; //-dk:TODO
}

_CXXRT_STD size_t _Locale_mbtowc(struct _Locale_ctype *,
                      wchar_t *,
                      const char *, _CXXRT_STD size_t,
                      mbstate_t *)
{
	return 0; //-dk:TODO
}

_CXXRT_STD size_t _Locale_wctomb(struct _Locale_ctype *,
                      char *, _CXXRT_STD size_t,
                      wchar_t,
                      mbstate_t *)
{
	return 0; //-dk:TODO
}

_CXXRT_STD size_t _Locale_unshift(struct _Locale_ctype *,
                       mbstate_t *,
                       char *, _CXXRT_STD size_t, char **)
{
	return 0; //-dk:TODO
}

// -----------------------------------------------------------------------------

int _Locale_mb_cur_max (struct _Locale_ctype *)
{
  return 0; //-dk:TODO
}

int _Locale_mb_cur_min (struct _Locale_ctype *)
{
  return 0; //-dk:TODO
}

int _Locale_is_stateless (struct _Locale_ctype *)
{
  return 0; //-dk:TODO
}

// -----------------------------------------------------------------------------

wint_t _Locale_getwc(struct _Locale_ctype *, FILE *)
{
	return 0; //-dk:TODO
}

wint_t _Locale_putwc(struct _Locale_ctype *, wchar_t, FILE *)
{
	return 0; //-dk:TODO
}

wint_t _Locale_ungetwc(struct _Locale_ctype *, wchar_t, FILE *)
{
	return 0; //-dk:TODO
}

int _Locale_fsetpos(struct _Locale_ctype *, FILE *, const _Locale_filepos *)
{
	return 0; //-dk:TODO
}

int _Locale_fgetpos(struct _Locale_ctype *, FILE *, _Locale_filepos *)
{
	return 0; //-dk:TODO
}
