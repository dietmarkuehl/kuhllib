// -*-C++-*- src/alloc.cc
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
// Title:   Instantiation of the allocation classes
// Version: $Id: alloc.cpp,v 1.1.1.1 2002/06/05 01:00:03 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrtcf.h>
#include <new>
typedef _CXXRT_STD_NAME::bad_alloc bad_alloc;

#include <cxxrt/alloc.h>
#include <srccxx/stl_malloc_alloc.cc>
#include <srccxx/stl_default_alloc.cc>

#include <exception>
#include <typeinfo>

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

template class __malloc_alloc_template<0>;
template class __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0>;

_CXXRT_NAMESPACE_END
