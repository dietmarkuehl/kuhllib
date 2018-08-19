// <!!-*-C++-*- file: locale.cc --->
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
// Title:   Implementation of the locale Member Functions
// Version: $Id: locale.cpp,v 1.1.1.1 2002/06/05 01:00:13 dietmar_kuehl Exp $
//------------------------------------------------------------------------

#include <cxxrt/locale.h>
#include <cxxrt/ctype.h>
#include <typeinfo>
#include <string>
#include <clocale>
#include <cassert>

_CXXRT_NAMESPACE_BEGIN

extern locale::_CS_intern* _CS_classic_rep();

//------------------------------------------------------------------------
//--- locale member functions: -------------------------------------------
//------------------------------------------------------------------------
//--- constructors/destructors: ---

locale::locale(_CS_intern* r):
  _CS_m_rep(r)
{
  ++_CS_m_rep->_CS_m_refcount;
}

locale::locale() _CXXRT_THROW_SPEC(()):
  _CS_m_rep(_CS_global_rep())
{
  ++_CS_m_rep->_CS_m_refcount;
}

locale::locale(locale const& other) _CXXRT_THROW_SPEC(()):
  _CS_m_rep(other._CS_m_rep)
{
  ++_CS_m_rep->_CS_m_refcount;
}

locale::_CS_intern* locale::_CS_combine(locale const& l, facet const* f, id const& i)
{
  if (f == 0)
    return l._CS_m_rep;

  _CS_intern* rc = new _CS_intern(*l._CS_m_rep, "*");
  rc->_CS_add(i, f);
  rc->_CS_init();

  return rc;
}

locale::~locale()
{
  // assert(_CS_m_rep->_CS_m_refcount > 0);
  if (--_CS_m_rep->_CS_m_refcount == 0)
    delete _CS_m_rep;
}

locale const &locale::operator= (locale const &other) _CXXRT_THROW_SPEC(())
{
  ++other._CS_m_rep->_CS_m_refcount;
  if (--_CS_m_rep->_CS_m_refcount == 0)
    delete _CS_m_rep;
  _CS_m_rep = other._CS_m_rep;
  return *this;
}

//--- access functions: ---

string locale::name() const { return _CS_m_rep->_CS_name(); }

//--- static members: ---

locale::_CS_intern*& locale::_CS_global_rep()
{
  static _CS_intern* val = 0;

  if (val == 0)
    {
      val = _CS_classic_rep();
      ++val->_CS_m_refcount;
    }
  
  return val;
}

locale locale::global(locale const &nloc)
{
  locale rc;                        // safe the current value
  ++nloc._CS_m_rep->_CS_m_refcount;
  --_CS_global_rep()->_CS_m_refcount;  // guaranteed to be non-zero: referenced from rc
  _CS_global_rep() = nloc._CS_m_rep;
  if (string(_CS_global_rep()->_CS_name()) != string("*")) // do not effect the C locale if there is no name!
    ; //-dk:TODO setlocale(LC_ALL, _CS_global_rep()->_CS_name());

#if defined(_CXXRT_USE_FACET_INSTALLS)
  basic_ios<char>::_CS_s_fill = basic_ios<char>::_CS_locale_fill;
  basic_ios<wchar_t>::_CS_s_fill = basic_ios<wchar_t>::_CS_locale_fill;
#endif
  return rc;
}

bool locale::operator==(locale const &l) const
{
  if (l._CS_m_rep == _CS_m_rep)
    return true;

  if (string(_CS_m_rep->_CS_name()) != string("*"))
    return string(_CS_m_rep->_CS_name()) == l._CS_m_rep->_CS_name();
  return false;
}

bool locale::operator!=(locale const &l) const
{
  return !(*this == l);
}

//------------------------------------------------------------------------
//--- locale::id member functions: ---------------------------------------
//------------------------------------------------------------------------

locale::id* locale::id::_CS_list = 0; // zero initialization

// The initialization is somewhat strange but should work...

locale::id::id()
{
  _CS_init(*this);
}

size_t locale::id::_CS_init(locale::id& i)
{
  id*             tmp = _CS_list;
  size_t _CS_count = 1;
  for (; tmp != 0 && tmp != &i; ++_CS_count)
    tmp = tmp->_CS_next;

  if (tmp == 0)
    {
      i._CS_next = _CS_list;
      _CS_list = &i;
      i._CS_identifier = _CS_count;
    }
  return i._CS_identifier;
}

//------------------------------------------------------------------------
//--- locale::facet member functions: ------------------------------------
//------------------------------------------------------------------------
// refs == 0 if the locale takes care of destruction of the facet
// refs == 1 if the facet is destructed otherwise
// => Using refs as initial reference count is appropriate: If it is
// initially == 1, it will never drop to zero.

locale::facet::facet(size_t refs):
  _CS_ref_count(refs)
{
}

locale::facet::~facet()
{
}

void locale::facet::_CS_reference() const
{
  ++_CS_ref_count;
}

bool locale::facet::_CS_release() const
{
  return --_CS_ref_count == 0;
}

locale::facet const*
locale::facet::_CS_object() const
{
  return this;
}

locale::facet const*
locale::facet::_CS_init(_CS_intern const&) const
{
  return this;
}

//------------------------------------------------------------------------

class _CS_use_facet_error: public ::std::bad_cast
{
public:
  ~_CS_use_facet_error() throw();
};

_CS_use_facet_error::~_CS_use_facet_error() throw() {}

void _CS_throw_bad_cast()
{
  throw _CS_use_facet_error();
}

//------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
