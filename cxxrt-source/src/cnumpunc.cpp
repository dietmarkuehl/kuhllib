// -*-C++-*- src/cnumpunc.cc
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
// Title:   Specialized implementation of numpunct<char> 
// Version: $Id: cnumpunc.cpp,v 1.1.1.1 2002/06/05 01:00:18 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/numpunct.cc>
#include <cxxrt/ctraits.h>
#include <cxxrt/string.h>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template <>
numpunct<char>::char_type
numpunct<char>::do_decimal_point() const
{
  return '.';
}

template <>
numpunct<char>::char_type
numpunct<char>::do_thousands_sep() const
{
  return ',';
}

template<>
numpunct<char>::string_type
numpunct<char>::do_truename() const
{
  return string_type("true");
}

template<>
numpunct<char>::string_type
numpunct<char>::do_falsename() const
{
  return string_type("false");
}

// --------------------------------------------------------------------------

template class numpunct<char>;
template class numpunct_byname<char>;

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
