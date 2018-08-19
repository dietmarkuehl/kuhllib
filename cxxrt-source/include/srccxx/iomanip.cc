// -*-C++-*- srccxx/iomanip.cc
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
// Title:   Definition of the manipulator functions
// Version: $Id: iomanip.cc,v 1.1.1.1 2002/06/05 01:02:34 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_IOMANIP_CC__)
#define __SRCCXX_IOMANIP_CC__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_IOMANIP_H__)
#  include <cxxrt/iomanip.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_charT>
_CS_setfill<_CS_charT> setfill(_CS_charT _CS_c)
{
  return _CS_setfill<_CS_charT>(_CS_c);
}

template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setfill<_CS_charT> const& _CS_f)
{
  _CS_strm.fill(_CS_f._CS_c);
  return _CS_strm;
}

template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setfill<_CS_charT> const& _CS_f)
{
  _CS_strm.fill(_CS_f._CS_c);
  return _CS_strm;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setprecision const& _CS_f)
{
  _CS_strm.precision(_CS_f._CS_precision);
  return _CS_strm;
}

template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setprecision const& _CS_f)
{
  _CS_strm.precision(_CS_f._CS_precision);
  return _CS_strm;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setw const& _CS_f)
{
  _CS_strm.width(_CS_f._CS_width);
  return _CS_strm;
}

template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setw const& _CS_f)
{
  _CS_strm.width(_CS_f._CS_width);
  return _CS_strm;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_resetiosflags const& _CS_f)
{
  _CS_strm.setf(ios_base::fmtflags(0), _CS_f._CS_flags);
  return _CS_strm;
}

template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_resetiosflags const& _CS_f)
{
  _CS_strm.setf(ios_base::fmtflags(0), _CS_f._CS_flags);
  return _CS_strm;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setiosflags const& _CS_f)
{
  _CS_strm.setf(_CS_f._CS_flags);
  return _CS_strm;
}

template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setiosflags const& _CS_f)
{
  _CS_strm.setf(_CS_f._CS_flags);
  return _CS_strm;
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setbase const& _CS_b)
{
  switch (_CS_b._CS_base)
    {
    case 8:
      oct(_CS_strm);
      break;
    case 10:
      dec(_CS_strm);
      break;
    case 16:
      hex(_CS_strm);
      break;
    default:
      _CS_strm.setf(ios_base::fmtflags(0), ios_base::basefield);
      break;
    }
  return _CS_strm;
}

template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>& _CS_strm,
            _CS_setbase const& _CS_b)
{
  switch (_CS_b._CS_base)
    {
    case 8:
      oct(_CS_strm);
      break;
    case 10:
      dec(_CS_strm);
      break;
    case 16:
      hex(_CS_strm);
      break;
    default:
      _CS_strm.setf(ios_base::fmtflags(0), ios_base::basefield);
      break;
    }
  return _CS_strm;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_IOMANIP_CC__ */
