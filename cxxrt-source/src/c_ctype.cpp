// -*-C++-*- src/c_ctype.cc
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
// Title:   Definition of the general low-level interface of the ctype facet
// Version: $Id: c_ctype.cpp,v 1.1.1.1 2002/06/05 01:00:10 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include "cxxrt/c_locale.h"

// -------------------------------------------------------------------------- 

struct _Locale_ctype* _Locale_ctype_create(char const*)
{
	return 0; //-dk:TODO
}

void _Locale_ctype_destroy(struct _Locale_ctype*)
{
	//-dk:TODO
}

char * _Locale_ctype_name(struct _Locale_ctype const*, char*)
{
	return 0; //-dk:TODO
}
