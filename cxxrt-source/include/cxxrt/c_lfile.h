// -*-C++-*- include/cxxrt/c_lfile.h
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
// Title:  A simple buffered file used in reading locale data
// Version: $Id: c_lfile.h,v 1.1.1.1 2002/06/05 01:01:51 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_C_LFILE_H__)
#define _CXXRT_C_LFILE_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_SYSFILE_H__)
#  include <cxxrt/sysfile.h>
#endif

// --------------------------------------------------------------------------

struct _Locale_file
{
public:
  _Locale_file();
  ~_Locale_file();
        
  bool _CS_open(char const*);    // try to open the file
  bool _CS_failed() const;       // return whether a read operation failed
  bool _CS_is_open() const;      // return whether the file is open
  
  bool _CS_read(char&);          // read a single character
  bool _CS_read(int&);           // read a simple formatted integer and the following whitespace
  bool _CS_read(char*, int);     // read a string of length len and the following whitespace

  char const* _CS_read_string(); // read string into new[]ed buffer
  
private:
  _Locale_file(_Locale_file const&);
  void operator= (_Locale_file const&);

  char*                           _CS_m_buffer;
  char*                           _CS_m_current;
  char*                           _CS_m_end;
  _CXXRT_STD_NAME:: _CS_sysfile _CS_m_file;
  bool                            _CS_m_ok;
};

// --------------------------------------------------------------------------

#endif /* _CXXRT_C_LFILE_H__ */
