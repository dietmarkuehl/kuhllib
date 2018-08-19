// -*-C++-*- num_putf.cc
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
// Title:   Implementation of functions doing float formatting
// Version: $Id: num_putf.cpp,v 1.1.1.1 2002/06/05 01:00:24 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/num_put.h>
#include <cxxrt/ios_base.h>
#include <cxxrt/string.h>
#include <cstdio>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

static string num_put_format(ios_base &fmt, double value)
{
  char buffer[1024];

  static char const* formats[] = {
    " %.*e", " %+.*e", " %#.*e", " %#+.*e",
    " %.*f", " %+.*f", " %#.*f", " %#+.*f",
    " %.*g", " %+.*g", " %#.*g", " %#+.*g",

    "%.*e ", "%+.*e ", "%#.*e ", "%#+.*e ",
    "%.*f ", "%+.*f ", "%#.*f ", "%#+.*f ",
    "%.*g ", "%+.*g ", "%#.*g ", "%#+.*g ",
  };

  int index = (fmt.flags() & ios_base::showpos? 1: 0)
    + (fmt.flags() & ios_base::showpoint? 2: 0);

  switch (fmt.flags() & ios_base::floatfield)
    {
    case ios_base::scientific:
      break;
    case ios_base::fixed:
      index += 4;
      break;
    default:
      index += 8;
      break;
    }

  switch (fmt.flags() & ios_base::adjustfield)
    {
    case ios_base::left:
      index += 12;
      break;
    case ios_base::internal:
      index += 12;
      break;
    case ios_base::right:
    default:
      break;
    }
      
  // ecvt(value, fmt.precision(), buffer);
  // gcvt(value, fmt.precision(), buffer);
  sprintf(buffer, formats[index], (int)fmt.precision(), value);
  // strcpy(buffer, "-dk:TODO!");

  return buffer;
}

// -------------------------------------------------------------------------- 

string __do_num_put(ios_base &fmt, float value)
{
  return num_put_format(fmt, value);
}

string __do_num_put(ios_base &fmt, double value)
{
  return num_put_format(fmt, value);
}

string __do_num_put(ios_base &fmt, long double value)
{
  return num_put_format(fmt, static_cast<double>(value));
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
