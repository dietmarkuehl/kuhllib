// -*-C++-*- dcomplex.cpp
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
// Title:   Instantiation of complex<double>
// Version: $Id: dcomplex.cpp,v 1.1.1.1 2002/06/05 01:00:19 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include <srccxx/complex.cc>
#include <cmath>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

void _CS_ddivide(double const& _CS_nomreal, double const& _CS_nomimag,
		 double const& _CS_denomreal, double const& _CS_denomimag,
		 double& _CS_real, double& _CS_imag)
{
  _CS_divide(_CS_nomreal, _CS_nomimag, _CS_denomreal, _CS_denomimag, _CS_real, _CS_imag);
}

// -------------------------------------------------------------------------- 

double abs(complex<double> const& _CS_c)
{
  return _CXXRT_STD sqrt(_CXXRT_STD norm(_CS_c));
}

double arg(complex<double> const& _CS_c)
{
  return _CXXRT_STD atan2(_CS_c.imag(), _CS_c.real());
}

double norm(complex<double> const& _CS_c)
{
  return _CS_c.real() * _CS_c.real() + _CS_c.imag() * _CS_c.imag();
}

complex<double> polar(double const& _CS_a, double const& _CS_phi)
{
  return complex<double>(_CS_a * _CXXRT_STD cos(_CS_phi), _CS_a * _CXXRT_STD sin(_CS_phi));
}

// -------------------------------------------------------------------------- 

basic_ostream<char, char_traits<char> >&
operator<< (basic_ostream<char, char_traits<char> >& _CS_os, complex<double> const& _CS_c)
{
  return _CS_print_complex(_CS_os, _CS_c);
}

basic_ostream<wchar_t, char_traits<wchar_t> >&
operator<< (basic_ostream<wchar_t, char_traits<wchar_t> >& _CS_os, complex<double> const& _CS_c)
{
  return _CS_print_complex(_CS_os, _CS_c);
}

basic_istream<char, char_traits<char> >&
operator>> (basic_istream<char, char_traits<char> >& _CS_is, complex<double>& _CS_c)
{
  return _CS_read_complex(_CS_is, _CS_c);
}

basic_istream<wchar_t, char_traits<wchar_t> >&
operator>> (basic_istream<wchar_t, char_traits<wchar_t> >& _CS_is, complex<double>& _CS_c)
{
  return _CS_read_complex(_CS_is, _CS_c);
}

// -------------------------------------------------------------------------- 

#if defined(__EDG__)
template <> double const _CS_complex_consts<double>::_CS_1 = 1.0;
template <> double const _CS_complex_consts<double>::_CS_2 = _CS_1 + _CS_1;
template <> double const _CS_complex_consts<double>::_CS_10 = _CS_2 + _CS_2 + _CS_2 + _CS_2 + _CS_2;
template <> double const _CS_complex_consts<double>::_CS_inv_log10 = _CS_1 / log(_CS_10);
#else
template <> double const _CS_complex_consts<double>::_CS_1 = 1.0;
template <> double const _CS_complex_consts<double>::_CS_2 = 2.0;
template <> double const _CS_complex_consts<double>::_CS_10 = 10.0;
template <> double const _CS_complex_consts<double>::_CS_inv_log10 = 1.0 / log(10.0);
#endif

template <> complex<double> sin(complex<double> const& c) { return _CS_sin(c); }
template <> complex<double> cos(complex<double> const& c) { return _CS_cos(c); }
template <> complex<double> tan(complex<double> const& c) { return _CS_tan(c); }
template <> complex<double> sinh(complex<double> const& c) { return _CS_sinh(c); }
template <> complex<double> cosh(complex<double> const& c) { return _CS_cosh(c); }
template <> complex<double> tanh(complex<double> const& c) { return _CS_tanh(c); }

template <> complex<double> sqrt(complex<double> const& c) { return _CS_sqrt(c); }
template <> complex<double> exp(complex<double> const& c) { return _CS_exp(c); }
template <> complex<double> log(complex<double> const& c) { return _CS_log(c); }
template <> complex<double> log10(complex<double> const& c) { return _CS_log10(c); }
template <> complex<double> pow(double const&f, complex<double> const& c) { return _CS_pow(f, c); }
template <> complex<double> pow(complex<double> const& c, int n) { return _CS_pow(c, n); }
template <> complex<double> pow(complex<double> const& c, double const& f) { return _CS_pow(c, f); }
template <> complex<double> pow(complex<double> const& c, complex<double> const& e) { return _CS_pow(c, e); }

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
