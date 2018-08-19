// -*-C++-*- cxxrt/in_it.h
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
// Title:   A class used for faster input...
// Version: $Id: in_it.h,v 1.1.1.1 2002/06/05 01:02:05 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_IN_IT_H__)
#define _CXXRT_IN_IT_H__ 1

#if !defined(_CXXRT_SBIT_FWD_H__)
#  include <cxxrt/sbit_fwd.h>
#endif
#include <iterator>
#include <streambuf>

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------
// The class 'in_it' is used internally to read characters from an
// iterator. The general case does not make a big difference but the
// special cases can reduce the number of checks made in the inner
// loop significantly. This is especially true for
// 'istreambuf_iterator's.

template <class _CS_cT, class _CS_It, class _CS_tag>
class _CS_in_it
{
public:
  typedef _CS_cT char_type;

  _CS_in_it(_CS_It _CS_b, _CS_It _CS_e, int _CS_c):
    _CS_begin(_CS_b),
    _CS_end(_CS_e),
    _CS_count(_CS_c)
    {
    }

  bool _CS_next_char(_CS_cT& _CS_c)
    {
      if (_CS_begin == _CS_end)
        return false;
      _CS_c = *_CS_begin;
      ++_CS_begin;
      return true;
    }
  bool _CS_next_counted_char(_CS_cT& _CS_c)
    {
      if (_CS_begin == _CS_end || _CS_count == 0)
        return false;
      _CS_c = *_CS_begin;
      ++_CS_begin;
      --_CS_count;
      return true;
    }

private:
  _CS_in_it(_CS_in_it const&); // deliberately not accessible
  void operator=(_CS_in_it const&); // deliberately not accessible

  _CS_It _CS_begin;
  _CS_It _CS_end;
  int          _CS_count;
};

// --------------------------------------------------------------------------
// for random access iterators maintainance of the count can be
// avoided. This not a big deal but still an improvement.

template <class _CS_cT, class _CS_It>
class _CS_in_it<_CS_cT, _CS_It, random_access_iterator_tag>
{
public:
  typedef _CS_cT char_type;

  _CS_in_it(_CS_It _CS_b, _CS_It _CS_e, int _CS_c):
    _CS_begin(_CS_b),
    _CS_end(_CS_e),
    _CS_stop(_CS_c < _CS_e - _CS_b? _CS_b + _CS_c: _CS_e)
    {
    }

  bool _CS_next_char(_CS_cT& _CS_c)
    {
      if (_CS_begin == _CS_end)
        return false;
      _CS_c = *_CS_begin;
      ++_CS_begin;
      return true;
    }
  bool _CS_next_counted_char(_CS_cT& _CS_c)
    {
      if (_CS_begin == _CS_stop)
        return false;
      _CS_c = *_CS_begin;
      ++_CS_begin;
      return true;
    }

private:
  _CS_in_it(_CS_in_it const&); // deliberately not accessible
  void operator=(_CS_in_it const&); // deliberately not accessible

  _CS_It _CS_begin;
  _CS_It _CS_end;
  _CS_It _CS_stop;
};

// --------------------------------------------------------------------------
// For 'istreambuf_iterator' things can be improved quite a lot,
// provided that there is a buffer. If there is no buffer, this
// specialization should still be as fast as a trivial implementation.

template <class _CS_cT, class _CS_traits, class tag>
class _CS_in_it<_CS_cT, istreambuf_iterator<_CS_cT, _CS_traits>, tag>
{
  typedef istreambuf_iterator<_CS_cT, _CS_traits> _CS_It;

public:
  typedef _CS_cT char_type;

  _CS_in_it(_CS_It _CS_b, _CS_It _CS_e, int _CS_c):
    _CS_sbuf(_CS_b._CS_sbuf()),
    _CS_stop(0),
    _CS_count(_CS_c)
    {
      if (_CS_b != _CS_e)
        init();
    }

  bool _CS_next_char(_CS_cT& _CS_c)
    {

      if (_CS_sbuf->_CS_gcur == _CS_sbuf->_CS_gend)
        {
          if (_CS_traits::eq_int_type(_CS_sbuf->underflow(), _CS_traits::eof()))
            {
              _CS_c = 0;
              return false;
            }
          else
            init();
        }

      _CS_c = *_CS_sbuf->_CS_gcur++;
      return true;
    }
  bool _CS_next_counted_char(_CS_cT& _CS_c)
    {
      if (_CS_sbuf->_CS_gcur == _CS_sbuf->_CS_gend)
        {
          if (_CS_traits::eq_int_type(_CS_sbuf->underflow(), _CS_traits::eof()))
            return false;
          else
            init();
        }
      if (_CS_sbuf->_CS_gcur == _CS_stop)
        return false;

      _CS_c = *_CS_sbuf->_CS_gcur++;
      return true;
    }
  
private:
  void init()
    {
      if (_CS_sbuf->_CS_gend - _CS_sbuf->_CS_gcur >= _CS_count)
        {
          _CS_stop = _CS_sbuf->_CS_gcur + _CS_count;
          _CS_count = 0;
        }
      else
        {
          _CS_stop = _CS_sbuf->_CS_gend;
          _CS_count -= _CS_stop - _CS_sbuf->_CS_gcur;
        }
    }

  _CS_in_it(_CS_in_it const&); // deliberately not accessible
  void operator=(_CS_in_it const&); // deliberately not accessible

  basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf;
  _CS_cT const* _CS_stop;
  int _CS_count;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_IN_IT_H__ */
