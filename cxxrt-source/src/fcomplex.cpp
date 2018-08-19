// -*-C++-*- fcomplex.cpp
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
// Title:   Instantiation of complex<float>
// Version: $Id: fcomplex.cpp,v 1.1.1.1 2002/06/05 01:00:25 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include <srccxx/complex.cc>
#include <cmath>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

void _CS_fdivide(float const& _CS_nomreal, float const& _CS_nomimag,
		 float const& _CS_denomreal, float const& _CS_denomimag,
		 float& _CS_real, float& _CS_imag)
{
  _CS_divide(_CS_nomreal, _CS_nomimag, _CS_denomreal, _CS_denomimag, _CS_real, _CS_imag);
}

// -------------------------------------------------------------------------- 

float abs(complex<float> const& _CS_c)
{
  return _CXXRT_STD sqrt(_CXXRT_STD norm(_CS_c));
}

float arg(complex<float> const& _CS_c)
{
  return _CXXRT_STD atan2(_CS_c.imag(), _CS_c.real());
}

float norm(complex<float> const& _CS_c)
{
  return _CS_c.real() * _CS_c.real() + _CS_c.imag() * _CS_c.imag();
}

complex<float> polar(float const& _CS_a, float const& _CS_phi)
{
  return complex<float>(_CS_a * _CXXRT_STD cos(_CS_phi), _CS_a * _CXXRT_STD sin(_CS_phi));
}

// -------------------------------------------------------------------------- 

basic_ostream<char, char_traits<char> >&
operator<< (basic_ostream<char, char_traits<char> >& _CS_os, complex<float> const& _CS_c)
{
  return _CS_print_complex(_CS_os, _CS_c);
}

basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >& _CS_os, complex<float> const& _CS_c)
{
  return _CS_print_complex(_CS_os, _CS_c);
}

basic_istream<char, char_traits<char> >&
operator>> (basic_istream<char, char_traits<char> >& _CS_is, complex<float>& _CS_c)
{
  return _CS_read_complex(_CS_is, _CS_c);
}

basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >& _CS_is, complex<float>& _CS_c)
{
  return _CS_read_complex(_CS_is, _CS_c);
}

// -------------------------------------------------------------------------- 

template <> float const _CS_complex_consts<float>::_CS_1 = 1.0f;
template <> float const _CS_complex_consts<float>::_CS_2 = 2.0f;
template <> float const _CS_complex_consts<float>::_CS_10 = 10.0f;
template <> float const _CS_complex_consts<float>::_CS_inv_log10 = 1.0f / log(10.0f);

template <> complex<float> sin(complex<float> const& c) { return _CS_sin(c); }
template <> complex<float> cos(complex<float> const& c) { return _CS_cos(c); }
template <> complex<float> tan(complex<float> const& c) { return _CS_tan(c); }
template <> complex<float> sinh(complex<float> const& c) { return _CS_sinh(c); }
template <> complex<float> cosh(complex<float> const& c) { return _CS_cosh(c); }
template <> complex<float> tanh(complex<float> const& c) { return _CS_tanh(c); }

template <> complex<float> sqrt(complex<float> const& c) { return _CS_sqrt(c); }
template <> complex<float> exp(complex<float> const& c) { return _CS_exp(c); }
template <> complex<float> log(complex<float> const& c) { return _CS_log(c); }
template <> complex<float> log10(complex<float> const& c) { return _CS_log10(c); }
template <> complex<float> pow(float const&f, complex<float> const& c) { return _CS_pow(f, c); }
template <> complex<float> pow(complex<float> const& c, int n) { return _CS_pow(c, n); }
template <> complex<float> pow(complex<float> const& c, float const& f) { return _CS_pow(c, f); }
template <> complex<float> pow(complex<float> const& c, complex<float> const& e) { return _CS_pow(c, e); }

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
