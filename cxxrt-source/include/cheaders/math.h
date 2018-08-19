// -*-C++-*- math.h
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
// Title:   Wrapper for the C library's math.h
// Version: $Id: math.h,v 1.2 2006/04/07 07:07:12 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(__MATH_H_CXX__)
#define __MATH_H_CXX__ 1

// -------------------------------------------------------------------------- 

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
extern "C"
{
#include _CXXRT_C_MATH_H
}


inline float acosf(float _CS_f) throw() { return ::acos(_CS_f); }
inline float asinf(float _CS_f) throw() { return ::asin(_CS_f); }
inline float atanf(float _CS_f) throw() { return ::atan(_CS_f); }
inline float atan2f(float _CS_f1, float _CS_f2) throw() { return ::atan2(_CS_f1, _CS_f2); }
#if 0 //-dk:TODO add configuration...?
inline float ceilf(float _CS_f) throw() { return ::ceil(_CS_f); }
#endif
inline float cosf(float _CS_f) throw() { return ::cos(_CS_f); }
inline float coshf(float _CS_f) throw() { return ::cosh(_CS_f); }
inline float expf(float _CS_f) throw() { return ::exp(_CS_f); }
#if 0 //-dk:TODO add configuration...?
inline float fabsf(float _CS_f) throw() { return ::fabs(_CS_f); }
inline float floorf(float _CS_f) throw() { return ::floor(_CS_f); }
#endif
inline float fmodf(float _CS_f, float _CS_d) throw() { return ::fmod(_CS_f, _CS_d); }
inline float frexpf(float _CS_f, int* _CS_e) throw() { return ::frexp(_CS_f, _CS_e); }
inline float ldexpf(float _CS_f, int _CS_e) throw() { return ::ldexp(_CS_f, _CS_e); }
inline float logf(float _CS_f) throw() { return ::log(_CS_f); }
inline float log10f(float _CS_f) throw() { return ::log10(_CS_f); }
inline float modff(float _CS_f, float* _CS_d) throw() {
  double _CS_tmp = *_CS_d;
  float  _CS_rc = ::modf(_CS_f, &_CS_tmp);
  *_CS_d = _CS_tmp;
  return _CS_rc;
}
inline float powf(float _CS_f, float _CS_e) throw() { return ::pow(_CS_f, _CS_e); }
inline float powf(float _CS_f, int _CS_e) throw() { return ::pow(_CS_f, _CS_e); }
inline float sinf(float _CS_f) throw() { return ::sin(_CS_f); }
inline float sinhf(float _CS_f) throw() { return ::sinh(_CS_f); }
inline float sqrtf(float _CS_f) throw() { return ::sqrt(_CS_f); }
inline float tanf(float _CS_f) throw() { return ::tan(_CS_f); }
inline float tanhf(float _CS_f) throw() { return ::tanh(_CS_f); }

inline long double acosl(long double _CS_f) throw() { return ::acos(_CS_f); }
inline long double asinl(long double _CS_f) throw() { return ::asin(_CS_f); }
inline long double atanl(long double _CS_f) throw() { return ::atan(_CS_f); }
inline long double atan2l(long double _CS_f1, long double _CS_f2) throw() { return ::atan2(_CS_f1, _CS_f2); }
#if 0 //-dk:TODO add configuration...?
inline long double ceill(long double _CS_f) throw() { return ::ceil(_CS_f); }
#endif
inline long double cosl(long double _CS_f) throw() { return ::cos(_CS_f); }
inline long double coshl(long double _CS_f) throw() { return ::cosh(_CS_f); }
inline long double expl(long double _CS_f) throw() { return ::exp(_CS_f); }
#if 0 //-dk:TODO add configuration...?
extern "C" inline long double fabsl(long double _CS_f) throw() { return ::fabs(_CS_f); }
inline long double floorl(long double _CS_f) throw() { return ::floor(_CS_f); }
#endif
inline long double fmodl(long double _CS_f, long double _CS_d) throw() { return ::fmod(_CS_f, _CS_d); }
inline long double frexpl(long double _CS_f, int* _CS_e) throw() { return ::frexp(_CS_f, _CS_e); }
inline long double ldexpl(long double _CS_f, int _CS_e) throw() { return ::ldexp(_CS_f, _CS_e); }
inline long double logl(long double _CS_f) throw() { return ::log(_CS_f); }
inline long double log10l(long double _CS_f) throw() { return ::log10(_CS_f); }
inline long double modfl(long double _CS_f, long double* _CS_d) throw() {
  double _CS_tmp = *_CS_d;
  long double _CS_rc = ::modf(_CS_f, &_CS_tmp);
  *_CS_d = _CS_tmp;
  return _CS_rc;
}
inline long double powl(long double _CS_f, long double _CS_e) throw() { return ::pow(_CS_f, _CS_e); }
inline long double powl(long double _CS_f, int _CS_e) throw() { return ::pow(_CS_f, _CS_e); }
inline long double sinl(long double _CS_f) throw() { return ::sin(_CS_f); }
inline long double sinhl(long double _CS_f) throw() { return ::sinh(_CS_f); }
inline long double sqrtl(long double _CS_f) throw() { return ::sqrt(_CS_f); }
inline long double tanl(long double _CS_f) throw() { return ::tan(_CS_f); }
inline long double tanhl(long double _CS_f) throw() { return ::tanh(_CS_f); }

_CXXRT_NAMESPACE_BEGIN

  inline float abs(float _CS_f) { return ::fabs(_CS_f); }
  inline float acos(float _CS_f) { return acosf(_CS_f); }
  inline float asin(float _CS_f) { return asinf(_CS_f); }
  inline float atan(float _CS_f) { return atanf(_CS_f); }
  inline float atan2(float _CS_f1, float _CS_f2) { return atan2f(_CS_f1, _CS_f2); }
  inline float ceil(float _CS_f) { return ceilf(_CS_f); }
  inline float cos(float _CS_f) { return cosf(_CS_f); }
  inline float cosh(float _CS_f) { return coshf(_CS_f); }
  inline float exp(float _CS_f) { return expf(_CS_f); }
  inline float fabs(float _CS_f) { return fabsf(_CS_f); }
  inline float floor(float _CS_f) { return floorf(_CS_f); }
  inline float fmod(float _CS_f, float _CS_d) { return fmodf(_CS_f, _CS_d); }
  inline float frexp(float _CS_f, int* _CS_e) { return frexpf(_CS_f, _CS_e); }
  inline float ldexp(float _CS_f, int _CS_e) { return ldexpf(_CS_f, _CS_e); }
  inline float log(float _CS_f) { return logf(_CS_f); }
  inline float log10(float _CS_f) { return log10f(_CS_f); }
  inline float modf(float _CS_f, float* _CS_d) { return modff(_CS_f, _CS_d); }
  inline float pow(float _CS_f, float _CS_e) { return powf(_CS_f, _CS_e); }
  inline float pow(float _CS_f, int _CS_e) { return powf(_CS_f, _CS_e); }
  inline float sin(float _CS_f) { return sinf(_CS_f); }
  inline float sinh(float _CS_f) { return sinhf(_CS_f); }
  inline float sqrt(float _CS_f) { return sqrtf(_CS_f); }
  inline float tan(float _CS_f) { return tanf(_CS_f); }
  inline float tanh(float _CS_f) { return tanhf(_CS_f); }

  inline double abs(double _CS_f) { return ::fabs(_CS_f); }
  inline double pow(double _CS_f, int _CS_e) { return ::pow(_CS_f, _CS_e); }
  using ::acos;
  using ::asin;
  using ::atan;
  using ::atan2;
  using ::ceil;
  using ::cos;
  using ::cosh;
  using ::exp;
  using ::fabs;
  using ::floor;
  using ::fmod;
  using ::frexp;
  using ::ldexp;
  using ::log;
  using ::log10;
  using ::modf;
  using ::pow;
  using ::sin;
  using ::sinh;
  using ::sqrt;
  using ::tan;
  using ::tanh;

  inline long double abs(long double _CS_f) { return ::fabs(_CS_f); }
  inline long double acos(long double _CS_f) { return acosl(_CS_f); }
  inline long double asin(long double _CS_f) { return asinl(_CS_f); }
  inline long double atan(long double _CS_f) { return atanl(_CS_f); }
  inline long double atan2(long double _CS_f1, long double _CS_f2) { return atan2l(_CS_f1, _CS_f2); }
  inline long double ceil(long double _CS_f) { return ceill(_CS_f); }
  inline long double cos(long double _CS_f) { return cosl(_CS_f); }
  inline long double cosh(long double _CS_f) { return coshl(_CS_f); }
  inline long double exp(long double _CS_f) { return expl(_CS_f); }
  inline long double fabs(long double _CS_f) { return fabsl(_CS_f); }
  inline long double floor(long double _CS_f) { return floorl(_CS_f); }
  inline long double fmod(long double _CS_f, long double _CS_d) { return fmodl(_CS_f, _CS_d); }
  inline long double frexp(long double _CS_f, int* _CS_e) { return frexpl(_CS_f, _CS_e); }
  inline long double ldexp(long double _CS_f, int _CS_e) { return ldexpl(_CS_f, _CS_e); }
  inline long double log(long double _CS_f) { return logl(_CS_f); }
  inline long double log10(long double _CS_f) { return log10l(_CS_f); }
  inline long double modf(long double _CS_f, long double* _CS_d) { return modfl(_CS_f, _CS_d); }
  inline long double pow(long double _CS_f, long double _CS_e) { return powl(_CS_f, _CS_e); }
  inline long double pow(long double _CS_f, int _CS_e) { return powl(_CS_f, _CS_e); }
  inline long double sin(long double _CS_f) { return sinl(_CS_f); }
  inline long double sinh(long double _CS_f) { return sinhl(_CS_f); }
  inline long double sqrt(long double _CS_f) { return sqrtl(_CS_f); }
  inline long double tan(long double _CS_f) { return tanl(_CS_f); }
  inline long double tanh(long double _CS_f) { return tanhl(_CS_f); }

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

#endif /* __MATH_H_CXX__ */
