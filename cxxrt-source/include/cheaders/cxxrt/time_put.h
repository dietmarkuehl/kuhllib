// -*-C++-*- cxxrt/time_put.h
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
// Title:  Declarations for the time_put facet
// Version: $Id: time_put.h,v 1.1.1.1 2002/06/05 01:01:55 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_TIME_PUT_H__)
#define _CXXRT_TIME_PUT_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif 
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif 
#if !defined(_CXXRT_SBIT_FWD_H__)
#  include <cxxrt/sbit_fwd.h>
#endif 
#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif 
#include <ctime>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT,
                class _CS_It _CXXRT_TEMPLATE_DEF_ARG(ostreambuf_iterator<_CS_cT>) >
class time_put: public locale::facet
{
public:
        typedef _CS_cT char_type;
        typedef _CS_It iter_type;

        static locale::id id;

        _CXXRT_EXPLICIT time_put(size_t = 0);

        iter_type put(iter_type, ios_base&, char_type, tm const*, char_type const*, char_type const*) const;
        iter_type put(iter_type, ios_base&, char_type, tm const*, char, char = 0) const;

protected:
        virtual iter_type do_put(iter_type, ios_base&, char_type, tm const*, char, char) const;
};

// --------------------------------------------------------------------------

template <class _CS_cT,
                class _CS_It _CXXRT_TEMPLATE_DEF_ARG(ostreambuf_iterator<_CS_cT>) >
class time_put_byname: public time_put<_CS_cT, _CS_It>
{
public:
        typedef _CS_cT char_type;
        typedef _CS_It iter_type;

        _CXXRT_EXPLICIT time_put_byname(char const *, size_t = 0);

protected:
        virtual iter_type do_put(iter_type, ios_base&, char_type, tm const*, char, char) const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<time_put<char> >(locale const&) 
{
  return true;
}

template <>
inline time_put<char> const& use_facet<time_put<char> >(locale const& _CS_l)
{
  return _CS_use_facet<time_put<char> >(_CS_l);
}

template <>
inline bool has_facet<time_put<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline time_put<wchar_t> const& use_facet<time_put<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<time_put<wchar_t> >(_CS_l);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_TIME_PUT_CC__)
#  include <srccxx/time_put.cc>
#endif

#endif /* _CXXRT_TIME_PUT_H__ */
