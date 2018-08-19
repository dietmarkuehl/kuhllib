// -*-C++-*- cfilebuf.cc
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
// Title:   Definition of the filebuf functions for character type char
// Version: $Id: cfilebuf.cpp,v 1.1.1.1 2002/06/05 01:00:21 dietmar_kuehl Exp $
// --------------------------------------------------------------------------
// filebufs for only reading or writing are relatively simple. It gets
// more complicated if the same filebuf is used for both reading and
// writing.  filebuf maintains a common read and write position. If,
// at the same time, the buffer provided by streambuf is to be used,
// this gets a little tricky, at least when using one filebuf for
// alternating reading and writing.  It is, however, highly desirable
// to use streambuf's buffer to support the streambuf iterators.

// The method used by this implementation is relatively simple: There
// is just one buffer used for both reading and writing. At any time,
// only a get or a put area is set up for the streambuf. Thus,
// consecutive writes or consecutive reads should be fast. If a read
// is followed by a write, the buffer is set up for writing, the get
// area is reduced to a size of zero, and a put area is set up (at
// least if buffered writing is done; things are a little different,
// if writing is unbuffered due to a setbuf(0, 0) call).

// The file operation mode currently not used uses a correponding
// empty buffer. The position in the buffer to be restored when
// switching modes is computed using the settings of the other buffer
// plus a pointer to the beginning of the buffer.

// When switching from write mode to read mode, the characters written
// so far sent to the file. Thus, there is actually nothing to be
// saved for the write mode: When switching to write mode, the buffer
// between the current read position and the end of the buffer is made
// the new put area until overflow is reached. At this point the whole
// read buffer is exhausted and switching to read mode results in
// reading new characters from the file.

// If the mode is changed from read mode to write it is possible that
// there are still characters in the get area which can be read. If
// there are fewer characters written than there are characters in the
// get area, the characters already read can be reused. To reflect
// this, only the current read pointer (gptr()) is adjusted to be at
// the end of the buffer when switching to write mode. This forces a
// call to underflow() or uflow() when the next character is read. In
// this case, the current read position is set to the current write
// position unless there was an intermediate call to overflow() (which
// was triggered because the buffer was full): In this case, the
// beginning of the get area (eback()) is set to be equal to the end
// of the buffer (egptr()). This indicates that reading new characters
// is necessary. A corresponding situation is produced at the
// beginning, before reading or writing a character, and after
// modifying the read or write position with one of the seek functions
// (unless the seek stays within the buffer).

// --------------------------------------------------------------------------

#if !defined(_MSC_VER)
//-dk:TODO port for MSVC++

#include <srccxx/filebuf.cc>
#include <srccxx/fstream.cc>
#include <srccxx/cvtbuf.cc>
#include <srccxx/convert.cc>

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

template class _CS_basic_fstream_pbase<char, char_traits<char> >;
template class basic_filebuf<char, char_traits<char> >;
template class _CS_cvtbuf<char, char_traits<char>, _CS_sysfile>;

template class _CS_converter<char, char_traits<char>, _CS_sysfile>;
template class _CS_cvt_unbuffered<char, char_traits<char>, _CS_sysfile>;
template class _CS_cvt_noconv<char, char_traits<char>, _CS_sysfile>;

_CXXRT_NAMESPACE_END
#endif
