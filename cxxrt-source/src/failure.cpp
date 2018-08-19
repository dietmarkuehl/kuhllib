// -*-C++-*- failure.cc
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
// Title:   Implementation of the ios_base::failure
// Version: $Id: failure.cpp,v 1.1.1.1 2002/06/05 01:00:02 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ios_base.h>
#include <cxxrt/string.h>
#include <cstring>

// -------------------------------------------------------------------------- 

static char const *__cxxrt_strdup(char const *str)
{
  char *rc = new char[_CXXRT_STD_NAME::strlen(str) + 1];
  return _CXXRT_STD_NAME::strcpy(rc, str);
}

// -------------------------------------------------------------------------- 

_CXXRT_STD_NAME:: ios_base::failure::failure(char const* msg):
  ::std::exception(),
  _CS_message(__cxxrt_strdup(msg))
{
}

_CXXRT_STD_NAME:: ios_base::failure::failure(_CXXRT_STD string const &msg):
  ::std::exception(),
  _CS_message(__cxxrt_strdup(msg.c_str()))
{
}

_CXXRT_STD_NAME::
ios_base::failure::failure(_CXXRT_STD ios_base::failure const &f):
  ::std::exception(f),
  _CS_message(__cxxrt_strdup(f._CS_message))
{
}

_CXXRT_STD ios_base::failure &
_CXXRT_STD_NAME:: ios_base::failure::operator= (_CXXRT_STD ios_base::failure const &f)
{
  if (this != & f)
    {
      delete [] const_cast<char*>(_CS_message);
      _CS_message = __cxxrt_strdup(f._CS_message);
    }
  return *this;
}

_CXXRT_STD_NAME:: ios_base::failure::~failure() throw()
{
  delete [] const_cast<char*>(_CS_message);
}

char const *_CXXRT_STD_NAME:: ios_base::failure::what() const throw()
{
  return _CS_message;
}
