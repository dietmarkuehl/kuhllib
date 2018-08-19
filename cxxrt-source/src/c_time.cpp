// -*-C++-*- src/c_time.cc
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
// Title:   Definition of the low-level interface to the time facet
// Version: $Id: c_time.cpp,v 1.1.1.1 2002/06/05 01:00:13 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include "cxxrt/c_locale.h"

// -------------------------------------------------------------------------- 

struct _Locale_time     * _Locale_time_create(const char *)
{
	return 0; //-dk:TODO
}

char * _Locale_time_name(const struct _Locale_time *, char *)
{
	return 0; //-dk:TODO
}

void _Locale_time_destroy(struct _Locale_time *)
{
	//-dk:TODO
}

int _Locale_dateorder(struct _Locale_time *)
{
	return 0; //-dk:TODO
}

_CXXRT_STD size_t _Locale_time_format(char *, _CXXRT_STD size_t, const struct tm *,
                           char, char, char)
{
	return 0; //-dk:TOD
}

const char ** _Locale_full_monthname(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char ** _Locale_abbrev_monthname(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char ** _Locale_full_dayofweek(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char ** _Locale_abbrev_dayofweek(struct _Locale_time *)
{
  return 0; //-dk:TODO
}


const char * _Locale_d_t_fmt(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char * _Locale_d_fmt(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char * _Locale_t_fmt(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char * _Locale_am_str(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char * _Locale_pm_str(struct _Locale_time *)
{
  return 0; //-dk:TODO
}

const char * _Locale_t_fmt_ampm(struct _Locale_time *)
{
  return 0; //-dk:TODO
}


