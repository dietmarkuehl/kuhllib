// -*-C++-*- cxxrt/iomanip.h
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
// Title:   Declaration of IOStream manipulators 
// Version: $Id: iomanip.h,v 1.1.1.1 2002/06/05 01:01:53 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_IOMANIP_H__)
#define _CXXRT_IOMANIP_H__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif
#if !defined(_CXXRT_IOSFWD_H__)
#  include <cxxrt/iosfwd.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

struct _CS_resetiosflags
{
  _CS_resetiosflags(ios_base::fmtflags _CS_f): _CS_flags(_CS_f) {}
  ios_base::fmtflags _CS_flags;
};
extern _CS_resetiosflags resetiosflags(ios_base::fmtflags);
template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>&, _CS_resetiosflags const&);
template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>&, _CS_resetiosflags const&);

struct _CS_setiosflags
{
  _CS_setiosflags(ios_base::fmtflags _CS_f): _CS_flags(_CS_f) {}
  ios_base::fmtflags _CS_flags;
};
extern _CS_setiosflags setiosflags(ios_base::fmtflags);
template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>&, _CS_setiosflags const&);
template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>&, _CS_setiosflags const&);

struct _CS_setbase
{
  _CS_setbase(int _CS_b): _CS_base(_CS_b) {}
  int _CS_base;
};
extern _CS_setbase setbase(int);
template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>&, _CS_setbase const&);
template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>&, _CS_setbase const&);

template <class _CS_charT>
struct _CS_setfill
{
  _CS_setfill(_CS_charT _CS_f): _CS_c(_CS_f) {}
  _CS_charT _CS_c;
};
template <class _CS_charT>
_CS_setfill<_CS_charT> setfill(_CS_charT);
template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>&, _CS_setfill<_CS_charT> const&);
template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>&, _CS_setfill<_CS_charT> const&);

struct _CS_setprecision
{
  _CS_setprecision(int _CS_p): _CS_precision(_CS_p) {}
  int _CS_precision;
};
extern _CS_setprecision setprecision(int);
template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>&, _CS_setprecision const&);
template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>&, _CS_setprecision const&);

struct _CS_setw
{
  _CS_setw(int _CS_w): _CS_width(_CS_w) {}
  int _CS_width;
};
extern _CS_setw setw(int);
template <class _CS_charT, class _CS_traits>
basic_istream<_CS_charT, _CS_traits>&
operator>> (basic_istream<_CS_charT, _CS_traits>& _CS_strm, _CS_setw const& _CS_w);

template <class _CS_charT, class _CS_traits>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_strm, _CS_setw const& _CS_w);

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_IOMANIP_H__ */
