// -*-C++-*- cxxrt/num_get.h
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
// Title:  Declarations for the num_get facet
// Version: $Id: num_get.h,v 1.1.1.1 2002/06/05 01:02:06 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_NUM_GET_H__)
#define _CXXRT_NUM_GET_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif
#if !defined(_CXXRT_SBIT_FWD_H__)
#  include <cxxrt/sbit_fwd.h>
#endif
#if !defined(_CXXRT_BSTR_FWD)
#  include <cxxrt/bstr_fwd.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT,
  class _CS_InIt _CXXRT_TEMPLATE_DEF_ARG(istreambuf_iterator<_CS_cT>) >
class num_get: public locale::facet
{
public:
  typedef _CS_cT char_type;
  typedef _CS_InIt  iter_type;

  _CXXRT_EXPLICIT num_get(size_t refs = 0);

  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, bool&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, long&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, unsigned short&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, unsigned int&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, unsigned long&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, float&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, double&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, long double&) const;
  iter_type get(iter_type, iter_type, ios_base&, ios_base::iostate&, void*&) const;

  static locale::id id;

protected:
  typedef ios_base::iostate iostate; // just to fit the members on one line
  ~num_get();

  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, bool&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, long&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, unsigned short&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, unsigned int&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, unsigned long&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, float&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, double&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, long double&) const;
  virtual iter_type do_get(iter_type, iter_type, ios_base&, iostate&, void*&) const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<num_get<char> >(locale const&) 
{
  return true;
}

template <>
inline num_get<char> const& use_facet<num_get<char> >(locale const& _CS_l)
{
  return _CS_use_facet<num_get<char> >(_CS_l);
}

template <>
inline bool has_facet<num_get<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline num_get<wchar_t> const& use_facet<num_get<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<num_get<wchar_t> >(_CS_l);
}

#if 0
template <>
inline bool has_facet<num_get<char, char const*> >(locale const&) 
{
  return true;
}

template <>
inline num_get<char, char const*> const& use_facet<num_get<char, char const*> >(locale const& _CS_l)
{
  return _CS_use_facet<num_get<char, char const*> >(_CS_l);
}
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_NUM_GET_CC__)
#  include <srccxx/num_get.cc>
#endif

#endif /* _CXXRT_NUM_GET_H__ */
