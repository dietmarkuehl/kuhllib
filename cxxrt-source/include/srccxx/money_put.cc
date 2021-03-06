// -*-C++-*- srccxx/money_put.cc
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
// Title:   Implementations for the srccxx/money_put category
// Version: $Id: money_put.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_MONY_PUT_CC__)
#define __SRCCXX_MONY_PUT_CC__ 1

#if !defined(_CXXRT_MONY_PUT_H__)
#  include <cxxrt/money_put.h>
#endif
#if !defined(_CXXRT_SBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
locale::id money_put<_CS_cT, _CS_It>::id;

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
money_put<_CS_cT, _CS_It>::money_put(size_t _CS_refs):
  locale::facet(_CS_refs)
{
}

template <class _CS_cT, class _CS_It>
money_put<_CS_cT, _CS_It>::~money_put()
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
typename money_put<_CS_cT, _CS_It>::iter_type
money_put<_CS_cT, _CS_It>::put(iter_type _CS_b,
			       bool _CS_intl,
			       ios_base& _CS_fmt,
			       char_type _CS_fill,
			       long double _CS_units) const
{
  return do_put(_CS_b, _CS_intl, _CS_fmt, _CS_fill, _CS_units);
}

template <class _CS_cT, class _CS_It>
typename money_put<_CS_cT, _CS_It>::iter_type
money_put<_CS_cT, _CS_It>::put(iter_type _CS_b,
			       bool _CS_intl,
			       ios_base& _CS_fmt,
			       char_type _CS_fill,
			       string_type const& _CS_units) const
{
  return do_put(_CS_b, _CS_intl, _CS_fmt, _CS_fill, _CS_units);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
typename money_put<_CS_cT, _CS_It>::iter_type
money_put<_CS_cT, _CS_It>::do_put(iter_type _CS_b,
				  bool,
				  ios_base&,
				  char_type,
				  long double) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename money_put<_CS_cT, _CS_It>::iter_type
money_put<_CS_cT, _CS_It>::do_put(iter_type _CS_b,
				  bool,
				  ios_base&,
				  char_type,
				  string_type const&) const
{
  return _CS_b; //-dk:TODO
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_MONY_PUT_CC__ */
