// -*-C++-*- src/num_puti.cc
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
// Title:   Implementation of the common part of integer formatting 
// Version: $Id: num_puti.cpp,v 1.1.1.1 2002/06/05 01:00:24 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if 0
#include <limits>
#include <cassert>
#include <cxxrt/num_put.h>
#include <cxxrt/ios_base.h>
#include <cxxrt/string.h>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

  enum
  {
    _CS_exponent_index    = 14,
    _CS_positive_index    = 16,
    _CS_negative_index    = 17,
    _CS_binary_index      = 18,
    _CS_hexadecimal_index = 19,
    _CS_separator_index   = 20,
    _CS_punct_index       = 21
  };

_CXXRT_NAMESPACE_END

// -------------------------------------------------------------------------- 

namespace {
  template <class T>
  struct _CS_sign_handler;

  template<> struct _CS_sign_handler<short> {
    static bool const _CS_signed = true;
    static bool _CS_negative(short _CS__CS_val) { return _CS__CS_val < 0; }
  };
  bool const _CS_sign_handler<short>::_CS_signed;

  template<> struct _CS_sign_handler<int> {
    static bool const _CS_signed = true;
    static bool _CS_negative(int _CS__CS_val) { return _CS__CS_val < 0; }
  };
  bool const _CS_sign_handler<int>::_CS_signed;

  template<> struct _CS_sign_handler<long> {
    static bool const _CS_signed = true;
    static bool _CS_negative(long _CS__CS_val) { return _CS__CS_val < 0; }
  };
  bool const _CS_sign_handler<long>::_CS_signed;

  template<> struct _CS_sign_handler<unsigned short> {
    static bool const _CS_signed = false;
    static bool _CS_negative(unsigned short _CS__CS_val) { return false; }
  };
  bool const _CS_sign_handler<unsigned short>::_CS_signed;

  template<> struct _CS_sign_handler<unsigned int> {
    static bool const _CS_signed = false;
    static bool _CS_negative(unsigned int _CS__CS_val) { return false; }
  };
  bool const _CS_sign_handler<unsigned int>::_CS_signed;

  template<> struct _CS_sign_handler<unsigned long> {
    static bool const _CS_signed = false;
    static bool _CS_negative(unsigned long _CS__CS_val) { return false; }
  };
  bool const _CS_sign_handler<unsigned long>::_CS_signed;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

inline
int*
_CS_digits(unsigned long _CS_base, string const& _CS_grouping, unsigned long _CS_value, int* _CS_begin)
{
  if (_CS_value == 0)
    *--_CS_begin = 0;
  else
    {
      if (_CS_base <= 1)
        _CS_base = 10;

      if (_CS_grouping.size() == 0 || _CS_grouping[0] == numeric_limits<char>::max())
        {
          for (; _CS_value > 0; _CS_value /= _CS_base)
            *--_CS_begin = _CS_value % _CS_base;
        }
      else
        {
          string::size_type _CS_group_index    = 0;
          int _CS_next_separator = _CS_grouping[_CS_group_index++] + 1;
          for (; _CS_value > 0; _CS_value /= _CS_base)
            {
              if (_CS_next_separator-- == 1)
                {
                  *--_CS_begin = _CS_separator_index;
                  if (_CS_group_index < _CS_grouping.size())
                    _CS_next_separator = _CS_grouping[_CS_group_index++];
                  else
                    _CS_next_separator = _CS_grouping[_CS_group_index - 1];
                }
              *--_CS_begin = _CS_value % _CS_base;
            }
        }
    }

  return _CS_begin;
}

// -------------------------------------------------------------------------- 

inline
int*
_CS_sign(bool _CS_negative, ios_base::fmtflags _CS_fmt, int* _CS_begin)
{
  if (_CS_negative)
    *--_CS_begin = _CS_negative_index;
  else
    if ((_CS_fmt & ios_base::showpos) != 0)
      *--_CS_begin = _CS_positive_index;
  return _CS_begin;
}

// -------------------------------------------------------------------------- 

template <class T>
inline int*
_CS_prefix(unsigned long _CS_base, ios_base::fmtflags _CS_fmt, int* _CS_begin, T _CS_value, bool _CS_negative)
{
  if ((ios_base::showbase & _CS_fmt) != 0)
    switch (_CS_base)
    {
    case 2:
      *--_CS_begin = _CS_binary_index;
      *--_CS_begin = 0;
      break;
    case 8:
      if (_CS_value != 0)
        *--_CS_begin = 0;
      break;
    case 16:
      *--_CS_begin = _CS_hexadecimal_index;
      *--_CS_begin = 0;
      break;
    case 10:
      _CS_begin = _CS_sign(_CS_negative, _CS_fmt, _CS_begin);
      break;
    default:
      // maybe something like '(<base>)<_CS_value>'?
      break;
    }
  else
    if (_CS_base == 10)
      _CS_begin = _CS_sign(_CS_negative, _CS_fmt, _CS_begin);

  return _CS_begin;
}

// -------------------------------------------------------------------------- 


template <class T>
inline int*
_CS_format(ios_base::fmtflags _CS_fmt, string const& _CS_grouping,
                 int* _CS_begin, int*& _CS_fillpos,
                 T _CS_val)
{
  unsigned long _CS_base = (_CS_fmt & ios_base::basefield) + 1;
  if (_CS_base == 1)
    _CS_base = 10;
  bool _CS_negative = _CS_sign_handler<T>::_CS_signed && _CS_sign_handler<T>::_CS_negative(_CS_val) && _CS_base == 10;
  unsigned long _CS_p_val = static_cast<unsigned long>(_CS_negative? -_CS_val: _CS_val);

  switch (_CS_fmt & ios_base::adjustfield)
    {
    case ios_base::left:
      _CS_fillpos = _CS_begin;
      _CS_begin = _CS_digits(_CS_base, _CS_grouping, _CS_p_val, _CS_begin); 
      _CS_begin = _CS_prefix(_CS_base, _CS_fmt, _CS_begin, _CS_val, _CS_negative);
      break;
    case ios_base::internal:
      _CS_fillpos = _CS_begin;
      _CS_begin = _CS_digits(_CS_base, _CS_grouping, _CS_p_val, _CS_begin); 
      if (_CS_base != 8)
        _CS_fillpos = _CS_begin;
      _CS_begin = _CS_prefix(_CS_base, _CS_fmt, _CS_begin, _CS_val, _CS_negative);
      break;
    case ios_base::right:
    default:
      _CS_begin = _CS_digits(_CS_base, _CS_grouping, _CS_p_val, _CS_begin); 
      _CS_begin = _CS_prefix(_CS_base, _CS_fmt, _CS_begin, _CS_val, _CS_negative);
      _CS_fillpos = _CS_begin;
      break;
    }

  return _CS_begin;
}

// -------------------------------------------------------------------------- 

int*
_CS_do_num_put(ios_base::fmtflags _CS_fmt, string const& _CS_grouping, int* _CS_begin, int*& _CS_fillpos, long _CS_val)
{
  return _CS_format(_CS_fmt, _CS_grouping, _CS_begin, _CS_fillpos, _CS_val);
}

int*
_CS_do_num_put(ios_base::fmtflags _CS_fmt, string const& _CS_grouping, int* _CS_begin, int*& _CS_fillpos,
                     unsigned long _CS_val)
{
  return _CS_format(_CS_fmt, _CS_grouping, _CS_begin, _CS_fillpos, _CS_val);
}

int*
_CS_do_num_put(ios_base::fmtflags _CS_fmt, string const& _CS_grouping, int* _CS_begin, int*& _CS_fillpos,
                     void const* _CS_val)
{
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER
  _CS_fmt = (_CS_fmt & ~ios_base::basefield) | ios_base::hex;
#else
  _CS_fmt = static_cast<ios_base::fmtflags>(_CS_fmt & ~static_cast<unsigned long>(ios_base::basefield))
    | ios_base::hex;
#endif
  return _CS_format(_CS_fmt, _CS_grouping, _CS_begin, _CS_fillpos,
		    static_cast<char const*>(_CS_val) - static_cast<char const*>(0));
}

// -------------------------------------------------------------------------- 

template int * _CS_format<long>(ios_base::fmtflags, basic_string<char, char_traits<char>, allocator<char> > const &, int *, int *&, long);
template int * _CS_prefix<long>(unsigned long, ios_base::fmtflags, int *, long, bool);

template int * _CS_format<unsigned long>(ios_base::fmtflags, basic_string<char, char_traits<char>, allocator<char> > const &, int *, int *&, unsigned long);
template int * _CS_prefix<unsigned long>(unsigned long, ios_base::fmtflags, int *, unsigned long, bool);

template int * _CS_format<int>(ios_base::fmtflags, basic_string<char, char_traits<char>, allocator<char> > const &, int *, int *&, int);
template int * _CS_prefix<int>(unsigned long, ios_base::fmtflags, int *, int, bool);

// -------------------------------------------------------------------------- 


_CXXRT_NAMESPACE_END
#endif
