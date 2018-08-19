// -*-C++-*- src/c_lfile.cc
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
// Title:   Implementation of a simple file used to locale data
// Version: $Id: c_lfile.cpp,v 1.1.1.1 2002/06/05 01:00:16 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include "cxxrt/c_lfile.h"
#include "cxxrt/ios_base.h"

// -------------------------------------------------------------------------- 
  
_Locale_file::_Locale_file():
  _CS_m_buffer(0),
  _CS_m_current(0),
  _CS_m_end(0),
  _CS_m_file(),
  _CS_m_ok(false)
{
}

_Locale_file::~_Locale_file()
{
  delete[] _CS_m_buffer;
}
	
bool _Locale_file::_CS_open(char const* fname)
{
  if (_CS_m_file._CS_open(fname, _CXXRT_STD_NAME::ios_base::in))
    {
      _CS_m_buffer = new char[_CS_m_file._CS_bufsiz()];
      _CS_m_current = _CS_m_end = _CS_m_buffer;
      _CS_m_ok = true;
    }
  else
    return false;
  return true;
}

bool _Locale_file::_CS_failed() const
{
  return !_CS_m_ok;
}

bool _Locale_file::_CS_is_open() const
{
  return _CS_m_file._CS_is_open();
}

bool _Locale_file::_CS_read(char& c)
{
  if (!_CS_m_ok)
    return false;
  
  if (_CS_m_current == _CS_m_end)
    {
      if (_CS_m_file._CS_is_open())
	{
	  _CXXRT_STD streamsize sz = _CS_m_file._CS_read(_CS_m_buffer, _CS_m_file._CS_bufsiz());
	  
	  _CS_m_end = _CS_m_buffer + sz;
	  _CS_m_current = _CS_m_buffer;
	}

      if (_CS_m_current == _CS_m_end)
	return _CS_m_ok = false;
    }
  c = *_CS_m_current++;
  return true;
}

bool _Locale_file::_CS_read(int& i)
{
  if (!_CS_m_ok)
    return false;

  i = 0;
  char c;
  if (!_CS_read(c) || c < '0' || '9' < c)
    return _CS_m_ok = false;
  else
    do
      i = i * 10 + c - '0';
    while ((_CS_m_ok = _CS_read(c)) && '0' <= c && c <= '9');

  return _CS_m_ok;
}

bool _Locale_file::_CS_read(char* str, int len)
{
  if (!_CS_m_ok)
    return false;

  while (len--)
    if (!_CS_read(*str++))
      return _CS_m_ok = false;
  *str = 0;
  
  char c;
  return _CS_m_ok = _CS_read(c); // skip the space after the string
}

char const* _Locale_file::_CS_read_string()
{
  if (!_CS_m_ok)
    return false;

  int len;
  if (!_CS_read(len))
    return 0;
  
  char* str = new char[len + 1];
  if (!_CS_read(str, len))
    {
      delete[] str;
      return 0;
    }
  
  return str;
}
