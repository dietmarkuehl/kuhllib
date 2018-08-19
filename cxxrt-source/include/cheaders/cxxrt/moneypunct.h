// -*-C++-*- cxxrt/moneypunct.h
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
// Title:   Declarations for the moneypunct facet
// Version: $Id: moneypunct.h,v 1.1.1.1 2002/06/05 01:02:03 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_MONEYPUNCT_H__)
#define _CXXRT_MONEYPUNCT_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif 
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif 

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

class money_base
{
public:
  enum part
  {
    none,
    space,
    symbol,
    sign,
    value
  };
  struct pattern
  {
    char field[4];
  };
};

template <class _CS_cT, bool _CS_Intl>
class moneypunct: public locale::facet, public money_base
{
public:
  typedef _CS_cT               char_type;
  typedef basic_string<_CS_cT> string_type;

  static locale::id id;
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
  static bool const intl = _CS_Intl;
#else
  enum { intl = _CS_Intl };
#endif

  _CXXRT_EXPLICIT moneypunct(size_t = 0);

  char_type          decimal_point() const;
  char_type          thousands_sep() const;
  basic_string<char> grouping() const;
  string_type        curr_symbol() const;
  string_type        positive_sign() const;
  string_type        negative_sign() const;
  int                frac_digits() const;
  pattern            pos_format() const;
  pattern            neg_format() const;
  
protected:
  ~moneypunct();

  virtual char_type          do_decimal_point() const;
  virtual char_type          do_thousands_sep() const;
  virtual basic_string<char> do_grouping() const;
  virtual string_type        do_curr_symbol() const;
  virtual string_type        do_positive_sign() const;
  virtual string_type        do_negative_sign() const;
  virtual int                do_frac_digits() const;
  virtual pattern            do_pos_format() const;
  virtual pattern            do_neg_format() const;
};

// --------------------------------------------------------------------------

template <class _CS_cT, bool _CS_Intl>
class moneypunct_byname: public moneypunct<_CS_cT, _CS_Intl>
{
public:
  typedef money_base::pattern        pattern;
  typedef _CS_cT               char_type;
  typedef basic_string<_CS_cT> string_type;

  _CXXRT_EXPLICIT moneypunct_byname(char const*, size_t = 0);

protected:
  ~moneypunct_byname();

  virtual char_type           do_decimal_point() const;
  virtual char_type           do_thousands_sep() const;
  virtual basic_string<char>  do_grouping() const;
  virtual string_type         do_curr_symbol() const;
  virtual string_type         do_positive_sign() const;
  virtual string_type         do_negative_sign() const;
  virtual int                 do_frac_digits() const;
  virtual money_base::pattern do_pos_format() const;
  virtual money_base::pattern do_neg_format() const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<moneypunct<char, false> >(locale const&) 
{
  return true;
}

template <>
inline moneypunct<char, false> const& use_facet<moneypunct<char, false> >(locale const& _CS_l)
{
  return _CS_use_facet<moneypunct<char, false> >(_CS_l);
}

template <>
inline bool has_facet<moneypunct<char, true> >(locale const&) 
{
  return true;
}
template <>
inline moneypunct<char, true> const& use_facet<moneypunct<char, true> >(locale const& _CS_l)
{
  return _CS_use_facet<moneypunct<char, true> >(_CS_l);
}

template <>
inline bool has_facet<moneypunct<wchar_t, false> >(locale const&) 
{
  return true;
}

template <>
inline moneypunct<wchar_t, false> const& use_facet<moneypunct<wchar_t, false> >(locale const& _CS_l)
{
  return _CS_use_facet<moneypunct<wchar_t, false> >(_CS_l);
}

template <>
inline bool has_facet<moneypunct<wchar_t, true> >(locale const&) 
{
  return true;
}
template <>
inline moneypunct<wchar_t, true> const& use_facet<moneypunct<wchar_t, true> >(locale const& _CS_l)
{
  return _CS_use_facet<moneypunct<wchar_t, true> >(_CS_l);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_MONEYPUNCT_CC__)
#  include <srccxx/moneypunct.cc>
#endif

#endif /* _CXXRT_MONEYPUNCT_H__ */
