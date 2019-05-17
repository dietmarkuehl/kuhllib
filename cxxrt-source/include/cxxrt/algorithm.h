// -*-C++-*- cxxrt/algorithm.h
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
// Title:  Make this implementation independent from the <algorithm> header
// Version: $Id: algorithm.h,v 1.1.1.1 2002/06/05 01:01:48 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_ALGORITHM_H__)
#define _CXXRT_ALGORITHM_H__ 1

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_IIt, class _CS_OIt>
inline _CS_OIt
_CS_copy(_CS_IIt _CS_b, _CS_IIt _CS_e, _CS_OIt _CS_d)
{
  while (_CS_b != _CS_e)
  {
    *_CS_d = *_CS_b;
    ++_CS_d;
    ++_CS_b;
  }
  return _CS_d;
}

template <class _CS_IIt, class _CS_T>
inline _CS_IIt
_CS_find(_CS_IIt _CS_b, _CS_IIt _CS_e, _CS_T const& _CS_val)
{
  while (_CS_b != _CS_e && *_CS_b != _CS_val)
    ++_CS_b;
  return _CS_b;
}

template <class _CS_IIt, class _CS_T>
inline _CS_IIt
_CS_fill_n(_CS_IIt _CS_b, _CS_IIt _CS_e, _CS_T const& _CS_val)
{
  for (; _CS_b != _CS_e; ++_CS_b)
    *_CS_b = _CS_val;
  return _CS_b;
}

// --------------------------------------------------------------------------

template <class _CS_It, class _CS_Val>
inline _CS_It
_CS_partition(_CS_It _CS_b, _CS_It _CS_e)
{
  _CS_Val _CS_tmp;
  
  for (--_CS_e; _CS_b < _CS_e; )
  {
    while (*_CS_b < *_CS_e)
      --_CS_e;
    ;
    if (_CS_b != _CS_e)
    {
      if (*_CS_b == *_CS_e)
        ++_CS_b;
      else
      {
        _CS_tmp = *_CS_b;
        *_CS_b = * _CS_e;
        *_CS_e = _CS_tmp;
      }
    }
    
    while (*_CS_b < *_CS_e)
      ++_CS_b;
    if (_CS_b != _CS_e)
    {
      if (*_CS_b == *_CS_e)
        --_CS_e;
      else
      {
        _CS_tmp = *_CS_b;
        *_CS_b = * _CS_e;
        *_CS_e = _CS_tmp;
      }
    }
  }
  return _CS_b;
}

template <class _CS_It, class _CS_Val>
void
_CS_quicksort(_CS_It _CS_b, _CS_It _CS_e, _CS_Val const&);

template <class _CS_It>
inline void
_CS_quicksort(_CS_It _CS_b, _CS_It _CS_e)
{
  if (_CS_b != _CS_e)
    _CS_quicksort(_CS_b, _CS_e, *_CS_b);
}

template <class _CS_It, class _CS_Val>
void
_CS_quicksort(_CS_It _CS_b, _CS_It _CS_e, _CS_Val const&)
{
  if (_CS_b != _CS_e)
  {
    _CS_It _CS_mid = _CS_partition<_CS_It, _CS_Val>(_CS_b, _CS_e);
    _CS_quicksort(_CS_b, _CS_mid);
    _CS_quicksort(++_CS_mid, _CS_e);
  }
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_ALGORITHM_H__ */
