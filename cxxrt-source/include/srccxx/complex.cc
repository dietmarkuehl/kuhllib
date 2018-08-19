// -*-C++-*- srccxx/complex.cc
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
// Title:   Definition of some complex functions
// Version: $Id: complex.cc,v 1.1.1.1 2002/06/05 01:02:27 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_COMPLEX_CC__)
#define __SRCCXX_COMPLEX_CC__ 1

#if !defined(_CXXRT_COMPLEX_H__)
#  include <cxxrt/complex.h>
#endif
#if !defined(_CXXRT_SSTREAM_H__)
#  include <cxxrt/sstream.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

template <class _CS_T, class _CS_cT, class _CS_traits>
inline basic_ostream<_CS_cT, _CS_traits>&
_CS_print_complex(basic_ostream<_CS_cT, _CS_traits>& _CS_os, complex<_CS_T> const& _CS_c)
{
  //-dk:TODO improve performance:
  // - construct only one sentry
  // - use stream buffer directly
  // - use exception mask

  basic_ostringstream<_CS_cT, _CS_traits> _CS_tmp;
  _CS_tmp.flags(_CS_os.flags());
  _CS_tmp.imbue(_CS_os.getloc());
  _CS_tmp.precision(_CS_os.precision());
  _CS_tmp << '(' << _CS_c.real() << "," << _CS_c.imag() << ')';
  return _CS_os << _CS_tmp.str();
}

template <class _CS_T, class _CS_cT, class _CS_traits>
inline basic_istream<_CS_cT, _CS_traits>&
_CS_read_complex(basic_istream<_CS_cT, _CS_traits>& _CS_is, complex<_CS_T>& _CS_c)
{
  _CS_is >> ws;
  typename basic_istream<_CS_cT, _CS_traits>::int_type _CS_peek = _CS_is.rdbuf()->sgetc();
  if (_CS_traits::eq_int_type(_CS_traits::eof(), _CS_peek))
    {
      _CS_is.setstate(_CS_is.rdstate() | ios_base::failbit | ios_base::eofbit);
      return _CS_is;
    }

  if (_CS_traits::eq_int_type(_CS_is.widen('('), _CS_peek))
    {
      _CS_T _CS_real = _CS_T();
      _CS_T _CS_imag = _CS_T();

      _CS_is.ignore();
      _CS_is >> ws >> _CS_real >> ws;

      _CS_peek = _CS_is.rdbuf()->sgetc();
      if (_CS_traits::eq_int_type(_CS_traits::eof(), _CS_peek))
        {
          _CS_is.setstate(_CS_is.rdstate() | ios_base::failbit | ios_base::eofbit);
          return _CS_is;
        }
      
      if (_CS_traits::eq_int_type(_CS_is.widen(','), _CS_peek))
        {
          _CS_is.ignore();
          _CS_is >> ws >> _CS_imag >> ws;
        }
      
      _CS_peek = _CS_is.rdbuf()->sgetc();
      if (_CS_traits::eq_int_type(_CS_traits::eof(), _CS_peek))
        {
          _CS_is.setstate(_CS_is.rdstate() | ios_base::failbit | ios_base::eofbit);
          return _CS_is;
        }
      
      if (_CS_traits::eq_int_type(_CS_is.widen(')'), _CS_peek))
        {
          _CS_is.ignore();
          if (!_CS_is.fail())
            _CS_c = complex<_CS_T>(_CS_real, _CS_imag);
        }
      else
        _CS_is.setstate(_CS_is.rdstate() | ios_base::failbit);
    }
  else
    {
      _CS_T _CS_val;
      _CS_is >> _CS_val;
      if (!_CS_is.fail())
        _CS_c = _CS_val;
    }

  return _CS_is;
}

// -------------------------------------------------------------------------- 

template <class _CS_T, class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>& _CS_os, complex<_CS_T> const& _CS_c)
{
  return _CS_print_complex(_CS_os, _CS_c);
}

template <class _CS_T, class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
operator>> (basic_istream<_CS_cT, _CS_traits>& _CS_is, complex<_CS_T>& _CS_c)
{
  return _CS_read_complex(_CS_is, _CS_c);
}

// -------------------------------------------------------------------------- 

extern basic_ostream<char, char_traits<char> >& operator>> (basic_ostream<char, char_traits<char> >&, complex<float> const&);
extern basic_ostream<char, char_traits<char> >& operator>> (basic_ostream<char, char_traits<char> >&, complex<double> const&);
extern basic_ostream<char, char_traits<char> >& operator>> (basic_ostream<char, char_traits<char> >&, complex<long double> const&);
extern basic_ostream<wchar_t, char_traits<wchar_t> >& operator>> (basic_ostream<wchar_t, char_traits<wchar_t> >&, complex<float> const&);
extern basic_ostream<wchar_t, char_traits<wchar_t> >& operator>> (basic_ostream<wchar_t, char_traits<wchar_t> >&, complex<double> const&);
extern basic_ostream<wchar_t, char_traits<wchar_t> >& operator>> (basic_ostream<wchar_t, char_traits<wchar_t> >&, complex<long double>const&);

extern basic_istream<char, char_traits<char> >& operator>> (basic_istream<char, char_traits<char> >&, complex<float>&);
extern basic_istream<char, char_traits<char> >& operator>> (basic_istream<char, char_traits<char> >&, complex<double>&);
extern basic_istream<char, char_traits<char> >& operator>> (basic_istream<char, char_traits<char> >&, complex<long double>&);
extern basic_istream<wchar_t, char_traits<wchar_t> >& operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, complex<float>&);
extern basic_istream<wchar_t, char_traits<wchar_t> >& operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, complex<double>&);
extern basic_istream<wchar_t, char_traits<wchar_t> >& operator>> (basic_istream<wchar_t, char_traits<wchar_t> >&, complex<long double>&);

// -------------------------------------------------------------------------- 

template <class _CS_T>
struct _CS_complex_consts
{
  static _CS_T const _CS_1;
  static _CS_T const _CS_2;
  static _CS_T const _CS_10;
  static _CS_T const _CS_inv_log10;
};

template <class _CS_T>
inline complex<_CS_T>
_CS_sin(complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(sin(_CS_c.real()) * cosh(_CS_c.imag()),
                        cos(_CS_c.real()) * sinh(_CS_c.imag()));
}

template <class _CS_T>
inline complex<_CS_T>
_CS_cos(complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(cos(_CS_c.real()) * cosh(_CS_c.imag()),
                        -sin(_CS_c.real()) * sinh(_CS_c.imag()));
}

template <class _CS_T>
inline complex<_CS_T>
_CS_tan(complex<_CS_T> const& _CS_c)
{
  _CS_T _CS_r2 = _CS_complex_consts<_CS_T>::_CS_2 * _CS_c.real();
  _CS_T _CS_i2 = _CS_complex_consts<_CS_T>::_CS_2 * _CS_c.imag();
  _CS_T _CS_denom = cos(_CS_r2) + cosh(_CS_i2);
  return complex<_CS_T>(sin(_CS_r2) / _CS_denom, sinh(_CS_i2) / _CS_denom);
}

template <class _CS_T>
inline complex<_CS_T>
_CS_sinh(complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(sinh(_CS_c.real()) * cos(_CS_c.imag()),
                        cosh(_CS_c.real()) * sin(_CS_c.imag()));
}

template <class _CS_T>
inline complex<_CS_T>
_CS_cosh(complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(cosh(_CS_c.real()) * cos(_CS_c.imag()),
                        sinh(_CS_c.real()) * sin(_CS_c.imag()));
}

template <class _CS_T>
inline complex<_CS_T>
_CS_tanh(complex<_CS_T> const& _CS_c)
{
  _CS_T _CS_r2 = _CS_complex_consts<_CS_T>::_CS_2 * _CS_c.real();
  _CS_T _CS_i2 = _CS_complex_consts<_CS_T>::_CS_2 * _CS_c.imag();
  _CS_T _CS_denom = cosh(_CS_r2) + cos(_CS_i2);
  return complex<_CS_T>(sinh(_CS_r2) / _CS_denom, sin(_CS_i2) / _CS_denom);
}

template <class _CS_T>
inline complex<_CS_T>
_CS_sqrt(complex<_CS_T> const& _CS_c)
{
  throw "sorry, not yet implemented\n";
}

template <class _CS_T>
inline complex<_CS_T>
_CS_exp(complex<_CS_T> const& _CS_c)
{
  _CS_T _CS_e = exp(_CS_c.real());
  return complex<_CS_T>(_CS_e * cos(_CS_c.imag()), _CS_e * sin(_CS_c.imag()));
}

template <class _CS_T>
inline complex<_CS_T>
_CS_log(complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(log(abs(_CS_c)),
                        atan2(_CS_c.imag(), _CS_c.real()));
}

template <class _CS_T>
inline complex<_CS_T>
_CS_log10(complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(log10(abs(_CS_c)),
                        _CS_complex_consts<_CS_T>::_CS_inv_log10 * atan2(_CS_c.imag(), _CS_c.real()));
}

template <class _CS_T>
inline complex<_CS_T>
_CS_pow(_CS_T const&, complex<_CS_T> const&)
{
  throw "sorry, not yet implemented\n";
}

template <class _CS_T>
inline complex<_CS_T>
_CS_pow(complex<_CS_T> const&, int)
{
  throw "sorry, not yet implemented\n";
}

template <class _CS_T>
inline complex<_CS_T>
_CS_pow(complex<_CS_T> const&, _CS_T)
{
  throw "sorry, not yet implemented\n";
}

template <class _CS_T>
inline complex<_CS_T>
_CS_pow(complex<_CS_T> const&, complex<_CS_T> const&)
{
  throw "sorry, not yet implemented\n";
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_COMPLEX_CC__ */
