// -*-C++-*- src/cty_base.cc
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
// Title:   Necessary definitions of ctype_base's static const members
// Version: $Id: cty_base.cpp,v 1.1.1.1 2002/06/05 01:00:16 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include "cxxrt/ctype_base.h"

// -------------------------------------------------------------------------- 

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::space;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::print;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::cntrl;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::upper;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::lower;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::alpha;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::digit;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::punct;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::xdigit;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::alnum;
_CXXRT_STD ctype_base::mask const _CXXRT_STD_NAME:: ctype_base::graph;
#endif
