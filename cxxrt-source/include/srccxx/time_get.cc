// -*-C++-*- srccxx/time_get.cc
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
// Title:   Implementations for the srccxx/time_get category
// Version: $Id: time_get.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_TIME_GET_CC__)
#define __SRCCXX_TIME_GET_CC__ 1

#if !defined(_CXXRT_TIME_GET_H__)
#  include <cxxrt/time_get.h>
#endif
#if !defined(_CXXRT_SBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
locale::id time_get<_CS_cT, _CS_It>::id;

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
time_get<_CS_cT, _CS_It>::time_get(size_t _CS_refs):
  locale::facet(_CS_refs)
{
}

template <class _CS_cT, class _CS_It>
time_base::dateorder
time_get<_CS_cT, _CS_It>::date_order() const
{
  return do_date_order();
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::get_time(iter_type          _CS_b,
				   iter_type          _CS_e,
				   ios_base&          _CS_fmt,
				   ios_base::iostate& _CS_err,
				   tm*                _CS_tm) const
{
  return do_get_time(_CS_b, _CS_e, _CS_fmt, _CS_err, _CS_tm);
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::get_date(iter_type          _CS_b,
				   iter_type          _CS_e,
				   ios_base&          _CS_fmt,
				   ios_base::iostate& _CS_err,
				   tm*                _CS_tm) const
{
  return do_get_time(_CS_b, _CS_e, _CS_fmt, _CS_err, _CS_tm);
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::get_weekday(iter_type          _CS_b,
				      iter_type          _CS_e,
				      ios_base&          _CS_fmt,
				      ios_base::iostate& _CS_err,
				      tm*                _CS_tm) const
{
  return do_get_time(_CS_b, _CS_e, _CS_fmt, _CS_err, _CS_tm);
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::get_monthname(iter_type          _CS_b,
					iter_type          _CS_e,
					ios_base&          _CS_fmt,
					ios_base::iostate& _CS_err,
					tm*                _CS_tm) const
{
  return do_get_time(_CS_b, _CS_e, _CS_fmt, _CS_err, _CS_tm);
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::get_year(iter_type          _CS_b,
				   iter_type          _CS_e,
				   ios_base&          _CS_fmt,
				   ios_base::iostate& _CS_err,
				   tm*                _CS_tm) const
{
  return do_get_time(_CS_b, _CS_e, _CS_fmt, _CS_err, _CS_tm);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
time_base::dateorder
time_get<_CS_cT, _CS_It>::do_date_order() const
{
  return dateorder(); //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::do_get_time(iter_type _CS_b,
				      iter_type,
				      ios_base&,
				      ios_base::iostate&,
				      tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::do_get_date(iter_type _CS_b,
				      iter_type,
				      ios_base&,
				      ios_base::iostate&,
				      tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::do_get_weekday(iter_type _CS_b,
					 iter_type,
					 ios_base&,
					 ios_base::iostate&,
					 tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::do_get_monthname(iter_type _CS_b,
					   iter_type,
					   ios_base&,
					   ios_base::iostate&,
					   tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get<_CS_cT, _CS_It>::iter_type
time_get<_CS_cT, _CS_It>::do_get_year(iter_type _CS_b,
				      iter_type,
				      ios_base&,
				      ios_base::iostate&,
				      tm*) const
{
  return _CS_b; //-dk:TODO
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
time_get_byname<_CS_cT, _CS_It>::time_get_byname(char const*,
						 size_t _CS_refs):
  time_get<_CS_cT, _CS_It>(_CS_refs)
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
time_base::dateorder
time_get_byname<_CS_cT, _CS_It>::do_date_order() const
{
  return dateorder(); //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get_byname<_CS_cT, _CS_It>::iter_type
time_get_byname<_CS_cT, _CS_It>::do_get_time(iter_type _CS_b,
					     iter_type,
					     ios_base&,
					     ios_base::iostate&,
					     tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get_byname<_CS_cT, _CS_It>::iter_type
time_get_byname<_CS_cT, _CS_It>::do_get_date(iter_type _CS_b,
					     iter_type,
					     ios_base&,
					     ios_base::iostate&,
					     tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get_byname<_CS_cT, _CS_It>::iter_type
time_get_byname<_CS_cT, _CS_It>::do_get_weekday(iter_type _CS_b,
						iter_type,
						ios_base&,
						ios_base::iostate&,
						tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get_byname<_CS_cT, _CS_It>::iter_type
time_get_byname<_CS_cT, _CS_It>::do_get_monthname(iter_type _CS_b,
						  iter_type,
						  ios_base&,
						  ios_base::iostate&,
						  tm*) const
{
  return _CS_b; //-dk:TODO
}

template <class _CS_cT, class _CS_It>
typename time_get_byname<_CS_cT, _CS_It>::iter_type
time_get_byname<_CS_cT, _CS_It>::do_get_year(iter_type _CS_b,
					     iter_type,
					     ios_base&,
					     ios_base::iostate&,
					     tm*) const
{
  return _CS_b; //-dk:TODO
}


// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_TIME_GET_CC__ */
