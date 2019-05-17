// -*-C++-*- cxxrt/num_put.h
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
// Title:   Declarations for the num_put Facet
// Version: $Id: num_put.h,v 1.1.1.1 2002/06/05 01:02:10 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_NUM_PUT_H__)
#define _CXXRT_NUM_PUT_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(_CXXRT_SBIT_FWD_H__)
#  include <cxxrt/sbit_fwd.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

class ios_base;

// --------------------------------------------------------------------------

template <class _CS_cT,
  class _CS_It _CXXRT_TEMPLATE_DEF_ARG(ostreambuf_iterator<_CS_cT>) >
class num_put: public locale::facet
{
public:
  typedef _CS_cT char_type;
  typedef _CS_It iter_type;

  _CXXRT_EXPLICIT num_put(size_t refs = 0);

  iter_type put(iter_type, ios_base&, char_type, bool) const;
#if 1
  iter_type put(iter_type, ios_base&, char_type, long) const;
#else
  iter_type put(iter_type _CS_oit, ios_base& _CS_fmt, char_type _CS_fill, long _CS_val) const
{
  return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}
#endif
  iter_type put(iter_type, ios_base&, char_type, unsigned long) const;
  iter_type put(iter_type, ios_base&, char_type, double) const;
  iter_type put(iter_type, ios_base&, char_type, long double) const;
  iter_type put(iter_type, ios_base&, char_type, void const*) const;

  static locale::id id;

protected:
  virtual ~num_put();

  virtual iter_type do_put(iter_type, ios_base&, char_type, bool) const;
  virtual iter_type do_put(iter_type, ios_base&, char_type, long) const;
  virtual iter_type do_put(iter_type, ios_base&, char_type, unsigned long) const;
  virtual iter_type do_put(iter_type, ios_base&, char_type, double) const;
  virtual iter_type do_put(iter_type, ios_base&, char_type, long double) const;
  virtual iter_type do_put(iter_type, ios_base&, char_type, void const*) const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<num_put<char> >(locale const&) 
{
  return true;
}

template <>
inline num_put<char> const& use_facet<num_put<char> >(locale const& _CS_l)
{
  return _CS_use_facet<num_put<char> >(_CS_l);
}

template <>
inline bool has_facet<num_put<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline num_put<wchar_t> const& use_facet<num_put<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<num_put<wchar_t> >(_CS_l);
}

#if 0
template <>
inline bool has_facet<num_put<char, char*> >(locale const&) 
{
  return true;
}

template <>
inline num_put<char, char*> const& use_facet<num_put<char, char*> >(locale const& _CS_l)
{
  return _CS_use_facet<num_put<char, char*> >(_CS_l);
}
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_NUM_PUT_CC__)
#  include <srccxx/num_put.cc>
#endif

#endif /* _CXXRT_NUM_PUT_H__ */
