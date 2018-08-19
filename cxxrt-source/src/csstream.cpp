// -*-C++-*- sstream.cc
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
// Title:   Instantiations of the string stream classes for character type char
// Version: $Id: csstream.cpp,v 1.1.1.1 2002/06/05 01:00:13 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/sstream.cc>

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

#if defined(__STL_USE_STD_ALLOCATORS)
template class basic_stringbuf<char, char_traits<char>, allocator<char> >;
template class _CS_basic_sstream_pbase<char, char_traits<char>, allocator<char> >;
template class basic_istringstream<char, char_traits<char>, allocator<char> >;
template class basic_ostringstream<char, char_traits<char>, allocator<char> >;
template class basic_stringstream<char, char_traits<char>, allocator<char> >;
#else
template class basic_stringbuf<char, char_traits<char>, alloc>;
template class _CS_basic_sstream_pbase<char, char_traits<char>, alloc>;
template class basic_istringstream<char, char_traits<char>, alloc>;
template class basic_ostringstream<char, char_traits<char>, alloc>;
template class basic_stringstream<char, char_traits<char>, alloc>;
#endif

_CXXRT_NAMESPACE_END
