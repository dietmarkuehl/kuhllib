// -*-C++-*- src/sysfile.cc
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
// Title:  Implementation of the system dependent abstract file access
// Version: $Id: sysfile.cpp,v 1.1.1.1 2002/06/05 01:00:40 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include "cxxrt/fstream.h"

#if !defined(_MSC_VER)
//-dk:TODO port for MSVC++

#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <iostream>
#include <cxxrt/string.h>


#if !defined(_CXXRT_STANDALONE)
extern "C" int _Cxxrt_write(int fd, char const* data, int size)
{
  return write(fd, data, size);
}
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

_CS_sysfile::_CS_sysfile():
  _CS_m_mode(ios_base::openmode()),
  _CS_m_fd(-1),
  _CS_m_bufsiz(0)
{
}

_CS_sysfile::~_CS_sysfile()
{
  if (_CS_m_fd != -1)
    ::close(_CS_m_fd);
}

void
_CS_sysfile::throw_failure(char const* msg)
{
  throw ios_base::failure(msg);
}

// -------------------------------------------------------------------------- 

bool _CS_sysfile::_CS_is_open() const
{
  return _CS_m_fd != -1;
}

bool _CS_sysfile::_CS_open(char const *fname, ios_base::openmode mode)
{
  if (_CS_m_fd != -1)
    return false;

  int flags = ((mode & ios_base::in)? ((mode & ios_base::out)? O_RDWR: O_RDONLY)
	       : ((mode & ios_base::out)? O_CREAT | O_WRONLY: 0));

  if (mode & ios_base::trunc)
    flags |= O_TRUNC | O_CREAT;
  if (mode & ios_base::app)
    flags |= O_APPEND;

  _CS_m_fd = ::open(fname, flags, 0777); //-dk:TODO S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (_CS_m_fd != -1)
    {
      if (mode & ios_base::ate)
        ::lseek(_CS_m_fd, 0, SEEK_END);

      struct stat st;
      ::fstat(_CS_m_fd, &st);
      if (S_ISDIR(st.st_mode))
      {
	::close(_CS_m_fd);
	_CS_m_fd = -1;
      }
      _CS_m_bufsiz = 2 * st.st_blksize;
    }

  _CS_m_mode = mode;
  return _CS_is_open();
}

bool _CS_sysfile::_CS_close()
{
  if (_CS_is_open())
    {
      ::close(_CS_m_fd);
      _CS_m_fd = -1;
    }

  return true;
}

// -------------------------------------------------------------------------- 

streamsize _CS_sysfile::_CS_read(char *buf, streamsize n)
{
  typedef streamsize ssize_t;
  if (_CS_m_mode & ios_base::in == 0)
    return ssize_t(-1);
  return ::read(_CS_m_fd, buf, n);
}

streamsize _CS_sysfile::_CS_write(char const *buf, streamsize n)
{
  typedef streamsize ssize_t;
  if (_CS_m_mode & ios_base::out == 0)
    return ssize_t(-1);
  return ::write(_CS_m_fd, buf, n);
}

_CXXRT_off_t _CS_sysfile::_CS_seek(_CXXRT_off_t off, ios_base::seekdir dir)
{
  switch (dir)
    {
    case ios_base::beg: return lseek(_CS_m_fd, off, SEEK_SET);
    case ios_base::cur: return lseek(_CS_m_fd, off, SEEK_CUR);
    case ios_base::end: return lseek(_CS_m_fd, off, SEEK_END);
    default: return -1;
    }
}

// -------------------------------------------------------------------------- 

streamsize _CS_sysfile::_CS_bufsiz() const
{
  return _CS_m_bufsiz;
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END

#endif
