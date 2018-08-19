// -*-C++-*- srccxx/locale.cc
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
// Title:   Implementation of the locale specific cache member functions
// Version: $Id: lcache.cc,v 1.1.1.1 2002/06/05 01:02:25 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 
#include <cstdio>

#if !defined(__SRCCXX_LOCALE_CC__)
#define __SRCCXX_LOCALE_CC__ 1

#if !defined(_CXXRT_LCACHE_H__)
#  include "cxxrt/lcache.h"
#endif
#if !defined(_CXXRT_ALGORITHM_H__)
#  include "cxxrt/algorithm.h"
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT>
locale::id _CS_cache<_CS_cT>::id;

template <class _CS_cT>
_CS_cache<_CS_cT>::_CS_cache():
  _CS_m_grouping()
{
}

template <class _CS_cT>
_CS_cache<_CS_cT>::~_CS_cache()
{
}

// --------------------------------------------------------------------------

namespace {
  template <class _CS_cT>
  struct _CS_sort_val
    {
      _CS_cT _CS_wide;
      char         _CS_val;
      bool operator< (_CS_sort_val const& _CS_sv) const
        {
          return _CS_wide < _CS_sv._CS_wide;
        }
      bool operator== (_CS_sort_val const& _CS_sv) const
        {
          return _CS_wide == _CS_sv._CS_wide;
        }
    };

  char const* _CS_lt = "0123456789abcdef+-bx,.";
  char const* _CS_ut = "0123456789ABCDEF+-BX,.";
  char const* const _CS_st = "0123456789abcdefABCDEF+-xX \t\n,.";

  char const _CS_ivals[32] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 10, 11, 12, 13, 14, 15,
    _CS_cache_base::_CS_plus, _CS_cache_base::_CS_minus,
    _CS_cache_base::_CS_hex, _CS_cache_base::_CS_hex, 
    _CS_cache_base::_CS_space, _CS_cache_base::_CS_space,
    _CS_cache_base::_CS_space,
    _CS_cache_base::_CS_sep, _CS_cache_base::_CS_point
  };

  template <class _CS_cT>
  inline void
    _CS_setup_scan_table(_CS_cT* _CS_chars, char* _CS_vals)
    {
      _CS_sort_val<_CS_cT> _CS_array[31];
      for (int _CS_i1 = 0; _CS_i1 < 31; ++_CS_i1)
        {
          _CS_array[_CS_i1]._CS_wide = _CS_chars[_CS_i1];
          _CS_array[_CS_i1]._CS_val  = _CS_ivals[_CS_i1];
        }

      _CS_quicksort(_CS_array, _CS_array + 31);

      for (int _CS_i2 = 0; _CS_i2 < 31; ++_CS_i2)
        {
          _CS_chars[_CS_i2] = _CS_array[_CS_i2]._CS_wide;
          _CS_vals[_CS_i2]  = _CS_array[_CS_i2]._CS_val;
        }
    }
}

// --------------------------------------------------------------------------

template <class _CS_cT>
locale::facet const*
_CS_cache<_CS_cT>::_CS_init(locale::_CS_intern const& _CS_i) const
{
  typedef numpunct<_CS_cT> _CS_NP;
  typedef ctype<_CS_cT>    _CS_CT;

  locale::facet const* _CS_f = _CS_i._CS_find(_CS_NP::id);
  _CS_NP const* _CS_np = static_cast<_CS_NP const*>(_CS_f);
  if (_CS_np == 0)
    return 0;

  _CS_f = _CS_i._CS_find(_CS_CT::id);
  _CS_CT const* _CS_ct = static_cast<_CS_CT const*>(_CS_f);
  if (_CS_ct == 0)
    return 0;

  _CS_m_grouping = _CS_np->grouping();

  _CS_ct->widen(_CXXRT_STD _CS_st, _CXXRT_STD _CS_st + 30, _CS_m_scan_table);
  _CS_ct->widen(_CXXRT_STD _CS_lt, _CXXRT_STD _CS_lt + 20, _CS_m_lower_table);
  _CS_ct->widen(_CXXRT_STD _CS_ut, _CXXRT_STD _CS_ut + 20, _CS_m_upper_table);

  _CS_cT _CS_ts = _CS_np->thousands_sep();
  _CS_cT _CS_dp = _CS_np->decimal_point();
  _CS_m_scan_table[29] = _CS_ts;
  _CS_m_scan_table[30] = _CS_dp;
  _CS_m_lower_table[20] = _CS_ts;
  _CS_m_lower_table[21] = _CS_dp;
  _CS_m_upper_table[20] = _CS_ts;
  _CS_m_upper_table[21] = _CS_dp;

  _CXXRT_STD _CS_setup_scan_table(_CS_m_scan_table, _CS_m_vals_table);

  return this;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
locale::facet const*
_CS_cache<_CS_cT>::_CS_object() const
{
  return new _CS_cache;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
int
_CS_cache<_CS_cT>::_CS_find(_CS_cT _CS_c) const
{
  _CS_cT const* _CS_fnd = _CS_m_scan_table + 15;
  if (*_CS_fnd != _CS_c)
    if (*(_CS_fnd += *_CS_fnd < _CS_c? 8: -8) != _CS_c)
      if (*(_CS_fnd += *_CS_fnd < _CS_c? 4: -4) != _CS_c)
        if (*(_CS_fnd += *_CS_fnd < _CS_c? 2: -2) != _CS_c)
          _CS_fnd += *_CS_fnd < _CS_c? 1: -1;

  return *_CS_fnd == _CS_c? _CS_m_vals_table[_CS_fnd - _CS_m_scan_table]: 22;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_LOCALE_CC__ */
