// -*-C++-*- srccxx/moneypunct.cc
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
// Title:   Implementations for the srccxx/moneypunct category
// Version: $Id: moneypunct.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_MONEYPUNCT_CC__)
#define __SRCCXX_MONEYPUNCT_CC__ 1

#if !defined(_CXXRT_MONEYPUNCT_H__)
#  include <cxxrt/moneypunct.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif
#if !defined(_CXXRT_ALLOC_H__)
#  include <cxxrt/alloc.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, bool _CS_Intl>
locale::id moneypunct<_CS_cT, _CS_Intl>::id;
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
template <class _CS_cT, bool _CS_Intl>
bool const moneypunct<_CS_cT, _CS_Intl>::intl;
#endif

// --------------------------------------------------------------------------

template <class _CS_cT, bool _CS_Intl>
moneypunct<_CS_cT, _CS_Intl>::moneypunct(size_t _CS_refs):
  locale::facet(_CS_refs)
{
}

template <class _CS_cT, bool _CS_Intl>
moneypunct<_CS_cT, _CS_Intl>::~moneypunct()
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::char_type
moneypunct<_CS_cT, _CS_Intl>::decimal_point() const
{
  return do_decimal_point();
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::char_type         
moneypunct<_CS_cT, _CS_Intl>::thousands_sep() const
{
  return do_thousands_sep();
}

template <class _CS_cT, bool _CS_Intl>
basic_string<char>
moneypunct<_CS_cT, _CS_Intl>::grouping() const
{
  return do_grouping();
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::string_type
moneypunct<_CS_cT, _CS_Intl>::curr_symbol() const
{
  return do_curr_symbol();
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::string_type
moneypunct<_CS_cT, _CS_Intl>::positive_sign() const
{
  return do_positive_sign();
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::string_type
moneypunct<_CS_cT, _CS_Intl>::negative_sign() const
{
  return do_negative_sign();
}

template <class _CS_cT, bool _CS_Intl>
int       
moneypunct<_CS_cT, _CS_Intl>::frac_digits() const
{
  return do_frac_digits();
}

template <class _CS_cT, bool _CS_Intl>
money_base::pattern  
moneypunct<_CS_cT, _CS_Intl>::pos_format() const
{
  return do_pos_format();
}

template <class _CS_cT, bool _CS_Intl>
money_base::pattern 
moneypunct<_CS_cT, _CS_Intl>::neg_format() const
{
  return do_pos_format();
}

// --------------------------------------------------------------------------

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::char_type   
moneypunct<_CS_cT, _CS_Intl>::do_decimal_point() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::char_type  
moneypunct<_CS_cT, _CS_Intl>::do_thousands_sep() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
basic_string<char>
moneypunct<_CS_cT, _CS_Intl>::do_grouping() const
{
  return basic_string<char>(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::string_type
moneypunct<_CS_cT, _CS_Intl>::do_curr_symbol() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::string_type
moneypunct<_CS_cT, _CS_Intl>::do_positive_sign() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct<_CS_cT, _CS_Intl>::string_type
moneypunct<_CS_cT, _CS_Intl>::do_negative_sign() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
int       
moneypunct<_CS_cT, _CS_Intl>::do_frac_digits() const
{
  return int(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
money_base::pattern  
moneypunct<_CS_cT, _CS_Intl>::do_pos_format() const
{
  return pattern(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
money_base::pattern 
moneypunct<_CS_cT, _CS_Intl>::do_neg_format() const
{
  return pattern(); //-dk:TODO
}

// --------------------------------------------------------------------------

template <class _CS_cT, bool _CS_Intl>
moneypunct_byname<_CS_cT, _CS_Intl>::moneypunct_byname(char const*, size_t _CS_refs):
  moneypunct<_CS_cT, _CS_Intl>(_CS_refs)
{
  //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
moneypunct_byname<_CS_cT, _CS_Intl>::~moneypunct_byname()
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, bool _CS_Intl>
typename moneypunct_byname<_CS_cT, _CS_Intl>::char_type   
moneypunct_byname<_CS_cT, _CS_Intl>::do_decimal_point() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct_byname<_CS_cT, _CS_Intl>::char_type  
moneypunct_byname<_CS_cT, _CS_Intl>::do_thousands_sep() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
basic_string<char>
moneypunct_byname<_CS_cT, _CS_Intl>::do_grouping() const
{
  return basic_string<char>(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct_byname<_CS_cT, _CS_Intl>::string_type
moneypunct_byname<_CS_cT, _CS_Intl>::do_curr_symbol() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct_byname<_CS_cT, _CS_Intl>::string_type
moneypunct_byname<_CS_cT, _CS_Intl>::do_positive_sign() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
typename moneypunct_byname<_CS_cT, _CS_Intl>::string_type
moneypunct_byname<_CS_cT, _CS_Intl>::do_negative_sign() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
int       
moneypunct_byname<_CS_cT, _CS_Intl>::do_frac_digits() const
{
  return int(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
money_base::pattern  
moneypunct_byname<_CS_cT, _CS_Intl>::do_pos_format() const
{
  return pattern(); //-dk:TODO
}

template <class _CS_cT, bool _CS_Intl>
money_base::pattern 
moneypunct_byname<_CS_cT, _CS_Intl>::do_neg_format() const
{
  return pattern(); //-dk:TODO
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_MONEYPUNCT_CC__ */
