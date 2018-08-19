// -*-C++-*- src/cctype.cc
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
// Title:   Implementation of 'ctype<char>'
// Version: $Id: cctype.cpp,v 1.1.1.1 2002/06/05 01:00:06 dietmar_kuehl Exp $
// ---------------------------------------------------------------------------

#include "srccxx/ctype.cc"
#include "cxxrt/c_locale.h"
#include <cstdlib>
#include <cxxrt/algorithm.h>

extern _Locale_mask_t _CS_char_mask[257];
extern char _CS_tolower[];
extern char _CS_toupper[];

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 0
ctype_base::mask const* _CS_convert_mask()
{
  static ctype_base::mask _CS_rc[257];
  static bool             _CS_init = true;

  if (_CS_init)
    {
      _CS_init = false;
      for (int _CS_i = 0; _CS_i < 257; ++_CS_i)
	_CS_rc[_CS_i] = ctype_base::mask(_CS_char_mask[_CS_i]);
    }
  return _CS_rc;
}

static ctype_base::mask const* _CS_converted_mask = _CS_convert_mask();
#endif

// ---------------------------------------------------------------------------

// template class ctype<char>;
locale::id ctype<char>::id;

// ---------------------------------------------------------------------------

ctype<char>::~ctype()
{
  if (_CS_m_owner)
    delete[] const_cast<mask*>(_CS_m_tab);
}

// ---------------------------------------------------------------------------

ctype_base::mask const *ctype<char>::classic_table() throw()
{
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
  return _CS_char_mask + 1;
#else
  return _CS_convert_mask() + 1;
#endif
}

// ---------------------------------------------------------------------------

inline char intern_tolower(char c) { return _CS_tolower[static_cast<unsigned char>(c)]; }
inline char intern_toupper(char c) { return _CS_toupper[static_cast<unsigned char>(c)]; }

// -------------------------------------------------------------------------

ctype<char>::ctype(ctype_base::mask const *tab, bool del, size_t refs):
    locale::facet(refs),
    _CS_m_tab(tab),
    _CS_m_owner(del)
  {
    if (tab == 0)
      {
        _CS_m_tab = classic_table();
        _CS_m_owner = false;
      }
  }

// -----------------------------------------------------------------------------

#if 0
bool ctype<char>::is(ctype_base::mask _mask, char c) const
{
    return (_CS_m_tab[static_cast<unsigned char>(c)] & _mask) != 0;
}
#endif

char const *ctype<char>::is(char const *low, char const *high, ctype_base::mask *vec) const
{
  for (; low != high; ++low, ++vec)
    *vec = _CS_m_tab[static_cast<unsigned char>(*low)];
  return low;
}

char const *ctype<char>::scan_is(ctype_base::mask m, char const *low, char const *high) const
{
  while (low != high && !(table()[static_cast<unsigned char>(*low)] & m))
    ++low;
  return low;
}

char const *ctype<char>::scan_not(ctype_base::mask m, char const *low, char const *high) const
{
  while (low != high && (table()[static_cast<unsigned char>(*low)] & m))
    ++low;
  return low;
}

// ---------------------------------------------------------------------------

char const  *ctype<char>::widen(char const *low, char const *high, char *to) const
{
	return do_widen(low, high, to);
}

char const *ctype<char>::narrow(char const *low, char const *high, char dfault, char *to) const
{
	return do_narrow(low, high, dfault, to);
}

// ---------------------------------------------------------------------------

char ctype<char>::do_toupper(char c) const
{
  return intern_toupper(c);
}

char const *ctype<char>::toupper(char *low, char const *high) const
{
	return do_toupper(low, high);
}
char const *ctype<char>::do_toupper(char *low, char const *high) const
{
  for (; low != high; ++low)
    *low = intern_toupper(*low);
  return low;
}

// ---------------------------------------------------------------------------

char ctype<char>::do_tolower(char c) const
{
  return intern_tolower(c);
}

char const *ctype<char>::tolower(char *low, char const *high) const
{
	return do_tolower(low, high);
}
char const *ctype<char>::do_tolower(char *low, char const *high) const
{
  for (; low != high; ++low)
    *low = intern_tolower(*low);
  return low;
}

// ---------------------------------------------------------------------------

char
ctype<char>::do_widen(char c) const
{
	return c;
}

char const *
ctype<char>::do_widen(char const *beg, char const *end, char *to) const
{
	_CS_copy(beg, end, to);
	return end;
}

char
ctype<char>::do_narrow(char c, char) const
{
	return c;
}

char const *
ctype<char>::do_narrow(char const *beg, char const *end, char, char *to) const
{
	_CS_copy(beg, end, to);
	return end;
}

// ---------------------------------------------------------------------------

ctype_byname<char>::ctype_byname(char const *, size_t)
{
}

ctype_byname<char>::~ctype_byname()
{
}

// ---------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
