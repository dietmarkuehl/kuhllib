// -*-C++-*- new.cpp
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
// Title:   Implementation of the memory management functions
// Version: $Id: new.cpp,v 1.1.1.1 2002/06/05 01:00:16 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include <new>

#if defined(_MSC_VER)
void* operator new[](_CXXRT_STD size_t sz) { return operator new(sz); }
void operator delete[](void* ptr) { operator delete(ptr); }
#endif

// #if defined(__EDG__)
void* operator new(_CXXRT_STD size_t size, _CXXRT_STD nothrow_t const&) throw()
{
  try
  {
    return operator new(size);
  }
  catch (...)
  {
    return 0;
  }
}
 
void* operator new[](_CXXRT_STD size_t size, _CXXRT_STD nothrow_t const&) throw()
{
  try
  {
    return operator new[](size);
  }
  catch (...)
  {
    return 0;
  }
}
 
void operator delete (void* _CS_ptr, const _CXXRT_STD nothrow_t&) throw() { return operator delete(_CS_ptr); }
void operator delete[] (void* _CS_ptr, const _CXXRT_STD nothrow_t&) throw() { return operator delete[](_CS_ptr); }
// #endif
