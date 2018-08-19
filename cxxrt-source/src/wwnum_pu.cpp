// -*-C++-*- wwnum_pu.cc
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
// Title:   Explicit instantiation of num_put<wchar_t, wchar_t *> 
// Version: $Id: wwnum_pu.cpp,v 1.1.1.1 2002/06/05 01:00:18 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/num_put.cc>

_CXXRT_NAMESPACE_BEGIN

template wchar_t *_CS_copy(wchar_t const *, wchar_t const *, wchar_t *);
template wchar_t *_CS_fill_n(wchar_t *, int, wchar_t);
template wchar_t *_CS_num_put(wchar_t *, ios_base &, wchar_t fill, string str);

template ostreambuf_iterator<wchar_t> _CS_new_num_put(ostreambuf_iterator<wchar_t>&, ios_base&, wchar_t, long);
template wchar_t* _CS_new_num_put(wchar_t*&, ios_base&, wchar_t, long);

template ostreambuf_iterator<wchar_t> _CS_copy(wchar_t*, wchar_t*, ostreambuf_iterator<wchar_t>);
template ostreambuf_iterator<wchar_t> _CS_copy(wchar_t const *, wchar_t const *, ostreambuf_iterator<wchar_t>);
template ostreambuf_iterator<wchar_t> _CS_fill_n(ostreambuf_iterator<wchar_t>, int, wchar_t);
template ostreambuf_iterator<wchar_t> _CS_num_put(ostreambuf_iterator<wchar_t>, ios_base &, wchar_t fill, string str);

template class num_put<wchar_t, wchar_t *>;
template class num_put<wchar_t, ostreambuf_iterator<wchar_t> >;

_CXXRT_NAMESPACE_END
