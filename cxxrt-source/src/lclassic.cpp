// -*-C++-*- lclassic.cc
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
// Title:   Implementation of the "classic" locale
// Version: $Id: lclassic.cpp,v 1.1.1.1 2002/06/05 01:00:23 dietmar_kuehl Exp $

// The classic C locale is the only required locale in the standard C++
// library. It contains a bunch of facets which are constructed here.
// -------------------------------------------------------------------------- 

#include <locale>
#include <cxxrt/string.h>
#include <cxxrt/lcache.h>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

locale::_CS_intern*
// locale::locale::_CS_classic_rep()
_CS_classic_rep()
{
  static locale::_CS_intern* rc = 0;
  static bool              init = true;

  if (init)
    {
      rc = new locale::_CS_intern("C");
      ++rc->_CS_m_refcount; // one for the reference "rc"...
      ++rc->_CS_m_refcount; // ... and one for the classic locale

#if !defined(_CXXRT_USE_FACET_INSTALLS)
      numpunct<char> *np1 = new numpunct<char>;
      np1->_CS_preprocess();
      rc->_CS_add(np1);

      // rc->_CS_add(new num_put<char, char*>);
      // rc->_CS_add(new num_get<char, char const *>);

      rc->_CS_add(new numpunct<wchar_t>);
      rc->_CS_add(new _CXXRT_STD collate<char>);
      rc->_CS_add(new _CXXRT_STD collate<wchar_t>);

      rc->_CS_add(new _CXXRT_STD ctype<char>);
      rc->_CS_add(new _CXXRT_STD ctype<wchar_t>);
      rc->_CS_add(new codecvt<char, char, mbstate_t>);
      rc->_CS_add(new codecvt<wchar_t, char, mbstate_t>);
      rc->_CS_add(new moneypunct<char, false>);
      rc->_CS_add(new moneypunct<char, true>);
      //-dk:TOD rc->_CS_add(new moneypunct<wchar_t, false>);
      //-dk:TOD rc->_CS_add(new moneypunct<wchar_t, true>);
      rc->_CS_add(new money_get<char>);
      rc->_CS_add(new money_get<wchar_t>);
      rc->_CS_add(new money_put<char>);
      rc->_CS_add(new money_put<wchar_t>);
      rc->_CS_add(new num_get<char>);
      rc->_CS_add(new num_get<wchar_t>);
      rc->_CS_add(new num_put<char>);
      rc->_CS_add(new num_put<wchar_t>);
      rc->_CS_add(new time_get<char>);
      rc->_CS_add(new time_get<wchar_t>);
      rc->_CS_add(new time_put<char>);
      rc->_CS_add(new time_put<wchar_t>);
      rc->_CS_add(new _CXXRT_STD messages<char>);
      rc->_CS_add(new _CXXRT_STD messages<wchar_t>);

      // the next two facets should be installed by the numpunct facet...
      rc->_CS_add(new _CS_cache<char>);
      rc->_CS_add(new _CS_cache<wchar_t>);
#endif

      init = false;
    }

  rc->_CS_init();

  return rc;
}

locale const &locale::classic()
{
  static locale rc(_CS_classic_rep());
  return rc;
}

// -----------------------------------------------------------------------------
_CXXRT_NAMESPACE_END
