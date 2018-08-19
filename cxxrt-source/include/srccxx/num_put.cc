// -*-C++-*- srccxx/num_put.cc
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
// Title:   Implementation of num_put's Virtual Functions and Static Member
// Version: $Id: num_put.cc,v 1.1.1.1 2002/06/05 01:02:21 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_NUM_PUT_CC__)
#define __SRCCXX_NUM_PUT_CC__ 1

#if !defined(_CXXRT_NUM_PUT_H__)
#  include <cxxrt/num_put.h>
#endif
#if !defined(_CXXRT_CTYPE_H__)
#  include <cxxrt/ctype.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(_CXXRT_NUMPUNCT_H__)
#  include <cxxrt/numpunct.h>
#endif
#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif
#if !defined(_CXXRT_SBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif
#if !defined(_CXXRT_LCACHE_H__)
#  include <cxxrt/lcache.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif
#if !defined(_CXXRT_LIMITS_H__)
#  include <cxxrt/limits.h>
#endif
#include <iostream>

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------
// internally used function to do the actual formatting:

extern string __do_num_put(ios_base&, float);
extern string __do_num_put(ios_base&, double);
extern string __do_num_put(ios_base&, long double);

// --------------------------------------------------------------------------

template <class _CS_It, class _CS_cT>
inline _CS_It
_CS_fill_n(_CS_It _CS_oit, int _CS_n, _CS_cT _CS_fill)
{
  for (; _CS_n-- > 0; ++_CS_oit)
    *_CS_oit = _CS_fill;
  return _CS_oit;
}

template <class _CS_cT, class _CS_traits>
inline ostreambuf_iterator<_CS_cT, _CS_traits>
_CS_fill_n(ostreambuf_iterator<_CS_cT, _CS_traits> _CS_oit, int _CS_n, _CS_cT _CS_fill)
{
  basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf = _CS_oit._CS_sbuf();
  while (_CS_n > 0)
    {
      int _CS_t = _CS_sbuf->_CS_epptr() - _CS_sbuf->_CS_pptr();
      if (_CS_t > 0)
        {
          if (_CS_t > _CS_n)
            _CS_t = _CS_n;
          _CS_traits::assign(_CS_sbuf->_CS_pptr(), _CS_t, _CS_fill);
          _CS_sbuf->_CS_pbump(_CS_t);
          _CS_n -= _CS_t;
        }
      else
        {
          _CS_sbuf->sputc(_CS_fill);
          --_CS_n;
        }
    }
  return _CS_oit;
}

template <class _CS_InIt, class _CS_It>
inline _CS_It
_CS_copy(_CS_InIt _CS_beg, _CS_InIt _CS_end, _CS_It _CS_to)
{
  for (; _CS_beg != _CS_end; ++_CS_beg, ++_CS_to)
    *_CS_to = *_CS_beg;
  return _CS_to;
}

template <class _CS_cT, class _CS_traits>
inline ostreambuf_iterator<_CS_cT, _CS_traits>
_CS_copy(_CS_cT* _CS_beg, _CS_cT* _CS_end,
         ostreambuf_iterator<_CS_cT, _CS_traits> _CS_oit)
{
  basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf = _CS_oit._CS_sbuf();
  while (_CS_beg != _CS_end)
    {
      streamsize _CS_n = _CS_sbuf->_CS_epptr() - _CS_sbuf->_CS_pptr();
      if (_CS_n != 0)
        {
          if (_CS_end - _CS_beg < _CS_n)
            _CS_n = _CS_end - _CS_beg;
          _CS_traits::copy(_CS_sbuf->_CS_pptr(), _CS_beg, _CS_n);
          _CS_sbuf->_CS_pbump(_CS_n);
          _CS_beg += _CS_n;
        }
      else
        {
          _CS_sbuf->sputc(*_CS_beg);
          ++_CS_beg;
        }
    }

  return _CS_oit;
}

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
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

template <class _CS_cT>
inline _CS_cT*
_CS_digits(unsigned long _CS_base, string const& _CS_grouping, unsigned long _CS_value, _CS_cT* _CS_begin, _CS_cT const* _CS_cvt)
{
  if (_CS_value == 0)
    *--_CS_begin = _CS_cvt[0];
  else
    {
      if (_CS_grouping.size() == 0 || _CS_grouping[0] == numeric_limits<char>::max())
        {
          switch (_CS_base)
            {
            case 8:
              for (; _CS_value > 0; _CS_value /= 8)
                *--_CS_begin = _CS_cvt[_CS_value % 8];
              break;
            case 10:
              for (; _CS_value > 0; _CS_value /= 10)
                *--_CS_begin = _CS_cvt[_CS_value % 10];
              break;
            case 16:
              for (; _CS_value > 0; _CS_value /= 16)
                *--_CS_begin = _CS_cvt[_CS_value % 16];
              break;
            default:
              for (; _CS_value > 0; _CS_value /= _CS_base)
                *--_CS_begin = _CS_cvt[_CS_value % _CS_base];
              break;
            }
        }
      else
        {
          string::size_type _CS_group_index    = 0;
          int _CS_next_separator = _CS_grouping[_CS_group_index++] + 1;
          for (; _CS_value > 0; _CS_value /= _CS_base)
            {
              if (_CS_next_separator-- == 1)
                {
                  *--_CS_begin = _CS_cvt[_CS_separator_index];
                  if (_CS_group_index < _CS_grouping.size())
                    _CS_next_separator = _CS_grouping[_CS_group_index++];
                  else
                    _CS_next_separator = _CS_grouping[_CS_group_index - 1];
                }
              *--_CS_begin = _CS_cvt[_CS_value % _CS_base];
            }
        }
    }

  return _CS_begin;
}

// -------------------------------------------------------------------------- 


template <class _CS_cT>
inline _CS_cT*
_CS_sign(bool _CS_negative, ios_base::fmtflags _CS_fmt, _CS_cT* _CS_begin, _CS_cT const* _CS_cvt)
{
  if (_CS_negative)
    *--_CS_begin = _CS_cvt[_CS_negative_index];
  else
    if ((_CS_fmt & ios_base::showpos) != 0)
      *--_CS_begin = _CS_cvt[_CS_positive_index];
  return _CS_begin;
}

// -------------------------------------------------------------------------- 

template <class _CS_T, class _CS_cT>
inline _CS_cT*
_CS_prefix(unsigned long _CS_base, ios_base::fmtflags _CS_fmt, _CS_cT* _CS_begin, _CS_T _CS_value, bool _CS_negative, _CS_cT const* _CS_cvt)
{
  if ((ios_base::showbase & _CS_fmt) != 0)
    switch (_CS_base)
    {
    case 2:
      *--_CS_begin = _CS_cvt[_CS_binary_index];
      *--_CS_begin = _CS_cvt[0];
      break;
    case 8:
      if (_CS_value != 0)
        *--_CS_begin = _CS_cvt[0];
      break;
    case 16:
      *--_CS_begin = _CS_cvt[_CS_hexadecimal_index];
      *--_CS_begin = _CS_cvt[0];
      break;
    case 10:
      _CS_begin = _CS_sign(_CS_negative, _CS_fmt, _CS_begin, _CS_cvt);
      break;
    default:
      // maybe something like '(<base>)<_CS_value>'?
      break;
    }
  else
    if (_CS_base == 10)
      _CS_begin = _CS_sign(_CS_negative, _CS_fmt, _CS_begin, _CS_cvt);

  return _CS_begin;
}

// -------------------------------------------------------------------------- 


template <class _CS_T, class _CS_cT>
inline _CS_cT*
_CS_format(ios_base::fmtflags _CS_fmt, string const& _CS_grouping,
                 _CS_cT* _CS_begin, _CS_cT*& _CS_fillpos,
                 _CS_T _CS_val, _CS_cT const* _CS_cvt)
{
  unsigned long _CS_base = (_CS_fmt & ios_base::basefield) + 1;
  if (_CS_base == 1)
    _CS_base = 10;
  bool _CS_negative = _CS_sign_handler<_CS_T>::_CS_signed && _CS_sign_handler<_CS_T>::_CS_negative(_CS_val) && _CS_base == 10;
  unsigned long _CS_p_val = static_cast<unsigned long>(_CS_negative? -_CS_val: _CS_val);

  switch (_CS_fmt & ios_base::adjustfield)
    {
    case ios_base::left:
      _CS_fillpos = _CS_begin;
      _CS_begin = _CS_digits(_CS_base, _CS_grouping, _CS_p_val, _CS_begin, _CS_cvt); 
      _CS_begin = _CS_prefix(_CS_base, _CS_fmt, _CS_begin, _CS_val, _CS_negative, _CS_cvt);
      break;
    case ios_base::internal:
      _CS_fillpos = _CS_begin;
      _CS_begin = _CS_digits(_CS_base, _CS_grouping, _CS_p_val, _CS_begin, _CS_cvt); 
      if (_CS_base != 8)
        _CS_fillpos = _CS_begin;
      _CS_begin = _CS_prefix(_CS_base, _CS_fmt, _CS_begin, _CS_val, _CS_negative, _CS_cvt);
      break;
    case ios_base::right:
    default:
      _CS_begin = _CS_digits(_CS_base, _CS_grouping, _CS_p_val, _CS_begin, _CS_cvt); 
      _CS_begin = _CS_prefix(_CS_base, _CS_fmt, _CS_begin, _CS_val, _CS_negative, _CS_cvt);
      _CS_fillpos = _CS_begin;
      break;
    }

  return _CS_begin;
}

// -------------------------------------------------------------------------- 

template <class _CS_cT>
inline _CS_cT*
_CS_do_num_put(ios_base::fmtflags _CS_fmt, string const& _CS_grouping, _CS_cT* _CS_begin, _CS_cT*& _CS_fillpos, long _CS_val,
               _CS_cT const* _CS_cvt)
{
  return _CS_format(_CS_fmt, _CS_grouping, _CS_begin, _CS_fillpos, _CS_val, _CS_cvt);
}

template <class _CS_cT>
inline _CS_cT*
_CS_do_num_put(ios_base::fmtflags _CS_fmt, string const& _CS_grouping, _CS_cT* _CS_begin, _CS_cT*& _CS_fillpos,
               unsigned long _CS_val, _CS_cT const* _CS_cvt)
{
  return _CS_format(_CS_fmt, _CS_grouping, _CS_begin, _CS_fillpos, _CS_val, _CS_cvt);
}

template <class _CS_cT>
inline _CS_cT*
_CS_do_num_put(ios_base::fmtflags _CS_fmt, string const& _CS_grouping, _CS_cT* _CS_begin, _CS_cT*& _CS_fillpos,
               void const* _CS_val, _CS_cT const* _CS_cvt)
{
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER
  _CS_fmt = (_CS_fmt & ~ios_base::basefield) | ios_base::hex;
#else
  _CS_fmt = static_cast<ios_base::fmtflags>(_CS_fmt & ~static_cast<unsigned long>(ios_base::basefield))
    | ios_base::hex;
#endif
  return _CS_format(_CS_fmt, _CS_grouping, _CS_begin, _CS_fillpos,
                    static_cast<char const*>(_CS_val) - static_cast<char const*>(0), _CS_cvt);
}

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
num_put<_CS_cT, _CS_It>::~num_put()
{
}

// --------------------------------------------------------------------------
// Implementation of the forwarding functions: These are extremely simple
// and only intended to delegate the request to the correct function.

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                             bool _CS_val) const
{
  return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                             long _CS_val) const
{
  return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                             unsigned long _CS_val) const
{
  return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                             double _CS_val) const
{
  return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                             long double _CS_val) const
{
  return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                             void const* _CS_val) const
{
  return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

/* -----------------------------------------------------------------------*/

template <class _CS_cT, class _CS_It>
locale::id num_put<_CS_cT, _CS_It>::id;

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
num_put<_CS_cT, _CS_It>::num_put(size_t refs):
  locale::facet(refs)
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
_CS_It
_CS_num_put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill, string _CS_str)
{
  ctype<_CS_cT>    const &ct = use_facet<ctype<_CS_cT> >(_CS_fmt.getloc());
  numpunct<_CS_cT> const &np = use_facet<numpunct<_CS_cT> >(_CS_fmt.getloc());

  for (string::iterator it = _CS_str.begin(); it != _CS_str.end(); ++it)
    switch (*it)
      {
      case ' ':
        _CS_oit = _CS_fill_n(_CS_oit, _CS_fmt.width() - (_CS_str.size() - 1), _CS_fill);
        break;
      case '.':
        *_CS_oit = np.decimal_point();
        ++_CS_oit;
        break;
      case ',':
        *_CS_oit = np.thousands_sep();
        ++_CS_oit;
        break;
      default:
        *_CS_oit = ct.widen(*it);
        ++_CS_oit;
        break;
      }

  _CS_fmt.width(0);
  return _CS_oit;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::do_put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill, bool _CS_val) const
{
  if (!(_CS_fmt.flags() & ios_base::boolalpha))
    return do_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val? 1l: 0l);

  typedef numpunct<_CS_cT> NPunct;
#if defined(__STL_USE_STD_ALLOCATORS)
  basic_string<_CS_cT, char_traits<_CS_cT>, allocator<_CS_cT> > _CS_str = _CS_val
#else
  basic_string<_CS_cT, char_traits<_CS_cT>, alloc> _CS_str = _CS_val
#endif
    ? use_facet<NPunct>(_CS_fmt.getloc()).truename()
    : use_facet<NPunct>(_CS_fmt.getloc()).falsename();

  _CS_It rc = _CS_oit;
  switch (_CS_fmt.flags() & ios_base::adjustfield)
    {
    case ios_base::left:
      _CS_oit = _CS_copy(_CS_str.begin(), _CS_str.end(), _CS_oit);
      rc =  _CS_fill_n(_CS_oit, _CS_fmt.width() - _CS_str.size(), _CS_fill);
      break;
    case ios_base::right:
    case ios_base::internal:
    default:
      _CS_oit = _CS_fill_n(_CS_oit, _CS_fmt.width() - _CS_str.size(), _CS_fill);
      rc =  _CS_copy(_CS_str.begin(), _CS_str.end(), _CS_oit);
      break;
    }

  _CS_fmt.width(0);
  return rc;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
inline _CS_It
_CS_num_copy(_CS_cT* _CS_beg, _CS_cT* _CS_end, _CS_It& _CS_oit)
{
  for (; _CS_beg != _CS_end; ++_CS_beg, ++_CS_oit)
    *_CS_oit = *_CS_beg;
  return _CS_oit;
}

template <class _CS_cT, class _CS_traits>
inline ostreambuf_iterator<_CS_cT, _CS_traits>
_CS_num_copy(_CS_cT* _CS_beg, _CS_cT* _CS_end,
             ostreambuf_iterator<_CS_cT, _CS_traits>& _CS_oit)
{
  basic_streambuf<_CS_cT, _CS_traits>* _CS_sb = _CS_oit._CS_sbuf();

  if (_CS_end - _CS_beg < _CS_sb->_CS_epptr() - _CS_sb->_CS_pptr())
    {
      _CS_cT* _CS_to = _CS_sb->_CS_pptr();
      _CS_sb->_CS_pbump(_CS_end - _CS_beg);
      _CS_traits::copy(_CS_to, _CS_beg, _CS_end - _CS_beg);
    }
  else
    for (; _CS_beg != _CS_end; ++_CS_beg, ++_CS_oit)
      *_CS_oit = *_CS_beg;
  return _CS_oit;
}

template <class _CS_cT, class _CS_It, class Val>
inline _CS_It
_CS_new_num_put(_CS_It &_CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill, Val _CS_val)
{
  typedef _CS_cache<_CS_cT> _CS_C;

  _CS_C const& _CS_c = use_facet<_CS_C>(_CS_fmt._CS_getloc());
  _CS_cT const* _CS_cvt = (_CS_fmt.flags() & ios_base::uppercase) != 0
    ? _CS_c._CS_upper_table()
    : _CS_c._CS_lower_table();

  _CS_cT  _CS_buf[64];
  _CS_cT* _CS_end = _CS_buf + 64;
  _CS_cT* _CS_fp = _CS_end;

  _CS_cT* _CS_beg = _CS_do_num_put(_CS_fmt.flags(), _CS_c._CS_grouping(), _CS_end, _CS_fp, _CS_val, _CS_cvt);

  if (_CS_fmt.width() != 0)
    {
      int _CS_f = _CS_fmt.width(0) - (_CS_end - _CS_beg);
      
      if (_CS_f > 0)
        {
          _CS_oit = _CS_num_copy(_CS_beg, _CS_fp, _CS_oit);
          _CS_oit = _CS_fill_n(_CS_oit, _CS_f, _CS_fill);
          _CS_beg = _CS_fp;
        }
    }

  return _CS_num_copy(_CS_beg, _CS_end, _CS_oit);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::do_put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                                long _CS_val) const
{
  return _CS_new_num_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::do_put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                                unsigned long _CS_val) const
{
  return _CS_new_num_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::do_put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                                double _CS_val) const
{
#if 0
  return _CS_new_num_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
#else
  return _CS_num_put(_CS_oit, _CS_fmt, _CS_fill, __do_num_put(_CS_fmt, _CS_val));
#endif
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::do_put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                                long double _CS_val) const
{
#if 0
  return _CS_new_num_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
#else
  return _CS_num_put(_CS_oit, _CS_fmt, _CS_fill, __do_num_put(_CS_fmt, _CS_val));
#endif
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
_CS_It
num_put<_CS_cT, _CS_It>::do_put(_CS_It _CS_oit, ios_base &_CS_fmt, _CS_cT _CS_fill,
                                void const* _CS_val) const
{
  return _CS_new_num_put(_CS_oit, _CS_fmt, _CS_fill, _CS_val);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_NUM_PUT_CC__ */
