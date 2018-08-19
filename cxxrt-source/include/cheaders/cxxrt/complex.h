// -*-C++-*- cxxrt/complex.h
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
// Title:   Declaration of the complex classes
// Version: $Id: complex.h,v 1.1.1.1 2002/06/05 01:01:50 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_COMPLEX_H__)
#define _CXXRT_COMPLEX_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_IOSFWD_H__)
#  include <cxxrt/iosfwd.h>
#endif
//-dk:TODO remove when EDG can handle this correctly
#include <cmath>

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_T> _CS_T atan2(_CS_T, _CS_T);
template <class _CS_T> _CS_T cos(_CS_T);
template <class _CS_T> _CS_T sin(_CS_T);
template <class _CS_T> _CS_T cosh(_CS_T);
template <class _CS_T> _CS_T sinh(_CS_T);

// --------------------------------------------------------------------------

template <class _CS_T>
inline void
_CS_divide(_CS_T const& _CS_nomreal, _CS_T const& _CS_nomimag,
           _CS_T const& _CS_denomreal, _CS_T const& _CS_denomimag,
           _CS_T& _CS_real, _CS_T& _CS_imag)
{
  _CS_T _CS_absreal = _CS_denomreal < _CS_T()? -_CS_denomreal: _CS_denomreal;
  _CS_T _CS_absimag = _CS_denomimag < _CS_T()? -_CS_denomimag: _CS_denomimag;

  if (_CS_absreal < _CS_absimag)
    {
      _CS_T _CS_ratio = _CS_denomreal / _CS_denomimag;
      _CS_T _CS_div = _CS_denomimag + _CS_denomimag * _CS_ratio * _CS_ratio;
      _CS_real = (_CS_nomreal * _CS_ratio + _CS_nomimag) / _CS_div;
      _CS_imag = (_CS_nomimag * _CS_ratio - _CS_nomreal) / _CS_div;
    }
  else
    {
      _CS_T _CS_ratio = _CS_denomimag / _CS_denomreal;
      _CS_T _CS_div = _CS_denomreal + _CS_denomreal * _CS_ratio * _CS_ratio;
      _CS_real = (_CS_nomreal + _CS_nomimag * _CS_ratio) / _CS_div;
      _CS_imag = (_CS_nomimag - _CS_nomreal * _CS_ratio) / _CS_div;
    }
}

void _CS_fdivide(float const&, float const&, float const&, float const&, float&, float&);
void _CS_ddivide(double const&, double const&, double const&, double const&, double&, double&);
void _CS_ldivide(long double const&, long double const&, long double const&, long double const&, long double&, long double&);

// --------------------------------------------------------------------------

template <class _CS_T> class complex;

template <> class complex<float>;
template <> class complex<double>;
template <> class complex<long double>;

template <class _CS_T>
class complex
{
public:
  typedef _CS_T value_type;

  complex();
  complex(_CS_T const& _CS_r);
  complex(_CS_T const& _CS_r, _CS_T const& _CS_i);
  complex(complex<_CS_T> const& _CS_c);
  template <class _CS_S>
  complex(complex<_CS_S> const& _CS_c);

  _CS_T real() const { return _CS_m_real; }
  _CS_T imag() const { return _CS_m_imag; }

  complex& operator= (_CS_T const& _CS_v);
  complex& operator+= (_CS_T const& _CS_v);
  complex& operator-= (_CS_T const& _CS_v);
  complex& operator*= (_CS_T const& _CS_v);
  complex& operator/= (_CS_T const& _CS_v);

  complex& operator= (complex<_CS_T> const& _CS_c);
  template <class _CS_S> complex& operator= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator+= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator-= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator*= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator/= (complex<_CS_S> const& _CS_c);

private:
  _CS_T _CS_m_real;
  _CS_T _CS_m_imag;
};

// --------------------------------------------------------------------------

template <>
class complex<float>
{
public:
  typedef float value_type;

  complex();
  complex(float const& _CS_r);
  complex(float const& _CS_r, float const& _CS_i);
  explicit complex(complex<double> const& _CS_c);
  explicit complex(complex<long double> const& _CS_c);

  float real() const { return _CS_m_real; }
  float imag() const { return _CS_m_imag; }

  complex& operator= (float const& _CS_v);
  complex& operator+= (float const& _CS_v);
  complex& operator-= (float const& _CS_v);
  complex& operator*= (float const& _CS_v);
  complex& operator/= (float const& _CS_v);

  complex& operator= (complex<float> const& _CS_c);
  template <class _CS_S> complex& operator= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator+= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator-= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator*= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator/= (complex<_CS_S> const& _CS_c);

private:
  float _CS_m_real;
  float _CS_m_imag;
};

// --------------------------------------------------------------------------

template <>
class complex<double>
{
public:
  typedef double value_type;

  complex();
  complex(double const& _CS_r);
  complex(double const& _CS_r, double const& _CS_i);
  complex(complex<float> const& _CS_c);
  explicit complex(complex<long double> const& _CS_c);

  double real() const { return _CS_m_real; }
  double imag() const { return _CS_m_imag; }

  complex& operator= (double const& _CS_v);
  complex& operator+= (double const& _CS_v);
  complex& operator-= (double const& _CS_v);
  complex& operator*= (double const& _CS_v);
  complex& operator/= (double const& _CS_v);

  complex& operator= (complex<double> const& _CS_c);
  template <class _CS_S> complex& operator= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator+= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator-= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator*= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator/= (complex<_CS_S> const& _CS_c);

private:
  double _CS_m_real;
  double _CS_m_imag;
};

// --------------------------------------------------------------------------

template <>
class complex<long double>
{
public:
  typedef long double value_type;

  complex();
  complex(long double const& _CS_r);
  complex(long double const& _CS_r, long double const& _CS_i);
  complex(complex<float> const& _CS_c);
  complex(complex<double> const& _CS_c);

  long double real() const { return _CS_m_real; }
  long double imag() const { return _CS_m_imag; }

  complex& operator= (long double const& _CS_v);
  complex& operator+= (long double const& _CS_v);
  complex& operator-= (long double const& _CS_v);
  complex& operator*= (long double const& _CS_v);
  complex& operator/= (long double const& _CS_v);

  complex& operator= (complex<long double> const& _CS_c);
  template <class _CS_S> complex& operator= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator+= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator-= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator*= (complex<_CS_S> const& _CS_c);
  template <class _CS_S> complex& operator/= (complex<_CS_S> const& _CS_c);

private:
  long double _CS_m_real;
  long double _CS_m_imag;
};

// --------------------------------------------------------------------------

template <class _CS_T>
inline
complex<_CS_T>::complex():
  _CS_m_real(),
  _CS_m_imag()
{
}

template <class _CS_T>
inline
complex<_CS_T>::complex(_CS_T const& _CS_r):
  _CS_m_real(_CS_r),
  _CS_m_imag()
{
}

template <class _CS_T>
inline
complex<_CS_T>::complex(_CS_T const& _CS_r, _CS_T const& _CS_i):
  _CS_m_real(_CS_r),
  _CS_m_imag(_CS_i)
{
}

template <class _CS_T>
inline
complex<_CS_T>::complex(complex<_CS_T> const& _CS_c):
  _CS_m_real(_CS_c._CS_m_real),
  _CS_m_imag(_CS_c._CS_m_imag)
{
}

template <class _CS_T>
  template <class _CS_S>
inline
complex<_CS_T>::complex(complex<_CS_S> const& _CS_c):
  _CS_m_real(_CS_c.real()),
  _CS_m_imag(_CS_c.imag())
{
}

// --------------------------------------------------------------------------

template <class _CS_T>
inline complex<_CS_T>&
complex<_CS_T>::operator= (_CS_T const& _CS_v)
{
  _CS_m_real = _CS_v;
  _CS_m_imag = _CS_T();
  return *this;
}

template <class _CS_T>
inline complex<_CS_T>&
complex<_CS_T>::operator+= (_CS_T const& _CS_v)
{
  _CS_m_real += _CS_v;
  return *this;
}

template <class _CS_T>
inline complex<_CS_T>&
complex<_CS_T>::operator-= (_CS_T const& _CS_v)
{
  _CS_m_real -= _CS_v;
  return *this;
}

template <class _CS_T>
inline complex<_CS_T>&
complex<_CS_T>::operator*= (_CS_T const& _CS_v)
{
  _CS_m_real *= _CS_v;
  _CS_m_imag *= _CS_v;
  return *this;
}

template <class _CS_T>
inline complex<_CS_T>&
complex<_CS_T>::operator/= (_CS_T const& _CS_v)
{
  _CS_m_real /= _CS_v;
  _CS_m_imag /= _CS_v;
  return *this;
}

template <class _CS_T>
inline complex<_CS_T>&
complex<_CS_T>::operator= (complex<_CS_T> const& _CS_c)
{
  _CS_m_real = _CS_c._CS_m_real;
  _CS_m_imag = _CS_c._CS_m_imag;
  return *this;
}

template <class _CS_T>
  template <class _CS_S>
inline complex<_CS_T>&
complex<_CS_T>::operator= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real = _CS_T(_CS_c.real());
  _CS_m_imag = _CS_T(_CS_c.imag());
  return *this;
}

template <class _CS_T>
  template <class _CS_S>
inline complex<_CS_T>&
complex<_CS_T>::operator+= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real += _CS_T(_CS_c.real());
  _CS_m_imag += _CS_T(_CS_c.imag());
  return *this;
}

template <class _CS_T>
  template <class _CS_S>
inline complex<_CS_T>&
complex<_CS_T>::operator-= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real -= _CS_T(_CS_c.real());
  _CS_m_imag -= _CS_T(_CS_c.imag());
  return *this;
}

template <class _CS_T>
  template <class _CS_S>
inline complex<_CS_T>&
complex<_CS_T>::operator*= (complex<_CS_S> const& _CS_c)
{
  _CS_T _CS_tmp = _CS_m_real * _CS_T(_CS_c.real()) - _CS_m_imag * _CS_T(_CS_c.imag());
  _CS_m_imag = _CS_m_real * _CS_T(_CS_c.imag()) + _CS_m_imag * _CS_T(_CS_c.real());
  _CS_m_real = _CS_tmp;
  return *this;
}

template <class _CS_T>
  template <class _CS_S>
inline complex<_CS_T>&
complex<_CS_T>::operator/= (complex<_CS_S> const& _CS_c)
{
  _CS_T _CS_real, _CS_imag;
  _CS_divide(_CS_m_real, _CS_m_imag, _CS_T(_CS_c.real()), _CS_T(_CS_c.imag()), _CS_real, _CS_imag);
  _CS_m_real = _CS_real;
  _CS_m_imag = _CS_imag;
  return *this;
}

// --------------------------------------------------------------------------

inline
complex<float>::complex():
  _CS_m_real(),
  _CS_m_imag()
{
}

inline
complex<float>::complex(float const& _CS_r):
  _CS_m_real(_CS_r),
  _CS_m_imag()
{
}

inline
complex<float>::complex(float const& _CS_r, float const& _CS_i):
  _CS_m_real(_CS_r),
  _CS_m_imag(_CS_i)
{
}

inline
complex<float>::complex(complex<double> const& _CS_c):
  _CS_m_real(_CS_c.real()),
  _CS_m_imag(_CS_c.imag())
{
}

inline
complex<float>::complex(complex<long double> const& _CS_c):
  _CS_m_real(_CS_c.real()),
  _CS_m_imag(_CS_c.imag())
{
}

// --------------------------------------------------------------------------

inline complex<float>&
complex<float>::operator= (float const& _CS_v)
{
  _CS_m_real = _CS_v;
  _CS_m_imag = float();
  return *this;
}

inline complex<float>&
complex<float>::operator+= (float const& _CS_v)
{
  _CS_m_real += _CS_v;
  return *this;
}

inline complex<float>&
complex<float>::operator-= (float const& _CS_v)
{
  _CS_m_real -= _CS_v;
  return *this;
}

inline complex<float>&
complex<float>::operator*= (float const& _CS_v)
{
  _CS_m_real *= _CS_v;
  _CS_m_imag *= _CS_v;
  return *this;
}

inline complex<float>&
complex<float>::operator/= (float const& _CS_v)
{
  _CS_m_real /= _CS_v;
  _CS_m_imag /= _CS_v;
  return *this;
}

inline complex<float>&
complex<float>::operator= (complex<float> const& _CS_c)
{
  _CS_m_real = _CS_c._CS_m_real;
  _CS_m_imag = _CS_c._CS_m_imag;
  return *this;
}

template <class _CS_S>
inline complex<float>&
complex<float>::operator= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real = _CS_c.real();
  _CS_m_imag = _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<float>&
complex<float>::operator+= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real += _CS_c.real();
  _CS_m_imag += _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<float>&
complex<float>::operator-= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real -= _CS_c.real();
  _CS_m_imag -= _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<float>&
complex<float>::operator*= (complex<_CS_S> const& _CS_c)
{
  float _CS_tmp = _CS_m_real * _CS_c.real() - _CS_m_imag * _CS_c.imag();
  _CS_m_imag = _CS_m_real * _CS_c.imag() + _CS_m_imag * _CS_c.real();
  _CS_m_real = _CS_tmp;
  return *this;
}

template <class _CS_S>
inline complex<float>&
complex<float>::operator/= (complex<_CS_S> const& _CS_c)
{
  float _CS_real, _CS_imag;
  _CS_fdivide(_CS_m_real, _CS_m_imag, _CS_c.real(), _CS_c.imag(), _CS_real, _CS_imag);
  _CS_m_real = _CS_real;
  _CS_m_imag = _CS_imag;
  return *this;
}

// --------------------------------------------------------------------------

inline
complex<double>::complex():
  _CS_m_real(),
  _CS_m_imag()
{
}

inline
complex<double>::complex(double const& _CS_r):
  _CS_m_real(_CS_r),
  _CS_m_imag()
{
}

inline
complex<double>::complex(double const& _CS_r, double const& _CS_i):
  _CS_m_real(_CS_r),
  _CS_m_imag(_CS_i)
{
}

inline
complex<double>::complex(complex<float> const& _CS_c):
  _CS_m_real(_CS_c.real()),
  _CS_m_imag(_CS_c.imag())
{
}

inline
complex<double>::complex(complex<long double> const& _CS_c):
  _CS_m_real(_CS_c.real()),
  _CS_m_imag(_CS_c.imag())
{
}

// --------------------------------------------------------------------------

inline complex<double>&
complex<double>::operator= (double const& _CS_v)
{
  _CS_m_real = _CS_v;
  _CS_m_imag = double();
  return *this;
}

inline complex<double>&
complex<double>::operator+= (double const& _CS_v)
{
  _CS_m_real += _CS_v;
  return *this;
}

inline complex<double>&
complex<double>::operator-= (double const& _CS_v)
{
  _CS_m_real -= _CS_v;
  return *this;
}

inline complex<double>&
complex<double>::operator*= (double const& _CS_v)
{
  _CS_m_real *= _CS_v;
  _CS_m_imag *= _CS_v;
  return *this;
}

inline complex<double>&
complex<double>::operator/= (double const& _CS_v)
{
  _CS_m_real /= _CS_v;
  _CS_m_imag /= _CS_v;
  return *this;
}

inline complex<double>&
complex<double>::operator= (complex<double> const& _CS_c)
{
  _CS_m_real = _CS_c._CS_m_real;
  _CS_m_imag = _CS_c._CS_m_imag;
  return *this;
}

template <class _CS_S>
inline complex<double>&
complex<double>::operator= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real = _CS_c.real();
  _CS_m_imag = _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<double>&
complex<double>::operator+= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real += _CS_c.real();
  _CS_m_imag += _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<double>&
complex<double>::operator-= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real -= _CS_c.real();
  _CS_m_imag -= _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<double>&
complex<double>::operator*= (complex<_CS_S> const& _CS_c)
{
  double _CS_tmp = _CS_m_real * _CS_c.real() - _CS_m_imag * _CS_c.imag();
  _CS_m_imag = _CS_m_real * _CS_c.imag() + _CS_m_imag * _CS_c.real();
  _CS_m_real = _CS_tmp;
  return *this;
}

template <class _CS_S>
inline complex<double>&
complex<double>::operator/= (complex<_CS_S> const& _CS_c)
{
  double _CS_real, _CS_imag;
  _CS_ddivide(_CS_m_real, _CS_m_imag, _CS_c.real(), _CS_c.imag(), _CS_real, _CS_imag);
  _CS_m_real = _CS_real;
  _CS_m_imag = _CS_imag;
  return *this;
}

// --------------------------------------------------------------------------

inline
complex<long double>::complex():
  _CS_m_real(),
  _CS_m_imag()
{
}

inline
complex<long double>::complex(long double const& _CS_r):
  _CS_m_real(_CS_r),
  _CS_m_imag()
{
}

inline
complex<long double>::complex(long double const& _CS_r, long double const& _CS_i):
  _CS_m_real(_CS_r),
  _CS_m_imag(_CS_i)
{
}

inline
complex<long double>::complex(complex<float> const& _CS_c):
  _CS_m_real(_CS_c.real()),
  _CS_m_imag(_CS_c.imag())
{
}

inline
complex<long double>::complex(complex<double> const& _CS_c):
  _CS_m_real(_CS_c.real()),
  _CS_m_imag(_CS_c.imag())
{
}

// --------------------------------------------------------------------------

inline complex<long double>&
complex<long double>::operator= (long double const& _CS_v)
{
  _CS_m_real = _CS_v;
  typedef long double _CS_T;
  _CS_m_imag = _CS_T();
  return *this;
}

inline complex<long double>&
complex<long double>::operator+= (long double const& _CS_v)
{
  _CS_m_real += _CS_v;
  return *this;
}

inline complex<long double>&
complex<long double>::operator-= (long double const& _CS_v)
{
  _CS_m_real -= _CS_v;
  return *this;
}

inline complex<long double>&
complex<long double>::operator*= (long double const& _CS_v)
{
  _CS_m_real *= _CS_v;
  _CS_m_imag *= _CS_v;
  return *this;
}

inline complex<long double>&
complex<long double>::operator/= (long double const& _CS_v)
{
  _CS_m_real /= _CS_v;
  _CS_m_imag /= _CS_v;
  return *this;
}

inline complex<long double>&
complex<long double>::operator= (complex<long double> const& _CS_c)
{
  _CS_m_real = _CS_c._CS_m_real;
  _CS_m_imag = _CS_c._CS_m_imag;
  return *this;
}

template <class _CS_S>
inline complex<long double>&
complex<long double>::operator= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real = _CS_c.real();
  _CS_m_imag = _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<long double>&
complex<long double>::operator+= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real += _CS_c.real();
  _CS_m_imag += _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<long double>&
complex<long double>::operator-= (complex<_CS_S> const& _CS_c)
{
  _CS_m_real -= _CS_c.real();
  _CS_m_imag -= _CS_c.imag();
  return *this;
}

template <class _CS_S>
inline complex<long double>&
complex<long double>::operator*= (complex<_CS_S> const& _CS_c)
{
  long double _CS_tmp = _CS_m_real * _CS_c.real() - _CS_m_imag * _CS_c.imag();
  _CS_m_imag = _CS_m_real * _CS_c.imag() + _CS_m_imag * _CS_c.real();
  _CS_m_real = _CS_tmp;
  return *this;
}

template <class _CS_S>
inline complex<long double>&
complex<long double>::operator/= (complex<_CS_S> const& _CS_c)
{
  long double _CS_real, _CS_imag;
  _CS_ldivide(_CS_m_real, _CS_m_imag, _CS_c.real(), _CS_c.imag(), _CS_real, _CS_imag);
  _CS_m_real = _CS_real;
  _CS_m_imag = _CS_imag;
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_T>
inline complex<_CS_T>
operator+ (complex<_CS_T> const& _CS_c1, complex<_CS_T> const& _CS_c2)
{
  return complex<_CS_T>(_CS_c1) += _CS_c2;
}

template <class _CS_T>
inline complex<_CS_T>
operator+ (_CS_T const& _CS_v, complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(_CS_c) += _CS_v;
}

template <class _CS_T>
inline complex<_CS_T>
operator+ (complex<_CS_T> const& _CS_c, _CS_T const& _CS_v)
{
  return complex<_CS_T>(_CS_c) += _CS_v;
}

template <class _CS_T>
inline complex<_CS_T>
operator- (complex<_CS_T> const& _CS_c1, complex<_CS_T> const& _CS_c2)
{
  return complex<_CS_T>(_CS_c1) -= _CS_c2;
}

template <class _CS_T>
inline complex<_CS_T>
operator- (_CS_T const& _CS_v, complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(_CS_v) -= _CS_c;
}

template <class _CS_T>
inline complex<_CS_T>
operator- (complex<_CS_T> const& _CS_c, _CS_T const& _CS_v)
{
  return complex<_CS_T>(_CS_c) -= _CS_v;
}


template <class _CS_T>
inline complex<_CS_T>
operator* (complex<_CS_T> const& _CS_c1, complex<_CS_T> const& _CS_c2)
{
  return complex<_CS_T>(_CS_c1) *= _CS_c2;
}

template <class _CS_T>
inline complex<_CS_T>
operator* (_CS_T const& _CS_v, complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(_CS_c) *= _CS_v;
}

template <class _CS_T>
inline complex<_CS_T>
operator* (complex<_CS_T> const& _CS_c, _CS_T const& _CS_v)
{
  return complex<_CS_T>(_CS_c) *= _CS_v;
}

template <class _CS_T>
inline complex<_CS_T>
operator/ (complex<_CS_T> const& _CS_c1, complex<_CS_T> const& _CS_c2)
{
  return complex<_CS_T>(_CS_c1) /= _CS_c2;
}

template <class _CS_T>
inline complex<_CS_T>
operator/ (_CS_T const& _CS_v, complex<_CS_T> const& _CS_c)
{
  return complex<_CS_T>(_CS_v) /= _CS_c;
}

template <class _CS_T>
inline complex<_CS_T>
operator/ (complex<_CS_T> const& _CS_c, _CS_T const& _CS_v)
{
  return complex<_CS_T>(_CS_c) /= _CS_v;
}


template <class _CS_T> inline complex<_CS_T> operator+ (complex<_CS_T> const& _CS_c) { return complex<_CS_T>(+_CS_c.real(), +_CS_c.imag()); }
template <class _CS_T> inline complex<_CS_T> operator- (complex<_CS_T> const& _CS_c) { return complex<_CS_T>(-_CS_c.real(), -_CS_c.imag()); }

template <class _CS_T>
inline bool
operator== (complex<_CS_T> const& _CS_c1, complex<_CS_T> const& _CS_c2)
{
  return _CS_c1.real() == _CS_c2.real() && _CS_c1.imag() == _CS_c2.imag();
}

template <class _CS_T>
inline bool
operator== (_CS_T const& _CS_v, complex<_CS_T> const& _CS_c)
{
  return _CS_c.real() == _CS_v && _CS_c.imag() == _CS_T();
}

template <class _CS_T>
inline bool
operator== (complex<_CS_T> const& _CS_c, _CS_T const& _CS_v)
{
  return _CS_c.real() == _CS_v && _CS_c.imag() == _CS_T();
}

template <class _CS_T>
inline bool
operator!= (complex<_CS_T> const& _CS_c1, complex<_CS_T> const& _CS_c2)
{
  return _CS_c1.real() != _CS_c2.real() || _CS_c1.imag() != _CS_c2.imag();
}

template <class _CS_T>
inline bool
operator!= (_CS_T const& _CS_v, complex<_CS_T> const& _CS_c)
{
  return _CS_c.real() != _CS_v || _CS_c.imag() != _CS_T();
}

template <class _CS_T>
inline bool
operator!= (complex<_CS_T> const& _CS_c, _CS_T const& _CS_v)
{
  return _CS_c.real() != _CS_v || _CS_c.imag() != _CS_T();
}

template <class _CS_T, class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>&, complex<_CS_T> const&);
template <class _CS_T, class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
operator>> (basic_istream<_CS_cT, _CS_traits>&, complex<_CS_T>&);

template <class _CS_T> inline _CS_T real(complex<_CS_T> const& _CS_c) { return _CS_c.real(); }
template <class _CS_T> inline _CS_T imag(complex<_CS_T> const& _CS_c) { return _CS_c.imag(); }

extern float abs(complex<float> const&);
extern double abs(complex<double> const&);
extern long double abs(complex<long double> const&);
template <class _CS_T> _CS_T abs(complex<_CS_T> const& _CS_c);

extern float arg(complex<float> const&);
extern double arg(complex<double> const&);
extern long double arg(complex<long double> const&);
template <class _CS_T> _CS_T arg(complex<_CS_T> const& _CS_c);

extern float norm(complex<float> const&);
extern double norm(complex<double> const&);
extern long double norm(complex<long double> const&);
template <class _CS_T> _CS_T norm(complex<_CS_T> const& _CS_c) { return _CS_c.real() * _CS_c.real() + _CS_c.imag() * _CS_c.imag(); }
template <class _CS_T> complex<_CS_T> conj(complex<_CS_T> const& _CS_c) { return complex<_CS_T>(_CS_c.real(), -_CS_c.imag()); }

extern complex<float> polar(float const&, float const&);
extern complex<double> polar(double const&, double const&);
extern complex<long double> polar(long double const&, long double const&);
template <class _CS_T>
complex<_CS_T>
polar(_CS_T const& _CS_a, _CS_T const& _CS_phi)
{
  return complex<_CS_T>(_CS_a * cos(_CS_phi), _CS_a * sin(_CS_phi));
}

template <class _CS_T>
complex<_CS_T>
cos(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
cosh(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
exp(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
log(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
log10(complex<_CS_T> const&);

template <class _CS_T>
complex<_CS_T>
pow(complex<_CS_T> const&, int);
template <class _CS_T>
complex<_CS_T>
pow(complex<_CS_T> const&, _CS_T const&);
template <class _CS_T>
complex<_CS_T>
pow(complex<_CS_T> const&, complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
pow(_CS_T const&, complex<_CS_T> const&);

template <class _CS_T>
complex<_CS_T>
sin(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
sinh(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
sqrt(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
tan(complex<_CS_T> const&);
template <class _CS_T>
complex<_CS_T>
tanh(complex<_CS_T> const&);

// --------------------------------------------------------------------------

template <class _CS_T> _CS_T abs(complex<_CS_T> const& _CS_c) { return sqrt(norm(_CS_c)); }
template <class _CS_T> _CS_T arg(complex<_CS_T> const& _CS_c) { return atan2(_CS_c.imag(), _CS_c.real()); }

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if !defined(__SRCCXX_COMPLEX_CC__)
#  include <srccxx/complex.cc>
#endif

#endif /* _CXXRT_COMPLEX_H__ */
