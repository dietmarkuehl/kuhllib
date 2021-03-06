// -*-C++-*- wwnum_ge.cc
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
// Title:   Explicit instantiation of num_get<wchar_t, wchar_t const *> 
// Version: $Id: wwnum_ge.cpp,v 1.1.1.1 2002/06/05 01:00:14 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/num_get.cc>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template class  num_get<wchar_t, wchar_t const *>;
template class  num_get<wchar_t, _CXXRT_STD istreambuf_iterator<wchar_t> >;

template bool _CS_get_float(_CS_extractor<wchar_t, wchar_t const*>&, ios_base::iostate&, long double&);
template bool _CS_get_float(_CS_extractor<wchar_t, istreambuf_iterator<wchar_t> >&, ios_base::iostate&, long double&);

_CXXRT_NAMESPACE_END
