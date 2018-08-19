// -*-C++-*- cxxrt/collate.h
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
// Title:   Declarations for the collate facet
// Version: $Id: collate.h,v 1.1.1.1 2002/06/05 01:01:43 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_COLLATE_H__)
#define _CXXRT_COLLATE_H__ 1

#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif 
#if !defined(_CXXRT_BSTR_FWD_H__)
#  include <cxxrt/bstr_fwd.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT>
class collate: public locale::facet
{
public:
  static locale::id id;

  typedef _CS_cT               char_type;
  typedef basic_string<_CS_cT> string_type;

  _CXXRT_EXPLICIT collate(size_t = 0);
  
  int         compare(_CS_cT const*, _CS_cT const*,
                      _CS_cT const*, _CS_cT const*) const;
  string_type transform(_CS_cT const*, _CS_cT const*) const;
  long        hash(_CS_cT const*, _CS_cT const*) const;

protected:
  virtual ~collate();

  virtual int         do_compare(_CS_cT const*, _CS_cT const*,
                                 _CS_cT const*, _CS_cT const*) const;
  virtual string_type do_transform(_CS_cT const*, _CS_cT const*) const;
  virtual long        do_hash(_CS_cT const*, _CS_cT const*) const;
};

// --------------------------------------------------------------------------

template <class _CS_cT> class collate_byname: public collate<_CS_cT>
{
public:
  typedef _CXXRT_TYPENAME collate<_CS_cT>::string_type string_type;

  collate_byname(char const*, size_t = 0);

protected:
  virtual int         do_compare(_CS_cT const*, _CS_cT const*,
                                 _CS_cT const*, _CS_cT const*) const;
  virtual string_type do_transform(_CS_cT const*, _CS_cT const*) const;
  virtual long        do_hash(_CS_cT const*, _CS_cT const*) const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<collate<char> >(locale const&) 
{
  return true;
}

template <>
inline collate<char> const& use_facet<collate<char> >(locale const& _CS_l)
{
  return _CS_use_facet<collate<char> >(_CS_l);
}

template <>
inline bool has_facet<collate<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline collate<wchar_t> const& use_facet<collate<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<collate<wchar_t> >(_CS_l);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_Tr, class _CS_Al>
inline
bool locale::operator() (basic_string<_CS_cT, _CS_Tr, _CS_Al> const &_CS_s1,
                         basic_string<_CS_cT, _CS_Tr, _CS_Al> const &_CS_s2) const
{
  typedef _CXXRT_STD_NAME ::collate<_CS_cT> _CS_collate;
  _CS_collate const &_CS_coll = use_facet<_CS_collate>(*this);
  return _CS_coll.compare(_CS_s1.data(), _CS_s1.data() + _CS_s1.size(),
                          _CS_s2.data(), _CS_s2.data() + _CS_s2.size());
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_COLLATE_CC__)
#  include <srccxx/collate.cc>
#endif

#endif /* _CXXRT_COLLATE_H__ */
