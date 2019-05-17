// -*-C++-*- cxxrt/c_locdef.h
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
// Title:  Definitions which have to be visible in public headers
// Version: $Id: c_locdef.h,v 1.1.1.1 2002/06/05 01:01:51 dietmar_kuehl Exp $

// Note: The implementation from Matt Austern (SGI) uses the same
// layer. Is is done to support switching implementations. The
// definition of this interface is from Matt Austern.

// This implementation uses a portable approach despite what is said
// in Matt's documentation: The "trick" is to ignore what is already
// provided by the platform...
// --------------------------------------------------------------------------

#if !defined(_CXXRT_C_LOCDEF_H__)
#define _CXXRT_C_LOCDEF_H__ 1

// --------------------------------------------------------------------------

#define _Locale_SPACE  (1<<0)
#define _Locale_PRINT  (1<<1)
#define _Locale_CNTRL  (1<<2)
#define _Locale_UPPER  (1<<3)
#define _Locale_LOWER  (1<<4)
#define _Locale_ALPHA  (1<<5)
#define _Locale_DIGIT  (1<<6)
#define _Locale_PUNCT  (1<<7)
#define _Locale_XDIGIT (1<<8)
#define _Locale_BLANK  (1<<9)

typedef unsigned short _Locale_mask_t;

// --------------------------------------------------------------------------

#endif /* _CXXRT_C_LOCDEF_H__ */
