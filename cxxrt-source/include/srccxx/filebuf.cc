// -*-C++-*- srccxx/filebuf.cc
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
// Title:  Implementation of basic_filebuf based on cvtbuf.
// Version: $Id: filebuf.cc,v 1.2 2006/04/07 07:07:13 dietmar_kuehl Exp $

// Basically, this class just forwards everything to either the
// "external representation" or to the cvtbuf

// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_FILEBUF_CC__)
#define __SRCCXX_FILEBUF_CC__ 1

#if !defined(_CXXRT_FSTREAM_H__)
#  include <cxxrt/fstream.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits>
basic_filebuf<_CS_charT, _CS_traits>::basic_filebuf()
{
}

template <class _CS_charT, class _CS_traits>
basic_filebuf<_CS_charT, _CS_traits>::~basic_filebuf()
{
  close();
}

template <class _CS_charT, class _CS_traits>
bool
basic_filebuf<_CS_charT, _CS_traits>::is_open() const
{
  return this->_CS_m_er._CS_is_open();
}

template <class _CS_charT, class _CS_traits>
basic_filebuf<_CS_charT, _CS_traits>*
basic_filebuf<_CS_charT, _CS_traits>::open(char const* _CS_fname,
                                           ios_base::openmode _CS_mode)
{
  if (this->_CS_m_er._CS_open(_CS_fname, _CS_mode))
    {
      this->_CS_init();
      return this;
    }
  else
    return 0;
}

template <class _CS_charT, class _CS_traits>
basic_filebuf<_CS_charT, _CS_traits>*
basic_filebuf<_CS_charT, _CS_traits>::close()
{
  if (!is_open())
    return 0;

  this->sync();
  this->_CS_finish();
  return this->_CS_m_er._CS_close()? this: 0;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_FILEBUF_CC__ */
