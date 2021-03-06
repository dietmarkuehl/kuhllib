// -*-C++-*- wsstream.cc
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
// Title:   Instantiations of the string stream classes for character type wchar_t
// Version: $Id: wsstream.cpp,v 1.1.1.1 2002/06/05 01:00:05 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/sstream.cc>

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

#if defined(__STL_USE_STD_ALLOCATORS)
template class basic_stringbuf<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >;
template class _CS_basic_sstream_pbase<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >;
template class basic_istringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >;
template class basic_ostringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >;
template class basic_stringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >;
#else
template class basic_stringbuf<wchar_t, char_traits<wchar_t>, alloc>;
template class _CS_basic_sstream_pbase<wchar_t, char_traits<wchar_t>, alloc>;
template class basic_istringstream<wchar_t, char_traits<wchar_t>, alloc>;
template class basic_ostringstream<wchar_t, char_traits<wchar_t>, alloc>;
template class basic_stringstream<wchar_t, char_traits<wchar_t>, alloc>;
#endif

_CXXRT_NAMESPACE_END
