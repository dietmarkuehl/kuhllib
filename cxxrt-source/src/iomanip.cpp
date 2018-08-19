// -*-C++-*- iomanip.cc
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
// Title:   Implementation of the iomanip stuff.
// Version: $Id: iomanip.cpp,v 1.1.1.1 2002/06/05 01:00:10 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/iomanip.cc>

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

_CS_resetiosflags resetiosflags(ios_base::fmtflags _CS_f)
{
  return _CS_resetiosflags(_CS_f);
}

void _CS_resetiosflags_fct(ios_base &_CS_strm, _CS_resetiosflags const &_CS_f)
{
  _CS_strm.setf(ios_base::fmtflags(0), _CS_f._CS_flags);
}

// -------------------------------------------------------------------------- 

_CS_setiosflags setiosflags(ios_base::fmtflags _CS_f)
{
  return _CS_setiosflags(_CS_f);
}

void _CS_setiosflags_fct(ios_base &_CS_strm, _CS_setiosflags const &_CS_f)
{
  _CS_strm.setf(_CS_f._CS_flags);
}

// -------------------------------------------------------------------------- 

_CS_setbase setbase(int _CS_b)
{
  return _CS_setbase(_CS_b);
}

void _CS_setbase_fct(ios_base &_CS_strm, _CS_setbase const &_CS_b)
{
  switch (_CS_b._CS_base)
    {
    case 8: _CS_strm.setf(ios_base::oct, ios_base::basefield); break;
    case 10: _CS_strm.setf(ios_base::dec, ios_base::basefield); break;
    case 16: _CS_strm.setf(ios_base::hex, ios_base::basefield); break;
    default: _CS_strm.setf(ios_base::fmtflags(0), ios_base::basefield); break;
    }
}

// -------------------------------------------------------------------------- 

_CS_setprecision setprecision(int _CS_p)
{
  return _CS_setprecision(_CS_p);
}

// -------------------------------------------------------------------------- 

_CS_setw setw(int _CS_w)
{
  return _CS_setw(_CS_w);
}

void _CS_setw_fct(ios_base &_CS_strm, _CS_setw const &_CS_w)
{
  _CS_strm.width(_CS_w._CS_width);
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
