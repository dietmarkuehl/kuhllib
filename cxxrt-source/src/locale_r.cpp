// -*-C++-*- locale_r.cc
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
// Title:   Implementation of the locale::_CS_intern member functions
// Version: $Id: locale_r.cpp,v 1.1.1.1 2002/06/05 01:00:43 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/locale.h>
#include <cxxrt/lcache.h>
#include <cstring>
#include <cassert>
#include <cstddef>

// -------------------------------------------------------------------------- 

static const _CXXRT_STD_NAME::size_t initial_size = 20;

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

locale::_CS_intern::_CS_intern(char const* str):
  _CS_m_refcount(0),
  _CS_m_size(initial_size),
  _CS_m_array(new facet const*[initial_size]),
  _CS_m_name(strcpy(new char[strlen(str)], str))
{
  for (size_t i = 0; i < _CS_m_size; ++i)
    _CS_m_array[i] = 0;
}

locale::_CS_intern::_CS_intern(_CS_intern const& ip, char const* str):
  _CS_m_refcount(0),
  _CS_m_size(ip._CS_m_size),
  _CS_m_array(new facet const*[_CS_m_size]),
  _CS_m_name(strcpy(new char[strlen(str)], str))
{
  for (size_t i = 0; i < _CS_m_size; ++i)
    {
      _CS_m_array[i] = ip._CS_m_array[i];
      if (_CS_m_array[i] != 0)
        _CS_m_array[i]->_CS_reference();
    }
}

locale::_CS_intern::~_CS_intern()
{
  for (size_t i = 0; i < _CS_m_size; ++i)
    {
      if (_CS_m_array[i] != 0 && _CS_m_array[i]->_CS_release())
        delete _CS_m_array[i];
    }
  delete[] _CS_m_array;
  delete[] _CS_m_name;
}

locale::facet const* locale::_CS_intern::_CS_add(id const& i, facet const* f)
{
  if (f == 0)
    return f;

  id::_CS_init(const_cast<id&>(i));

  f = f->_CS_init(*this);

  f->_CS_reference();
  size_t idx = i._CS_identifier;

  if (_CS_m_size <= idx)
    {
      facet const** tmp = _CS_m_array;
      _CS_m_array = new facet const*[idx + 1];
      size_t _CS_count = 0;
      for (; _CS_count < _CS_m_size; ++_CS_count)
        if ((_CS_m_array[_CS_count] = tmp[_CS_count]) != 0)
	  _CS_m_array[_CS_count]->_CS_reference();
      for (; _CS_count < idx; ++_CS_count)
        _CS_m_array[_CS_count] = 0;
      _CS_m_size = idx + 1;

      delete[] tmp;
    }
  else
    {
      if (_CS_m_array[idx] != 0 && _CS_m_array[idx]->_CS_release())
        delete _CS_m_array[idx];
    }
  _CS_m_array[idx] = f;
  return f;
}

void locale::_CS_intern::_CS_init()
{
#if !defined(_CXXRT_USE_FACET_INSTALLS)
  _CS_m_array[_CS_cache<char>::id._CS_identifier] = new _CS_cache<char>;
  _CS_m_array[_CS_cache<char>::id._CS_identifier]->_CS_init(*this);

  _CS_m_array[_CS_cache<wchar_t>::id._CS_identifier] = new _CS_cache<wchar_t>;
  _CS_m_array[_CS_cache<wchar_t>::id._CS_identifier]->_CS_init(*this);
#endif
}

_CXXRT_NAMESPACE_END
