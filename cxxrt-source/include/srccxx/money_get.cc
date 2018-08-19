// -*-C++-*- srccxx/money_get.cc
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
// Title:   Implementations for the srccxx/money_get category
// Version: $Id: money_get.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_MONEY_GET_CC__)
#define __SRCCXX_MONEY_GET_CC__ 1

#if !defined(_CXXRT_MONEY_GET_H__)
#  include <cxxrt/money_get.h>
#endif
#if !defined(_CXXRT_SBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
locale::id money_get<_CS_cT, _CS_It>::id;

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
money_get<_CS_cT, _CS_It>::money_get(size_t _CS_refs):
  locale::facet(_CS_refs)
{
}

template <class _CS_cT, class _CS_It>
money_get<_CS_cT, _CS_It>::~money_get()
{
}

template <class _CS_cT, class _CS_It>
typename money_get<_CS_cT, _CS_It>::iter_type
money_get<_CS_cT, _CS_It>::get(iter_type          _CS_b,
			       iter_type          _CS_e,
			       bool               _CS_intl,
			       ios_base&          _CS_fmt,
			       ios_base::iostate& _CS_err,
			       long double&       _CS_units) const
{
  return do_get(_CS_b, _CS_e, _CS_intl, _CS_fmt, _CS_err, _CS_units);
}

template <class _CS_cT, class _CS_It>
typename money_get<_CS_cT, _CS_It>::iter_type
money_get<_CS_cT, _CS_It>::get(iter_type          _CS_b,
			       iter_type          _CS_e,
			       bool               _CS_intl,
			       ios_base&          _CS_fmt,
			       ios_base::iostate& _CS_err,
			       string_type&       _CS_digits) const
{
  return do_get(_CS_b, _CS_e, _CS_intl, _CS_fmt, _CS_err, _CS_digits);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
typename money_get<_CS_cT, _CS_It>::iter_type
money_get<_CS_cT, _CS_It>::do_get(iter_type _CS_b,
				  iter_type,
				  bool,
				  ios_base&,
				  ios_base::iostate&,
				  long double&) const
{
  return _CS_b;
}

template <class _CS_cT, class _CS_It>
typename money_get<_CS_cT, _CS_It>::iter_type
money_get<_CS_cT, _CS_It>::do_get(iter_type _CS_b,
				  iter_type,
				  bool,
				  ios_base&,
				  ios_base::iostate&,
				  string_type&) const
{
  return _CS_b;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_MONEY_GET_CC__ */
