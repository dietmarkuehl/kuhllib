// -*-C++-*- srccxx/num_get.cc
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
// Title:   Implementation of num_get's Virtual Functions and Static Member
// Version: $Id: num_get.cc,v 1.4 2006/04/07 07:07:13 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_NUM_GET_CC__)
#define __SRCCXX_NUM_GET_CC__ 1

#if !defined(_CXXRT_NUM_GET_H__)
#  include <cxxrt/num_get.h>
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
#if !defined(_CXXRT_LCACHE_H__)
#  include <cxxrt/lcache.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif
#if !defined(_CXXRT_EXTRACTOR_H__)
#  include <cxxrt/extractor.h>
#endif
#include <limits>
#include <cstdlib>
#include <cmath>

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        bool& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
                               num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                               ios_base& _CS_fmt, ios_base::iostate& _CS_err, long& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        unsigned short& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        unsigned int& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        unsigned long& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        float& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        double& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        long double& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        void*& _CS_val) const
{
  return do_get(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

/* -----------------------------------------------------------------------*/

template <class _CS_cT, class _CS_InIt>
locale::id num_get<_CS_cT, _CS_InIt>::id;

template <class _CS_cT, class _CS_InIt>
num_get<_CS_cT, _CS_InIt>::num_get(size_t _CS_refs):
  locale::facet(_CS_refs)
{
}

template <class _CS_cT, class _CS_InIt>
num_get<_CS_cT, _CS_InIt>::~num_get()
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
        num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        bool& _CS_val) const
{
  if ((_CS_fmt.flags() & ios_base::boolalpha) == 0)
    {
      // if boolalpha is not set, try numeric scanning:
      long _CS_lval;
      _CS_begin = get(_CS_begin, _CS_end, _CS_fmt, _CS_err, _CS_lval);
      if ((_CS_err & ios_base::failbit) == ios_base::goodbit)
        switch (_CS_lval)
          {
          case 0: _CS_val = false; break;
          case 1: _CS_val = true; break;
          default: _CS_err |= ios_base::failbit; break;
          }
    }
  else
    {
      // optimization potential:
      // - avoid virtual calls using the cache facet instead
      // - use the extractor class to get the individual characters
      // - reduce the comparisons by determine which string is longer (potentially with the extractor)
      typedef numpunct<_CS_cT> _CS_NP;
      _CS_NP const& _CS_np = use_facet<_CS_NP>(_CS_fmt.getloc());
      basic_string<_CS_cT> const& _CS_match = _CS_val? _CS_np.truename():
                                                                         _CS_np.falsename();
      _CXXRT_TYPENAME basic_string<_CS_cT>::const_iterator _CS_sit = _CS_match.begin();
      while (_CS_sit != _CS_match.end()
             && _CS_begin != _CS_end
             && *_CS_sit == *_CS_begin)
        {
          ++_CS_sit;
          ++_CS_begin;
        }

      if (_CS_sit == _CS_match.end())
        _CS_err = ios_base::goodbit;
      else if (_CS_begin == _CS_end)
        _CS_err = ios_base::eofbit;
      else
        _CS_err = ios_base::failbit;
      
    }
  return _CS_begin;
}

// --------------------------------------------------------------------------
// preconditions:
// - _CS_val == 0
// - _CS_negative == false

template <class _CS_cT, class _CS_It>
inline bool
_CS_get_integer(_CS_extractor<_CS_cT, _CS_It>& _CS_ext,
                      ios_base& _CS_fmt, ios_base::iostate& _CS_err,
                      unsigned long& _CS_val, bool& _CS_negative)
{
  _CS_err = ios_base::goodbit;
  unsigned int _CS_cval = 0;

  if (_CS_ext._CS_get_current(_CS_cval))
    switch (_CS_cval)
      {
      case _CS_cache_base::_CS_minus:
        _CS_negative = true;
      case _CS_cache_base::_CS_plus:
        if (!_CS_ext._CS_get_next(_CS_cval))
          {
            _CS_err |= ios_base::failbit | ios_base::eofbit;
            return false;
          }
        break;
      default:
        break;
      }
  else
    {
      _CS_err |= ios_base::failbit | ios_base::eofbit;
      return false;
    }

  unsigned long _CS_base;
  switch (_CS_fmt.flags() & ios_base::basefield)
    {
    case ios_base::oct: _CS_base = 8; break;
    case ios_base::dec: _CS_base = 10; break;
    case ios_base::hex: _CS_base = 16; break;
    default: _CS_base = 17; break;
    }

  if (_CS_cval == 0)
    {
      if (!_CS_ext._CS_get_next(_CS_cval))
        {
          _CS_err |= ios_base::eofbit;
          return true;
        }

      if (_CS_base >= 16)
        {
          if (_CS_cval == _CS_cache_base::_CS_hex)
            {
              _CS_base = 16;
              if (!_CS_ext._CS_get_next(_CS_cval) || _CS_cval > 16)
                {
                  _CS_err |= ios_base::eofbit;
                  return true;
                }
            }
          else
            if (_CS_base == 17)
              _CS_base = 8;
        }
    }
  else
    {
      if (_CS_base == 17)
        _CS_base = 10;
      if (_CS_cval >= _CS_base)
        {
          _CS_err |= ios_base::failbit;
          return false;
        }
    }

  while (_CS_cval == 0)
    if (!_CS_ext._CS_get_next(_CS_cval))
      {
        _CS_err |= ios_base::eofbit;
        return true;
      }

  switch (_CS_base)
    {
    case 10: _CS_ext._CS_init_count(numeric_limits<unsigned long>::digits10 - 1); break;
    case 8:  _CS_ext._CS_init_count(numeric_limits<unsigned long>::digits / 3 - 1); break;
    case 16: _CS_ext._CS_init_count(numeric_limits<unsigned long>::digits / 4 - 1); break;
    default:
      break;
    }

  // the actual loop reading the integer's value:
  do
    if (_CS_cval < _CS_base)
      _CS_val = _CS_val * _CS_base + _CS_cval;
    else
      {
        //-dk: handle thousands separators
        return true;
      }
  while (_CS_ext._CS_get_next_counted(_CS_cval));

  // overflow checking:
  if (_CS_ext._CS_get_count() == 0
      && _CS_ext._CS_get_next(_CS_cval)
      && _CS_cval < _CS_base)
    {
      if (_CS_val <= numeric_limits<unsigned long>::max() / _CS_base
          && (numeric_limits<unsigned long>::max() - (_CS_val *= _CS_base)) >= _CS_cval)
        _CS_val += _CS_cval;
      else
        goto _CS_overflow;

      if (_CS_ext._CS_get_next(_CS_cval) && _CS_cval < _CS_base)
        goto _CS_overflow;
    }

  // OK, that's it: signal success but check for EOF first
  if (!_CS_ext._CS_get_current(_CS_cval))
    _CS_err |= ios_base::eofbit;
  return true;

  // question: is it really intended that overflow does not set the failbit?
_CS_overflow:
  _CS_val = numeric_limits<unsigned long>::max();
  while (_CS_ext._CS_get_next(_CS_cval) && _CS_cval < _CS_base)
    ;
  if (!_CS_ext._CS_get_current(_CS_cval))
    _CS_err |= ios_base::eofbit;
  return false;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_InIt, class _CS_Val>
inline _CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
_CS_get_signed_integer(_CXXRT_TYPENAME num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                       _CXXRT_TYPENAME num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                       ios_base& _CS_fmt, ios_base::iostate& _CS_err, _CS_Val& _CS_val)
{
  _CS_extractor<_CS_cT, _CS_InIt> _CS_ext(_CS_begin, _CS_end, _CS_fmt.getloc());
  bool _CS_neg = false;
  unsigned long _CS_tmpval = 0;

  if (_CS_get_integer(_CS_ext, _CS_fmt, _CS_err, _CS_tmpval, _CS_neg))
    {
      if (_CS_neg)
        {
          if (_CS_tmpval > -static_cast<unsigned long>(numeric_limits<_CS_Val>::min()))
            _CS_val = numeric_limits<_CS_Val>::min();
          else
            _CS_val = -static_cast<_CS_Val>(_CS_tmpval);
        }
      else
        {
          if (_CS_tmpval > static_cast<unsigned long>(numeric_limits<_CS_Val>::max()))
            _CS_val = numeric_limits<_CS_Val>::max();
          else
            _CS_val = static_cast<_CS_Val>(_CS_tmpval);
        }
    }
  else
    if (_CS_tmpval == numeric_limits<unsigned long>::max())
      _CS_val = _CS_neg? numeric_limits<_CS_Val>::min()
                                   : numeric_limits<_CS_Val>::max();

  return _CS_begin;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_InIt, class _CS_Val>
inline _CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
_CS_get_unsigned_integer(
        _CXXRT_TYPENAME num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
        _CXXRT_TYPENAME num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
        ios_base& _CS_fmt,
        ios_base::iostate& _CS_err,
        _CS_Val& _CS_val)
{
  _CS_extractor<_CS_cT, _CS_InIt> _CS_ext(_CS_begin, _CS_end,
                                                                  _CS_fmt.getloc());
  bool _CS_neg = false;
  unsigned long _CS_tmpval = 0;

  if (_CS_get_integer(_CS_ext, _CS_fmt, _CS_err, _CS_tmpval, _CS_neg))
    {
      if (_CS_tmpval > static_cast<unsigned long>(numeric_limits<_CS_Val>::max()))
        _CS_val = numeric_limits<_CS_Val>::max();
      else
        _CS_val = _CS_neg? -static_cast<_CS_Val>(_CS_tmpval)
                                     : static_cast<_CS_Val>(_CS_tmpval);
    }
  else
    if (_CS_tmpval == numeric_limits<unsigned long>::max())
      _CS_val =  numeric_limits<_CS_Val>::max();

  return _CS_begin;
}


// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err, long& _CS_val) const
{
  return _CS_get_signed_integer<_CS_cT, _CS_InIt, long>(_CS_begin, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err,
                                  unsigned short& _CS_val) const
{
  return _CS_get_unsigned_integer<_CS_cT, _CS_InIt>(_CS_begin, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err, unsigned int& _CS_val) const
{
  return _CS_get_unsigned_integer<_CS_cT, _CS_InIt>(_CS_begin, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err, unsigned long& _CS_val) const
{
  return _CS_get_unsigned_integer<_CS_cT, _CS_InIt>(_CS_begin, _CS_end, _CS_fmt, _CS_err, _CS_val);
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_in,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err, void*& _CS_val) const
{
  unsigned long _CS_tval = 0;
  typename num_get<_CS_cT, _CS_InIt>::iter_type _CS_rc = 
    _CS_get_unsigned_integer<_CS_cT, _CS_InIt>(_CS_in, _CS_end, _CS_fmt, _CS_err, _CS_tval);
  if ((_CS_err & ios_base::failbit) == 0)
    _CS_val = static_cast<char*>(0) + _CS_tval;
  return _CS_rc;
}

// --------------------------------------------------------------------------
// precondition:
// - _CS_val == 0.0

namespace
{
  enum { _CS_bufsize = numeric_limits<long double>::digits10 + 64 };
}

template <class _CS_cT, class _CS_It>
bool
_CS_get_float(_CS_extractor<_CS_cT, _CS_It>& _CS_ext, ios_base::iostate& _CS_err, long double& _CS_val)
{
  // Note: This code is temporary! It should really do the value generation on
  //       fly. However, this is non-trivial due to rounding and thus left
  //       undone until there is time to address this issue...

  // a precise calculation would be 'digits10 + log(max(abs(min_exponent), max_exponent)) + 4'
  // but a little to much does not really harm (at least on most systems...).
  static char const _CS_chars[] = "0123456789    e +-   . ";
  char         _CS_buffer[_CS_bufsize];
  unsigned int _CS_cval;
  char*        _CS_cur = _CS_buffer;
  bool         _CS_has_point = false;
  double       _CS_factor = 1.0;
  
  if (!_CS_ext._CS_get_current(_CS_cval))
    {
      _CS_err = ios_base::failbit | ios_base::eofbit;
      return false;
    }
  else
    _CS_err = ios_base::goodbit;

  switch (_CS_chars[_CS_cval])
    {
    case ' ':
    case 'e':
      _CS_err = ios_base::failbit;
      return false;
    case '.':
      _CS_has_point = true;
    default:
      *_CS_cur++ = _CS_chars[_CS_cval];
      break;
    }

  _CS_ext._CS_init_count(numeric_limits<long double>::digits10);

  while (_CS_ext._CS_get_next_counted(_CS_cval) && _CS_cval < 10)
    *_CS_cur++ = _CS_chars[_CS_cval];
  
  if (_CS_ext._CS_get_count() == 0)
    while (_CS_ext._CS_get_next(_CS_cval) && _CS_cval < 10)
      _CS_factor *= 10;

  if (!_CS_ext._CS_get_current(_CS_cval))
    {
      _CS_err = ios_base::eofbit;
      goto _CS_done;
    }

  // if the next character is a '.' read the fractional part (if there wasn't one yet)
  if (_CS_chars[_CS_cval] == '.')
    {
      if (_CS_has_point)
        goto _CS_done;
      *_CS_cur++ = '.';

      while (_CS_ext._CS_get_next_counted(_CS_cval) && _CS_cval < 10)
        *_CS_cur++ = _CS_chars[_CS_cval];
      
      if (_CS_ext._CS_get_count() == 0)
        while (_CS_ext._CS_get_next(_CS_cval) && _CS_cval < 10)
          ;
      
      if (!_CS_ext._CS_get_current(_CS_cval))
        {
          _CS_err = ios_base::eofbit;
          goto _CS_done;
        }
    }

  // if the next character is an 'e' read the exponent:
  if (_CS_chars[_CS_cval] == 'e')
    {
      *_CS_cur++ = 'e';
      if (!_CS_ext._CS_get_next(_CS_cval))
        {
          _CS_err = ios_base::eofbit;
          goto _CS_done;
        }
      switch (_CS_chars[_CS_cval])
        {
        case ' ':
        case '.':
        case 'e':
          goto _CS_done;
        default:
          *_CS_cur++ = _CS_chars[_CS_cval];
          break;
        }

      _CS_ext._CS_init_count(60);
      while (_CS_ext._CS_get_next_counted(_CS_cval) && _CS_cval < 10)
        *_CS_cur++ = _CS_chars[_CS_cval];

      if (_CS_ext._CS_get_count() == 0)
        while (_CS_ext._CS_get_next(_CS_cval) && _CS_cval < 10)
          ;
      
      if (!_CS_ext._CS_get_current(_CS_cval))
        _CS_err = ios_base::eofbit;
    }
  
_CS_done:
  *_CS_cur = 0;
  _CS_val = strtod(_CS_buffer, 0);
  _CS_val *= _CS_factor;
 
  return true;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err, float& _CS_val) const
{
  _CS_extractor<_CS_cT, _CS_InIt> _CS_ext(_CS_begin, _CS_end, _CS_fmt.getloc());

  long double _CS_tval = 0.0; 
  if (_CS_get_float(_CS_ext, _CS_err, _CS_tval))
    if (fabs(_CS_tval) < numeric_limits<float>::min())
      _CS_val = 0.0;
    else if (fabs(_CS_tval) > numeric_limits<float>::max())
      _CS_val = _CS_tval < 0.0? -numeric_limits<float>::max(): numeric_limits<float>::max();
    else
      _CS_val = static_cast<float>(_CS_tval);
  return _CS_begin;
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err, double& _CS_val) const
{
  _CS_extractor<_CS_cT, _CS_InIt> _CS_ext(_CS_begin, _CS_end, _CS_fmt.getloc());
  long double _CS_tval = 0.0; 
  if (_CS_get_float(_CS_ext, _CS_err, _CS_tval))
    if (fabs(_CS_tval) < numeric_limits<double>::min())
      _CS_val = 0.0;
    else if (fabs(_CS_tval) > numeric_limits<double>::max())
      _CS_val = _CS_tval < 0.0? -numeric_limits<double>::max(): numeric_limits<double>::max();
    else
      _CS_val = static_cast<double>(_CS_tval);
  return _CS_begin;
}

template <class _CS_cT, class _CS_InIt>
_CXXRT_TYPENAME num_get<_CS_cT, _CS_InIt>::iter_type
num_get<_CS_cT, _CS_InIt>::do_get(num_get<_CS_cT,_CS_InIt>::iter_type _CS_begin,
                                  num_get<_CS_cT,_CS_InIt>::iter_type _CS_end,
                                  ios_base& _CS_fmt, ios_base::iostate& _CS_err, long double& _CS_val) const
{
  _CS_extractor<_CS_cT, _CS_InIt> _CS_ext(_CS_begin, _CS_end, _CS_fmt.getloc());
  long double _CS_tval = 0.0; 
  if (_CS_get_float(_CS_ext, _CS_err, _CS_tval))
    _CS_val = _CS_tval;
  return _CS_begin;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_NUM_GET_CC__ */
