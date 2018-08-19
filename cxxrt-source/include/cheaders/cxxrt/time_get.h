// -*-C++-*- cxxrt/time_get.h
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
// Title:   Declarations for the time_get facet
// Version: $Id: time_get.h,v 1.1.1.1 2002/06/05 01:01:55 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_TIME_GET_H__)
#define _CXXRT_TIME_GET_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif 
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif 
#if !defined(_CXXRT_SBIT_FWD_H__)
#  include <cxxrt/sbit_fwd.h>
#endif 
#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif 
#if !defined(_CXXRT_TIME_BASE_H__)
#  include <cxxrt/time_base.h>
#endif 
#include <ctime>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT,
          class _CS_It _CXXRT_TEMPLATE_DEF_ARG(istreambuf_iterator<_CS_cT>) >
class time_get: public locale::facet, public time_base
{
public:
  typedef _CS_cT char_type;
  typedef _CS_It iter_type;

  static locale::id id;

  _CXXRT_EXPLICIT time_get(size_t = 0);

  dateorder date_order() const;
  iter_type get_time(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  iter_type get_date(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  iter_type get_weekday(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  iter_type get_monthname(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  iter_type get_year(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;

protected:
  virtual dateorder do_date_order() const;
  virtual iter_type do_get_time(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_date(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_weekday(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_monthname(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_year(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
};

// --------------------------------------------------------------------------

template <class _CS_cT,
          class _CS_It _CXXRT_TEMPLATE_DEF_ARG(istreambuf_iterator<_CS_cT>) >
class time_get_byname: public time_get<_CS_cT, _CS_It>
{
public:
  typedef time_base::dateorder dateorder;
  typedef _CS_cT               char_type;
  typedef _CS_It               iter_type;

  explicit time_get_byname(char const*, size_t = 0);

protected:
  virtual dateorder do_date_order() const;
  virtual iter_type do_get_time(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_date(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_weekday(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_monthname(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
  virtual iter_type do_get_year(iter_type, iter_type, ios_base&, ios_base::iostate&, tm*) const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<time_get<char> >(locale const&) 
{
  return true;
}

template <>
inline time_get<char> const& use_facet<time_get<char> >(locale const& _CS_l)
{
  return _CS_use_facet<time_get<char> >(_CS_l);
}

template <>
inline bool has_facet<time_get<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline time_get<wchar_t> const& use_facet<time_get<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<time_get<wchar_t> >(_CS_l);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_TIME_GET_CC__)
#  include <srccxx/time_get.cc>
#endif

#endif /* _CXXRT_TIME_GET_H__ */
