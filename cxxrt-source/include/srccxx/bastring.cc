// -*-C++-*- srccxx/bastring.cc
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
// Title:   A partial implementation of basic_string
// Version: $Id: bastring.cc,v 1.1.1.1 2002/06/05 01:02:23 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_BASTRING_CC__)
#define __SRCCXX_BASTRING_CC__ 1

#if 0
#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_BASTRING_H__)
#  include <cxxrt/bastring.h>
#endif
#if !defined(_CXXRT_ISTREAM_H__)
#  include <cxxrt/istream.h>
#endif
#if !defined(_CXXRT_OSTREAM_H__)
#  include <cxxrt/ostream.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif
#include <cassert>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::
    basic_string(_CS_alloc const&):
  _CS_str(0),
  _CS_size(0),
  _CS_capacity(0)
{
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::
    basic_string(basic_string const& _CS_c):
  _CS_str(new _CS_cT[_CS_c._CS_capacity]),
  _CS_size(_CS_c._CS_size),
  _CS_capacity(_CS_c._CS_capacity)
{
  _CS_traits::copy(_CS_str, _CS_c.begin(), _CS_c.end() - _CS_c.begin());
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::
    basic_string(_CS_cT const*_CS_s, _CS_alloc const&):
  _CS_str(0),
  _CS_size(0),
  _CS_capacity(0)
{
  _CS_size = _CS_traits::length(_CS_s);
  _CS_capacity = _CS_size;
  _CS_str = new _CS_cT[_CS_size];
  _CS_traits::copy(_CS_str, _CS_s, _CS_size);
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::
    basic_string(_CS_cT const* _CS_s, size_type _CS_n, _CS_alloc const&):
  _CS_str(new _CS_cT[_CS_n + 1]),
  _CS_size(_CS_n),
  _CS_capacity(_CS_n)
{
  _CS_traits::copy(_CS_str, _CS_s, _CS_n);
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::~basic_string()
{
  delete[] _CS_str;
}
  
// -------------------------------------------------------------------------- 

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>&
basic_string<_CS_cT, _CS_traits, _CS_alloc>::operator= (basic_string const& _CS_s)
{
  if (this != &_CS_s)
    {
      if (_CS_s._CS_size != 0)
        {
          _CS_cT* _CS_tmp = new _CS_cT[_CS_s._CS_capacity + 1];
          _CS_traits::copy(_CS_tmp, _CS_s._CS_str, _CS_s._CS_size);
          delete[] _CS_str;
          _CS_str = _CS_tmp;
        }
      else
        {
          delete[] _CS_str;
          _CS_str = 0;
        }
      _CS_capacity = _CS_s._CS_capacity;
      _CS_size = _CS_s._CS_size;
    }
  return *this;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::const_reference
basic_string<_CS_cT, _CS_traits, _CS_alloc>::operator[](size_type _CS_idx) const
{
  return _CS_str[_CS_idx];
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::reference
basic_string<_CS_cT, _CS_traits, _CS_alloc>::operator[](size_type _CS_idx)
{
  return _CS_str[_CS_idx];
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::iterator
basic_string<_CS_cT, _CS_traits, _CS_alloc>::begin()
{
  return _CS_str;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::const_iterator
basic_string<_CS_cT, _CS_traits, _CS_alloc>::begin() const
{
  return _CS_str;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::iterator
basic_string<_CS_cT, _CS_traits, _CS_alloc>::end()
{
  return _CS_str + _CS_size;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::const_iterator
basic_string<_CS_cT, _CS_traits, _CS_alloc>::end() const
{
  return _CS_str + _CS_size;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::size_type
basic_string<_CS_cT, _CS_traits, _CS_alloc>::size() const
{
  return _CS_size;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>::size_type
basic_string<_CS_cT, _CS_traits, _CS_alloc>::capacity() const
{
  return _CS_capacity;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
void
basic_string<_CS_cT, _CS_traits, _CS_alloc>::reserve(size_type _CS_n)
{
  _CS_capacity *= 2 * (_CS_capacity + 1);
  if (_CS_capacity < _CS_n + 1)
    _CS_capacity = _CS_n + 1;
  _CS_cT* _CS_ns = new _CS_cT[_CS_capacity];
  _CS_traits::copy(_CS_ns, _CS_str, _CS_size);
  delete[] _CS_str;
  _CS_str = _CS_ns;
  _CS_capacity -= 1;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
_CS_cT const*
basic_string<_CS_cT, _CS_traits, _CS_alloc>::data() const
{
  return _CS_str;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
_CS_cT const*
basic_string<_CS_cT, _CS_traits, _CS_alloc>::c_str() const
{
  if (_CS_str == 0)
    {
      static _CS_cT const _CS_rc[] = { 0 };
      return _CS_rc;
    }
  _CS_str[_CS_size] = 0;
  return _CS_str;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>&
basic_string<_CS_cT, _CS_traits, _CS_alloc>::append(_CS_cT const* _CS_s,
                                                                      size_type _CS_n)
{
  if (_CS_size + _CS_n >= _CS_capacity)
    reserve(_CS_size + _CS_n);
  _CS_traits::copy(_CS_str + _CS_size, _CS_s, _CS_n);
  _CS_size += _CS_n;
  return *this;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>&
basic_string<_CS_cT, _CS_traits, _CS_alloc>::append(
  basic_string<_CS_cT, _CS_traits, _CS_alloc> const& _CS_s)
{
  if (_CS_size + _CS_s.size() >= _CS_capacity)
    reserve(_CS_size + _CS_s.size());
  _CS_traits::copy(_CS_str + _CS_size, _CS_s._CS_str, _CS_s.size());
  _CS_size += _CS_s.size();
  return *this;
}

template<class _CS_cT, class _CS_traits, class _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>&
basic_string<_CS_cT, _CS_traits, _CS_alloc>::append(size_type _CS_n,
                                                                      _CS_cT _CS_c)
{
  if (_CS_size + _CS_n >= _CS_capacity)
    reserve(_CS_size + _CS_n);
  _CS_traits::assign(_CS_str + _CS_size, _CS_n, _CS_c);
  _CS_size += _CS_n;
  return *this;
}

// --------------------------------------------------------------------------

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
bool operator==(const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_lhs,
                const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_rhs)
{
  if (_CS_lhs.size() != _CS_rhs.size())
    return false;
  return !_CS_traits::compare(_CS_lhs.data(), _CS_rhs.data(), _CS_lhs.size());
}

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
bool operator==(const _CS_cT* _CS_lhs,
                const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_rhs)
{
  return basic_string<_CS_cT, _CS_traits, _CS_alloc>(_CS_lhs) == _CS_rhs;
}

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
bool operator==(const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_lhs,
                const _CS_cT* _CS_rhs)
{
  return _CS_lhs == basic_string<_CS_cT, _CS_traits, _CS_alloc>(_CS_rhs);
}

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
bool operator!=(const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_lhs,
                const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_rhs)
{
  if (_CS_lhs.size() != _CS_rhs.size())
    return true;
  return _CS_traits::compare(_CS_lhs.data(), _CS_rhs.data(), _CS_lhs.size());
}

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
bool operator!=(const _CS_cT* _CS_lhs,
                const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_rhs)
{
  return basic_string<_CS_cT, _CS_traits, _CS_alloc>(_CS_lhs) != _CS_rhs;
}

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
bool operator!=(const basic_string<_CS_cT, _CS_traits, _CS_alloc>& _CS_lhs,
                const _CS_cT* _CS_rhs)
{
  return _CS_lhs != basic_string<_CS_cT, _CS_traits, _CS_alloc>(_CS_rhs);
}

// --------------------------------------------------------------------------

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>&
basic_string<_CS_cT, _CS_traits, _CS_alloc>::
  operator+=(basic_string<_CS_cT, _CS_traits, _CS_alloc> const& _CS_rhs)
{
  append(_CS_rhs);
  return *this;
}

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>
operator+(_CS_cT const* _CS_lhs,
          basic_string<_CS_cT, _CS_traits, _CS_alloc> const& _CS_rhs)
{
  basic_string<_CS_cT, _CS_traits, _CS_alloc> _CS_rc(_CS_lhs);
  return _CS_rc += _CS_rhs;
}

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>
operator+(basic_string<_CS_cT, _CS_traits, _CS_alloc> const& _CS_lhs,
          _CS_cT const* _CS_rhs)
{
  basic_string<_CS_cT, _CS_traits, _CS_alloc> _CS_rc(_CS_lhs);
  return _CS_rc += _CS_rhs;
}

// --------------------------------------------------------------------------

template<class _CS_cT, class _CS_traits, class  _CS_alloc>
basic_string<_CS_cT, _CS_traits, _CS_alloc>&
basic_string<_CS_cT, _CS_traits, _CS_alloc>::erase(
  basic_string<_CS_cT, _CS_traits, _CS_alloc>::size_type,
  basic_string<_CS_cT, _CS_traits, _CS_alloc>::size_type)
{
  _CS_size = 0;
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_Tr, class _CS_alloc>
basic_istream<_CS_cT, _CS_Tr> &
getline(basic_istream<_CS_cT, _CS_Tr> &_CS_in,
        basic_string<_CS_cT, _CS_Tr, _CS_alloc> &_CS_str,
        _CS_cT _CS_d)
{
  typename basic_istream<_CS_cT, _CS_Tr>::sentry _CS_kerberos(_CS_in, true);
  if (_CS_kerberos)
    try
      {
        _CS_str.erase();
        basic_streambuf<_CS_cT, _CS_Tr>* _CS_sb(_CS_in.rdbuf());

        if (_CS_Tr::eq_int_type(_CS_sb->sgetc(), _CS_Tr::eof()))
          _CS_in.setstate(ios_base::failbit | ios_base::eofbit);
        else
          {
            do
              {
                _CS_cT* _CS_b = _CS_sb->_CS_begin();
                int _CS_max = _CS_sb->_CS_end() - _CS_b;
                
                if (_CS_max == 0)
                  {
                    assert("oops" == 0);
                    typename _CS_Tr::int_type _CS_i = _CS_sb->_CS_underflow();
                    if (_CS_Tr::eq_int_type(_CS_i, _CS_Tr::eof()))
                      {
                        _CS_in.setstate(ios_base::eofbit);
                        return _CS_in;
                      }
                    else
                      {
                        typename _CS_Tr::char_type _CS_c= _CS_Tr::to_char_type(_CS_i);
                        if (_CS_Tr::eq(_CS_c, _CS_d))
                          return _CS_in;
                        else
                          {
                            _CS_str.append(1, _CS_c);
                            _CS_sb->sbumpc();
                          }
                      }
                  }
                else
                  {
                    _CS_cT const* _CS_f =
                      _CS_Tr::find(_CS_b, _CS_max, _CS_d);
                    _CS_str.append(_CS_b, _CS_f - _CS_b);
                    
                    if (_CS_f - _CS_b < _CS_max)
                      {
                        _CS_sb->_CS_commit(const_cast<_CS_cT*>(_CS_f + 1));
                        return _CS_in;
                      }
                    else
                      _CS_sb->_CS_commit(const_cast<_CS_cT*>(_CS_f));
                  }
              }
            while (!_CS_Tr::eq_int_type(_CS_sb->_CS_underflow(), _CS_Tr::eof()));
            _CS_in.setstate(ios_base::eofbit);
          }
      }
    catch (...)
      {
        _CS_in._CS_process_exception();
      }
  else
    _CS_in.setstate(ios_base::failbit);

  return _CS_in;
}

template <class _CS_charT, class _CS_traits, class _CS_alloc>
basic_istream<_CS_charT, _CS_traits>&
getline(basic_istream<_CS_charT, _CS_traits>& _CS_in,
        basic_string<_CS_charT, _CS_traits, _CS_alloc>& _CS_str)
{
  return getline(_CS_in, _CS_str, _CS_in._CS_newline());
}

// --------------------------------------------------------------------------

template <class _CS_charT, class _CS_traits, class _CS_alloc>
basic_ostream<_CS_charT, _CS_traits>&
operator<< (basic_ostream<_CS_charT, _CS_traits>& _CS_out,
            basic_string<_CS_charT, _CS_traits, _CS_alloc> const& _CS_str)
{
  basic_ostream<_CS_charT, _CS_traits>::sentry _CS_kerberos(_CS_out);

  if (_CS_kerberos)
    _CS_out.rdbuf()->sputn(_CS_str.data(), _CS_str.size());

  return _CS_out;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
#else
#include <srccxx/string.cc>
#endif

#endif /* __SRCCXX_BASTRING_CC__ */
