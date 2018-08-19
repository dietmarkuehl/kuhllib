// -*-C++-*- src/wnumpunc.cc
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
// Title:   Specialized implementation of numpunct<wchar_t> 
// Version: $Id: wnumpunc.cpp,v 1.1.1.1 2002/06/05 01:00:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/numpunct.cc>
#include <cxxrt/ctraits.h>
#include <cxxrt/string.h>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template <>
numpunct<wchar_t>::char_type
numpunct<wchar_t>::do_decimal_point() const
{
  return L'.';
}

template <>
numpunct<wchar_t>::char_type
numpunct<wchar_t>::do_thousands_sep() const
{
  return L',';
}

template<>
numpunct<wchar_t>::string_type
numpunct<wchar_t>::do_truename() const
{
  return string_type(L"true");
}

template<>
numpunct<wchar_t>::string_type
numpunct<wchar_t>::do_falsename() const
{
  return string_type(L"false");
}

// --------------------------------------------------------------------------

template class numpunct<wchar_t>;
template class numpunct_byname<wchar_t>;

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
