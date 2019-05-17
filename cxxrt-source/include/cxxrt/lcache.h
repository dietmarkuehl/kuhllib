// -*-C++-*- cxxrt/lcache.h
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
// Title:   A facet used as cache of a combination of others.
// Version: $Id: lcache.h,v 1.1.1.1 2002/06/05 01:02:11 dietmar_kuehl Exp $ 

// Using a facet as cache needs special processing because it cannot
// simply be copied when instantiating a locale. Actually, this
// special processing is somewhat distributed: The class locale::facet
// has a virtual member function, _CS_object, which returns a
// pointer to the facet for non-cache facets and 0 for cache
// facets. This function is used to copy facets during locale
// construction. In addition, the function use_facet is specialized
// for all cache facets to install the facet in a locale object if it
// is not found.
// --------------------------------------------------------------------------

#if !defined(_CXXRT_LCACHE_H__)
#define _CXXRT_LCACHE_H__ 1

#include "cxxrt/locale.h"
#include "cxxrt/numpunct.h"
#include "cxxrt/ctype.h"
#include "cxxrt/string.h"

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

enum
{
  _CS_exponent_index    = 14,
  _CS_positive_index    = 16,
  _CS_negative_index    = 17,
  _CS_binary_index      = 18,
  _CS_hexadecimal_index = 19,
  _CS_separator_index   = 20,
  _CS_punct_index       = 21
};

class _CS_cache_base
{
 public:
  enum
  {
    _CS_plus  = 16,
    _CS_minus = 17,
    _CS_hex   = 18,
    _CS_space = 19,
    _CS_sep   = 20,
    _CS_point = 21,
    _CS_none  = 22
  };
};

// --------------------------------------------------------------------------

template <class _CS_cT>
class _CS_cache: public locale::facet, public _CS_cache_base
{
public:
  locale::facet const* _CS_init(locale::_CS_intern const&) const;
  locale::facet const* _CS_object() const;

  _CS_cache();
  ~_CS_cache();

  // scanning the cached result of widen():
  int _CS_find(_CS_cT _CS_c) const;
  
  // caching for formatted output:
  _CS_cT const* _CS_lower_table() const { return _CS_m_lower_table; }
  _CS_cT const* _CS_upper_table() const { return _CS_m_upper_table; }
  string const& _CS_grouping() const { return _CS_m_grouping; }

  static locale::id id;
private:
  mutable string _CS_m_grouping;
  // scan table: widened and sorted form of "0123456789abcdefABCDEF+-,.xX \n\t\r"
  mutable _CS_cT _CS_m_scan_table[32];
  // ... and a table with the corresponding values:
  mutable char         _CS_m_vals_table[32];
  // lower table: widened form of "0123456789abcdef+-bx,."
  mutable _CS_cT _CS_m_lower_table[22];
  // upper table: widened form of "0123456789ABCDEF+-BX,."
  mutable _CS_cT _CS_m_upper_table[22];
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<_CS_cache<char> >(locale const&) 
{
  return true;
}

template <>
inline _CS_cache<char> const& use_facet<_CS_cache<char> >(locale const& _CS_l)
{
  locale::facet const* _CS_tmp = _CS_l._CS_m_rep->_CS_find(_CS_cache<char>::id);
  if (_CS_tmp == 0)
    {
      use_facet<numpunct<char> >(_CS_l);
      use_facet<ctype<char> >(_CS_l);
      _CS_cache<char>* _CS_c = new _CS_cache<char>;
      _CS_l._CS_m_rep->_CS_add(_CS_c);
      _CS_c->_CS_init(*_CS_l._CS_m_rep);
      return *_CS_c;
    }
  return static_cast<_CS_cache<char> const&>(*_CS_tmp);
}

template <>
inline bool has_facet<_CS_cache<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline _CS_cache<wchar_t> const& use_facet<_CS_cache<wchar_t> >(locale const& _CS_l)
{
  locale::facet const* _CS_tmp = _CS_l._CS_m_rep->_CS_find(_CS_cache<wchar_t>::id);
  if (_CS_tmp == 0)
    {
      use_facet<numpunct<wchar_t> >(_CS_l);
      use_facet<ctype<wchar_t> >(_CS_l);
      _CS_cache<wchar_t>* _CS_c = new _CS_cache<wchar_t>;
      _CS_l._CS_m_rep->_CS_add(_CS_c);
      _CS_c->_CS_init(*_CS_l._CS_m_rep);
      return *_CS_c;
    }
  return static_cast<_CS_cache<wchar_t> const&>(*_CS_tmp);
}


// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_LCACHE_H__ */
