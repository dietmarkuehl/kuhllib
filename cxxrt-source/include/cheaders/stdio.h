// -*-C++-*- stdio.h
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
// Title:   Wrapper for the C library's stdio.h
// Version: $Id: stdio.h,v 1.2 2002/06/06 01:56:54 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(__STDIO_H_CXX__)
#define __STDIO_H_CXX__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

extern "C"
{
#define _CXXRT_IN_STDIO_H
#include _CXXRT_C_STDIO_H
#undef _CXXRT_IN_STDIO_H
} // extern "C"

#if !defined(_CXXRT_WCHAR_H_PROCESSING)

#if defined(putchar)
   inline int _CS_putchar(int _CS_c) { return putchar(_CS_c); }
#  undef putchar
   extern "C" { inline int putchar(int _CS_c) { return _CS_putchar(_CS_c); } }
#endif

#if defined(printf)
#  undef printf
#endif

_CXXRT_NAMESPACE_BEGIN

  using ::FILE;
  using ::fpos_t;
//using ::size_t;

  using ::clearerr;
  using ::fclose;
  using ::feof;
  using ::ferror;
  using ::fflush;
  using ::fgetc;
  using ::fgetpos;
  using ::fgets;
  using ::fopen;
  using ::fprintf;
  using ::fputc;
  using ::fputs;
  using ::fread;
  using ::freopen;
  using ::fscanf;
  using ::fseek;
  using ::fsetpos;
  using ::ftell;
  using ::fwrite;
  using ::getc;
  using ::getchar;
  using ::gets;
  using ::perror;
  using ::printf;
  using ::putc;
  using ::putchar;
  using ::puts;
  using ::remove;
  using ::rename;
  using ::rewind;
  using ::scanf;
  using ::setbuf;
  using ::setvbuf;
  using ::sprintf;
  using ::sscanf;
  using ::tmpfile;
  using ::tmpnam;
  using ::ungetc;
  using ::vfprintf;
  using ::vprintf;
  using ::vsprintf;

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#else //_CXXRT_WCHAR_H_PROCESING
#  undef __STDIO_H_CXX__
#endif // _CXXRT_WCHAR_H_PROCESSING

#endif /* __STDIO_H_CXX__ */
