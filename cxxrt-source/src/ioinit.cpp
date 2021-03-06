// -*-C++-*- ioinit.cc
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
// Title:   Initialization and deinitialization of the standard stream objects.
// Version: $Id: ioinit.cpp,v 1.3 2003/04/13 19:36:46 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ios_base.h>
#include <cxxrt/ioobj.h>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

unsigned int ios_base::Init::_CS_count = 0;

ios_base::Init::Init()
{
  ++_CS_count;
  //-dk:TODO this should actually initialize the IOStream classes
}

ios_base::Init::~Init()
{
  if (--_CS_count == 0)
    {
      cout.flush();
      cerr.flush();
      clog.flush();

      wcout.flush();
      wcerr.flush();
      wclog.flush();
    }
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
