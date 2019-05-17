// -*-C++-*- cxxrt/iostream.h
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
// Title:   Declaration of the class basic_iostream
// Version: $Id: iostream.h,v 1.1.1.1 2002/06/05 01:01:47 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_IOSTREAM_H__)
#define _CXXRT_IOSTREAM_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

#if !defined(_CXXRT_IOSFWD_H__)
#  include <cxxrt/iosfwd.h>
#endif
#if !defined(_CXXRT_ISTREAM_H__)
#  include <cxxrt/istream.h>
#endif
#if !defined(_CXXRT_OSTREAM_H__)
#  include <cxxrt/ostream.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
class basic_iostream:
        public basic_istream<_CS_cT, _CS_traits>,
        public basic_ostream<_CS_cT, _CS_traits>
{
public:
  using basic_istream<_CS_cT, _CS_traits>::char_type;

  explicit basic_iostream(basic_streambuf<_CS_cT, _CS_traits>*);
  virtual ~basic_iostream();
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_IOSTREAM_CC__)
#  include <srccxx/iostream.cc>
#endif

#endif /* _CXXRT_IOSTREAM_H__ */
