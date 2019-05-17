// -*-C++-*- cxxrt/sysfile.h
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
// Title:   Declaration of a class for system dependent file access
// Version: $Id: sysfile.h,v 1.1.1.1 2002/06/05 01:01:50 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_SYSFILE_H__)
#define _CXXRT_SYSFILE_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

class _CS_sysfile
{
public:
  _CS_sysfile();
  _CS_sysfile(char const*, ios_base::openmode);
  ~_CS_sysfile();

  bool         _CS_open(char const*, ios_base::openmode);
  bool         _CS_close();
  bool         _CS_is_open() const;

  streamsize   _CS_bufsiz() const;
  streamsize   _CS_read(char*, streamsize);
  streamsize   _CS_write(char const*, streamsize);
  _CXXRT_off_t _CS_seek(_CXXRT_off_t, ios_base::seekdir);

#if !defined(_MSC_VER)
  template <class _CS_second_cT>
  streamsize _CS_read(_CS_second_cT*, streamsize) { throw_failure("can only read 'char'"); return 0; }
  template <class _CS_second_cT>
  streamsize _CS_write(_CS_second_cT const*, streamsize) { throw_failure("can only write 'char'"); return 0; }
#endif

private:
  void throw_failure(char const*);
  ios_base::openmode  _CS_m_mode;
  int                 _CS_m_fd;
  streamsize          _CS_m_bufsiz;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_SYSFILE_H__ */
