// -*-C++-*- cxxrt/sstr_fwd.h
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
// Title:   Forward declaration of the string stream types
// Version: $Id: stdexcept.h,v 1.1.1.1 2002/06/05 01:01:56 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_STDEXCEPT_H__)
#define _CXXRT_STDEXCEPT_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#include <exception>
#if !defined(_STL_STRING_FWD_H_)
#  include <stl_string_fwd.h>
#endif


_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

class _CS_named_exception:
  public exception
{
public:
  _CS_named_exception(string const&);
  virtual char const* what() const throw();

private:
  enum { _CS_bufsiz = 256 };
  char _CS_m_what[_CS_bufsiz];
};

// -------------------------------------------------------------------------- 

class logic_error:
  public _CS_named_exception
{
public:
  logic_error(string const&);
};

class runtime_error:
  public _CS_named_exception
{
public:
  runtime_error(string const&);
};

class domain_error:
  public logic_error
{
public:
  domain_error(string const&);
};

class invalid_argument:
  public logic_error
{
public:
  invalid_argument(string const&);
};

class length_error:
  public logic_error
{
public:
  length_error(string const&);
};

class out_of_range:
  public logic_error
{
public:
  out_of_range(string const&);
};

class range_error:
  public runtime_error
{
public:
  range_error(string const&);
};

class overflow_error:
  public runtime_error
{
public:
  overflow_error(string const&);
};

class underflow_error:
  public runtime_error
{
public:
  underflow_error(string const&);
};

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_STDEXCEPT_H__ */
