// -*-C++-*- srccxx/fstream.cc
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
// Title:  Implementation of the file stream classes
// Version: $Id: fstream.cc,v 1.2 2006/04/07 07:07:13 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_FSTREAM_CC__)
#define __SRCCXX_FSTREAM_CC__ 1

#if !defined(_CXXRT_FSTREAM_H__)
#  include <cxxrt/fstream.h>
#endif

// #include <unistd.h>

#if _CXXRT_IO_TYPEDEFS == 0
  typedef _CXXRT_STREAMSIZE streamsize;
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
_CS_basic_fstream_pbase<_CS_charT, _CS_traits>::_CS_basic_fstream_pbase():
  _CS_m_buf()
{
}

template <class _CS_charT, class _CS_traits>
_CS_basic_fstream_pbase<_CS_charT, _CS_traits>::~_CS_basic_fstream_pbase()
{
}

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_charT, class _CS_traits>
basic_ifstream<_CS_charT, _CS_traits>::basic_ifstream(basic_ifstream const&):
  _CS_basic_fstream_pbase<_CS_charT, _CS_traits>(),
  basic_ios<_CS_charT, _CS_traits>(&_CS_m_buf),
  basic_istream<_CS_charT, _CS_traits>(&_CS_m_buf)
{
  init(&_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
void
basic_ifstream<_CS_charT, _CS_traits>::operator= (basic_ifstream const&)
{
}
#endif


template <class _CS_charT, class _CS_traits>
basic_ifstream<_CS_charT, _CS_traits>::basic_ifstream():
  basic_ios<_CS_charT, _CS_traits>(&this->_CS_m_buf),
  basic_istream<_CS_charT, _CS_traits>(&this->_CS_m_buf)
{
  this->init(&this->_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
basic_ifstream<_CS_charT, _CS_traits>::basic_ifstream(char const* _CS_fname,
                                                                  ios_base::openmode _CS_oflags):
  basic_ios<_CS_charT, _CS_traits>(&this->_CS_m_buf),
  basic_istream<_CS_charT, _CS_traits>(&this->_CS_m_buf)
{
  this->init(&this->_CS_m_buf);
  if (this->_CS_m_buf.open(_CS_fname, _CS_oflags | ios_base::in) == 0)
    this->setstate(ios_base::failbit);
}

template <class _CS_charT, class _CS_traits>
basic_filebuf<_CS_charT, _CS_traits>*
basic_ifstream<_CS_charT, _CS_traits>::rdbuf() const
{
  return &this->_CS_m_buf;
}

template <class _CS_charT, class _CS_traits>
bool
basic_ifstream<_CS_charT, _CS_traits>::is_open() const
{
  return this->_CS_m_buf.is_open();
}

template <class _CS_charT, class _CS_traits>
void
basic_ifstream<_CS_charT, _CS_traits>::open(char const* _CS_fname,
                                                        ios_base::openmode _CS_oflags)
{
  if (this->_CS_m_buf.open(_CS_fname, _CS_oflags | ios_base::in))
    this->init(&this->_CS_m_buf);
  else
    this->setstate(ios_base::failbit);
}

template <class _CS_charT, class _CS_traits>
void
basic_ifstream<_CS_charT, _CS_traits>::close()
{
  this->_CS_m_buf.close();
}

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_charT, class _CS_traits>
basic_ofstream<_CS_charT, _CS_traits>::basic_ofstream(basic_ofstream const&):
  basic_ios<_CS_charT, _CS_traits>(&_CS_m_buf),
  basic_ostream<_CS_charT, _CS_traits>(&_CS_m_buf)
{
}

template <class _CS_charT, class _CS_traits>
void
basic_ofstream<_CS_charT, _CS_traits>::operator= (basic_ofstream const&)
{
}
#endif

template <class _CS_charT, class _CS_traits>
basic_ofstream<_CS_charT, _CS_traits>::basic_ofstream():
  basic_ios<_CS_charT, _CS_traits>(&this->_CS_m_buf),
  basic_ostream<_CS_charT, _CS_traits>(&this->_CS_m_buf)
{
  this->init(&this->_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
basic_ofstream<_CS_charT, _CS_traits>::basic_ofstream(char const* _CS_fname,
                                                                  ios_base::openmode _CS_oflags):
  basic_ios<_CS_charT, _CS_traits>(&this->_CS_m_buf),
  basic_ostream<_CS_charT, _CS_traits>(&this->_CS_m_buf)
{
  this->_CS_m_buf.open(_CS_fname, _CS_oflags | ios_base::out);
  this->init(&this->_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
basic_filebuf<_CS_charT, _CS_traits>*
basic_ofstream<_CS_charT, _CS_traits>::rdbuf() const
{
  return &this->_CS_m_buf;
}

template <class _CS_charT, class _CS_traits>
bool
basic_ofstream<_CS_charT, _CS_traits>::is_open() const
{
  return this->_CS_m_buf.is_open();
}

template <class _CS_charT, class _CS_traits>
void
basic_ofstream<_CS_charT, _CS_traits>::open(char const* _CS_fname,
                                            ios_base::openmode _CS_oflags)
{
  this->_CS_m_buf.open(_CS_fname, _CS_oflags | ios_base::out);
  this->init(&this->_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
void
basic_ofstream<_CS_charT, _CS_traits>::close()
{
  this->_CS_m_buf.close();
}

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_charT, class _CS_traits>
basic_fstream<_CS_charT, _CS_traits>::basic_fstream(basic_fstream const&):
  _CS_basic_fstream_pbase<_CS_charT, _CS_traits>(),
  basic_ios<_CS_charT, _CS_traits>(&_CS_m_buf),
  basic_iostream<_CS_charT, _CS_traits>(&_CS_m_buf)
{
  init(&_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
void
basic_fstream<_CS_charT, _CS_traits>::operator= (basic_fstream const&)
{
}
#endif

template <class _CS_charT, class _CS_traits>
basic_fstream<_CS_charT, _CS_traits>::basic_fstream():
  _CS_basic_fstream_pbase<_CS_charT, _CS_traits>(),
  basic_ios<_CS_charT, _CS_traits>(&this->_CS_m_buf),
  basic_iostream<_CS_charT, _CS_traits>(&this->_CS_m_buf)
{
  this->init(&this->_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
basic_fstream<_CS_charT, _CS_traits>::basic_fstream(char const* _CS_fname,
                                                    ios_base::openmode _CS_oflags):
  basic_ios<_CS_charT, _CS_traits>(&this->_CS_m_buf),
  basic_iostream<_CS_charT, _CS_traits>(&this->_CS_m_buf)
{
  this->_CS_m_buf.open(_CS_fname, _CS_oflags | ios_base::in | ios_base::out);
  this->init(&this->_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
basic_filebuf<_CS_charT, _CS_traits>*
basic_fstream<_CS_charT, _CS_traits>::rdbuf() const
{
  return &this->_CS_m_buf;
}

template <class _CS_charT, class _CS_traits>
bool
basic_fstream<_CS_charT, _CS_traits>::is_open() const
{
  return this->_CS_m_buf.is_open();
}

template <class _CS_charT, class _CS_traits>
void
basic_fstream<_CS_charT, _CS_traits>::open(char const* _CS_fname,
                                                         ios_base::openmode _CS_oflags)
{
  this->_CS_m_buf.open(_CS_fname, _CS_oflags | ios_base::in | ios_base::out);
  init(&this->_CS_m_buf);
}

template <class _CS_charT, class _CS_traits>
void
basic_fstream<_CS_charT, _CS_traits>::close()
{
  this->_CS_m_buf.close();
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_FSTREAM_CC__ */
