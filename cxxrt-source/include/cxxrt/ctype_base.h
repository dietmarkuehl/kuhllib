// -*-C++-*- cxxrt/ctype_base.h
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
// Title:   Declaration of the class common to all ctype facets
// Version: $Id: ctype_base.h,v 1.1.1.1 2002/06/05 01:01:58 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_CTYPE_BASE_H__)
#define _CXXRT_CTYPE_BASE_H__ 1

#if !defined(_CXXRTCF_H__)
#  include "cxxrtcf.h"
#endif
#if !defined(_CXXRT_C_LOCDEF_H__)
#  include <cxxrt/c_locdef.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------
// The class code(ctype_base) defines a bitmask type used by all ctype
// facets (22.2.1).

class ctype_base
{
public:
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 0
  enum mask
  {
    space  = _Locale_SPACE,
    print  = _Locale_PRINT,
    cntrl  = _Locale_CNTRL,
    upper  = _Locale_UPPER,
    lower  = _Locale_LOWER,
    alpha  = _Locale_ALPHA,
    digit  = _Locale_DIGIT,
    punct  = _Locale_PUNCT,
    xdigit = _Locale_XDIGIT,
    alnum  = alpha | digit,
    graph  = alnum | punct
  };
#else
  typedef _Locale_mask_t mask;

  static mask const  space  = _Locale_SPACE;
  static mask const  print  = _Locale_PRINT;
  static mask const  cntrl  = _Locale_CNTRL;
  static mask const  upper  = _Locale_UPPER;
  static mask const  lower  = _Locale_LOWER;
  static mask const  alpha  = _Locale_ALPHA;
  static mask const  digit  = _Locale_DIGIT;
  static mask const  punct  = _Locale_PUNCT;
  static mask const  xdigit = _Locale_XDIGIT;
  static mask const  alnum  = alpha | digit;
  static mask const  graph  = alnum | punct;
#endif
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_CTYPE_BASE_H__ */
