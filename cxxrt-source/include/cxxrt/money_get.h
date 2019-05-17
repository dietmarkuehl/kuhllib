// -*-C++-*- cxxrt/money_get.h
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
// Title:   Declarations for the money_get facet
// Version: $Id: money_get.h,v 1.1.1.1 2002/06/05 01:01:58 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_MONEY_GET_H__)
#define _CXXRT_MONEY_GET_H__ 1

#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif 
#if !defined(_CXXRT_SBIT_FWD_H__)
#  include <cxxrt/sbit_fwd.h>
#endif 
#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif 

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT,
                class _CS_It _CXXRT_TEMPLATE_DEF_ARG(istreambuf_iterator<_CS_cT>) >
class money_get: public locale::facet
{
public:
        typedef _CS_cT               char_type;
        typedef _CS_It               iter_type;
        typedef basic_string<_CS_cT> string_type;

        static locale::id id;

        _CXXRT_EXPLICIT money_get(size_t = 0);

        iter_type get(iter_type, iter_type, bool, ios_base&, ios_base::iostate&, long double&) const;
        iter_type get(iter_type, iter_type, bool, ios_base&, ios_base::iostate&, string_type&) const;

protected:
        ~money_get();

        virtual iter_type do_get(iter_type, iter_type, bool, ios_base&, ios_base::iostate&, long double&) const;
        virtual iter_type do_get(iter_type, iter_type, bool, ios_base&, ios_base::iostate&, string_type&) const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<money_get<char> >(locale const&) 
{
  return true;
}

template <>
inline money_get<char> const& use_facet<money_get<char> >(locale const& _CS_l)
{
  return _CS_use_facet<money_get<char> >(_CS_l);
}

template <>
inline bool has_facet<money_get<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline money_get<wchar_t> const& use_facet<money_get<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<money_get<wchar_t> >(_CS_l);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_MONEY_GET_CC__)
#  include <srccxx/money_get.cc>
#endif

#endif /* _CXXRT_MONEY_GET_H__ */
