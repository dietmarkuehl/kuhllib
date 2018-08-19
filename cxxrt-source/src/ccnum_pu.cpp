// -*-C++-*- ccnum_pu.cc
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
// Title:   Explicit instantiation of num_put<char, char *> 
// Version: $Id: ccnum_pu.cpp,v 1.1.1.1 2002/06/05 01:00:09 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/num_put.cc>

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

template ostreambuf_iterator<char> _CS_copy(char *, char *, ostreambuf_iterator<char>);
template ostreambuf_iterator<char> _CS_fill_n(ostreambuf_iterator<char>, int, char);
template ostreambuf_iterator<char> _CS_num_put(ostreambuf_iterator<char>, ios_base &, char fill, string str);

template ostreambuf_iterator<char> _CS_new_num_put(ostreambuf_iterator<char>&, ios_base&, char, long);
template class num_put<char, ostreambuf_iterator<char> >;

#if 0
template char *_CS_copy(char const *, char const *, char *);
template char *_CS_fill_n(char *, int, char);
template char *_CS_num_put(char *, ios_base &, char fill, string str);

template char* _CS_new_num_put(char*&, ios_base&, char, long);
template class num_put<char, char *>;
#endif

_CXXRT_NAMESPACE_END
