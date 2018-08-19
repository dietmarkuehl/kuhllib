// -*-C++-*- ios_base.cc
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
// Title:   Implementation of the ios_base stuff.
// Version: $Id: ios_base.cpp,v 1.1.1.1 2002/06/05 01:00:07 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <cxxrt/ios_base.h>

#if !defined(_CXXRT_TRACE)
#  define _CXXRT_TRACE(str)
#endif

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

ios_base::fmtflags const ios_base::oct;
ios_base::fmtflags const ios_base::dec;
ios_base::fmtflags const ios_base::hex;
ios_base::fmtflags const ios_base::internal;
ios_base::fmtflags const ios_base::left;
ios_base::fmtflags const ios_base::right;
ios_base::fmtflags const ios_base::fixed;
ios_base::fmtflags const ios_base::scientific;
ios_base::fmtflags const ios_base::showbase;
ios_base::fmtflags const ios_base::showpoint;
ios_base::fmtflags const ios_base::showpos;
ios_base::fmtflags const ios_base::skipws;
ios_base::fmtflags const ios_base::unitbuf;
ios_base::fmtflags const ios_base::uppercase;
ios_base::fmtflags const ios_base::boolalpha;
ios_base::fmtflags const ios_base::adjustfield;
ios_base::fmtflags const ios_base::basefield;
ios_base::fmtflags const ios_base::floatfield;

ios_base::iostate const ios_base::goodbit;
ios_base::iostate const ios_base::badbit;
ios_base::iostate const ios_base::eofbit;
ios_base::iostate const ios_base::failbit;

ios_base::openmode const ios_base::in;
ios_base::openmode const ios_base::out;
ios_base::openmode const ios_base::app;
ios_base::openmode const ios_base::ate;
ios_base::openmode const ios_base::trunc;
ios_base::openmode const ios_base::binary;

ios_base::seekdir const ios_base::beg;
ios_base::seekdir const ios_base::cur;
ios_base::seekdir const ios_base::end;

// -------------------------------------------------------------------------- 

ios_base::fmtflags
ios_base::setf(ios_base::fmtflags _f)
{
  _CXXRT_TRACE("std::ios_base::setf(ios_base::fmtflags)");
  fmtflags _rc = _CS_flags;
  _CS_flags |= _f;
  return _rc & _CS_fmtmask;
}

ios_base::fmtflags
ios_base::setf(ios_base::fmtflags _f, ios_base::fmtflags _m)
{
  _CXXRT_TRACE("std::ios_base::setf(ios_base::fmtflags, ios_base::fmtflags)");
  fmtflags _rc = _CS_flags;
#if 1 || _CXXRT_HAS_CONST_EXRPESSION_MEMBER == 1
  _CS_flags = (_CS_flags & ~_m) | (_m & _f);
#else
  _CS_flags = static_cast<fmtflags>((_CS_flags & ~static_cast<unsigned int>(_m)) | (_m & _f));
#endif
  return _rc & _CS_fmtmask;
}

ios_base::fmtflags
ios_base::unsetf(ios_base::fmtflags _m)
{
  _CXXRT_TRACE("std::ios_base::unsetf(ios_base::fmtflags)");
  fmtflags _rc = _CS_flags;
#if 1 || _CXXRT_HAS_CONST_EXRPESSION_MEMBER == 1
  _CS_flags &= ~fmtflags(_m);
#else
  _CS_flags &= static_cast<fmtflags>(~static_cast<unsigned int>(_m));
#endif
  return _rc & _CS_fmtmask;
}

// -------------------------------------------------------------------------- 

ios_base::fmtflags
ios_base::flags(fmtflags f)
{
  _CXXRT_TRACE("std::ios_base::flags(ios_base::fmtflags)");
  fmtflags rc = _CS_flags;
  _CS_flags = f;
  return rc & _CS_fmtmask;
}

streamsize
ios_base::precision(streamsize p)
{
  _CXXRT_TRACE("std::ios_base::precision(streamsize)");
  streamsize rc = _CS_precision;
  _CS_precision = p;
  return rc;
}

streamsize
ios_base::width(streamsize w)
{
  _CXXRT_TRACE("std::ios_base::width(streamsize)");
  streamsize rc = _CS_m_width;
  _CS_m_width = w;
  return rc;
}

// -------------------------------------------------------------------------- 

locale
ios_base::imbue(locale const &_CS_l)
{
  _CXXRT_TRACE("std::ios_base::imbue(locale const&)");
  locale _CS_rc = _CS_loc;
  _CS_loc = _CS_l;
  _CS_call_callbacks(imbue_event);
  return _CS_rc;
}

locale
ios_base::getloc() const
{
  _CXXRT_TRACE("std::ios_base::getloc()");
  return _CS_loc;
}

locale const&
ios_base::_CS_getloc() const
{
  _CXXRT_TRACE("std::ios_base::_CS_getloc()");
  return _CS_loc;
}

// -------------------------------------------------------------------------- 

struct ios_base::_CS_callback
{
  _CS_callback(event_callback fn, int ud, _CS_callback *n):
    function(fn),
    userdata(ud),
    refcount(1),
    next(n)
  {
    _CXXRT_TRACE("std::ios_base::_CS_callback::_CS_callback(event_callback, int, _CS_callback*)");
    if (next != 0)
      ++(next->refcount);
  }

  ~_CS_callback();

  event_callback     function;
  int                userdata;
  int                refcount;
  _CS_callback *next;

private:
  _CS_callback(_CS_callback const&);
  void operator= (_CS_callback const&);
};


ios_base::_CS_callback::~_CS_callback()
{
  _CXXRT_TRACE("std::ios_base::_CS_callback::~_CS_callback()");
  if (next != 0 && --(next->refcount) == 0)
    delete next;
}

static void _CS_call_real(ios_base& fmt, ios_base::_CS_callback* lst, ios_base::event e)
{
  _CXXRT_TRACE("std::_CS_call_real(ios_base&, ios_base::_CS_callback*, ios_base::event)");
  for (ios_base::_CS_callback *cb = lst; cb != 0; cb = cb->next)
    (cb->function)(e, fmt, cb->userdata);
}

static void _CS_delete_real(ios_base::_CS_callback* lst)
{
  _CXXRT_TRACE("std::_CS_delete_real(ios_base::_CS_callback*)");
  if (lst != 0 && --(lst->refcount) == 0)
    delete lst;
}

static void (*_CS_caller)(ios_base&, ios_base::_CS_callback*, ios_base::event) = 0;
static void (*_CS_deleter)(ios_base::_CS_callback*) = 0;

void ios_base::register_callback(ios_base::event_callback fn, int ud)
{
  _CXXRT_TRACE("std::register_callback(ios_base::event_callback, int)");
  _CS_list = new _CS_callback(fn, ud, _CS_list);
  _CS_caller = _CS_call_real;
  _CS_deleter = _CS_delete_real;
}

void ios_base::_CS_call_callbacks(ios_base::event e)
{
  _CXXRT_TRACE("std::_CS_call_callbacks(ios_base::event)");
  if (_CS_caller != 0)
    _CS_caller(*this, _CS_list, e);
}

// -------------------------------------------------------------------------- 

ios_base::ios_base():
  _CS_flags(dec | skipws), //-dk:TODO check
  _CS_precision(6),
  _CS_m_width(0),
  _CS_loc(),
  _CS_list(0),
  _CS_words()
{
  _CXXRT_TRACE("std::ios_base::ios_base()");
}

ios_base::~ios_base()
{
  _CXXRT_TRACE("std::ios_base::~ios_base()");
  _CS_call_callbacks(erase_event);
  if (_CS_deleter)
    _CS_deleter(_CS_list);
}

// ---------------------------------------------------------------------------

static int _CS_xalloc_idx = 0;

int
ios_base::xalloc()
{
  _CXXRT_TRACE("std::ios_base::xalloc()");
  return _CS_xalloc_idx++;
}

// ---------------------------------------------------------------------------

streamsize _CS_std_bufsize = 1;

bool ios_base::sync_with_stdio(bool arg)
{
  _CXXRT_TRACE("std::ios_base::sync_with_stdio(bool)");
  // this library is alway in sync with stdio
  static bool last_arg = true;
  bool rc = last_arg;
  last_arg = arg;
  if (last_arg)
    _CS_std_bufsize = 1;
  else
    _CS_std_bufsize = 65536; //-dk:TODO use constant
  return rc;
}

// ---------------------------------------------------------------------------

long &ios_base::iword(int index)
{
  _CXXRT_TRACE("std::ios_base::iword(int)");
  //-dk:TODO error handling
  return _CS_words[index]._CS_iword;
}

void *&ios_base::pword(int index)
{
  _CXXRT_TRACE("std::ios_base::pword(int)");
  //-dk:TODO error handling
  return _CS_words[index]._CS_pword;
}

// ---------------------------------------------------------------------------

ios_base &ios_base::_CS_copyfmt(ios_base const &cpy)
{
  _CXXRT_TRACE("std::ios_base::_CS_copyfmt(ios_base const&)");
  _CS_flags = cpy._CS_flags;
  _CS_precision = cpy._CS_precision;
  _CS_m_width = cpy._CS_m_width;

  _CS_loc = cpy._CS_loc;

  if (cpy._CS_list != 0)
    ++(cpy._CS_list->refcount);
  if (_CS_list != 0 && --(_CS_list->refcount) == 0)
    delete _CS_list;
  _CS_list = cpy._CS_list;

  _CS_words.clear();
  for (_CS_map<int, _CS_word_rec>::iterator tmp(cpy._CS_words.begin());
       tmp != cpy._CS_words.end();
       ++tmp)
    _CS_words[(*tmp).first] = (*tmp).second;

  return *this;
}

// ---------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
