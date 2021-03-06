// -*-C++-*- cxxrt/limits.h
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
// Title:  An implementation of the C++ Standard file <limits>
// Version: $Id: limits.h.in,v 1.4 2003/04/13 19:36:00 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_LIMITS_H__)
#define _CXXRT_LIMITS_H__ 1

// --------------------------------------------------------------------------
// This is probably not standard conforming but should be sufficient
// for now: Having an implementation which is close to the standard
// (but not really standard conforming) is better than having no
// implementation at all. However, if including these files is a real
// problem, it would be easy to build a script which creates a
// '#include' free 'limits' by processing it with the preprocessor and
// then putting some stuff around it again...

#if !defined(_CXXRTCF_H__)
#  include "cxxrtcf.h"
#endif
#include <limits.h>

#if !defined(__CFLOAT__)
#  include <cfloat>
#endif
#if !defined(__CWCHAR__)
#  include <cwchar>
#endif

_CXXRT_NAMESPACE_BEGIN

//------------------------------------------------------------------------------

enum float_round_style
{
  round_indeterminate       = -1,
  round_toward_zero         = 0,
  round_to_nearest          = 1,
  round_toward_infinity     = 2,
  round_toward_neg_infinity = 3
};

enum float_denorm_style
{
  denorm_indeterminate = -1,
  denorm_absent        = 0,
  denorm_present       = 1
};

class _CS_numeric_limits
{
public:
  static bool const is_specialized    = false;

  static bool const is_signed         = false;
  static bool const is_integer        = false;
  static bool const is_exact          = false;

  static bool const has_infinity      = false;
  static bool const has_quiet_NaN     = false;
  static bool const has_signaling_NaN = false;
  static float_denorm_style const has_denorm = denorm_indeterminate;
  static bool const has_denorm_loss   = false;

  static bool const is_iec559         = false;
  static bool const is_bounded        = false;
  static bool const is_modulo         = false;

  static bool const traps             = false;
  static bool const tinyness_before   = false;

  static int const  digits         = 0;
  static int const  digits10       = 0;
  static int const  radix          = 0;

  static int const  min_exponent   = 0;
  static int const  min_exponent10 = 0;
  static int const  max_exponent   = 0;
  static int const  max_exponent10 = 0;

  static float_round_style const round_style = round_toward_zero;
};

//------------------------------------------------------------------------------
// the default specialization: just use the default defined in the base class.

template <class T>
class numeric_limits: public _CS_numeric_limits
{
public:
  inline static T min() _CXXRT_THROW_SPEC(())           { return T(); }
  inline static T max() _CXXRT_THROW_SPEC(())           { return T(); }
  inline static T epsilon() _CXXRT_THROW_SPEC(())       { return T(); }
  inline static T round_error() _CXXRT_THROW_SPEC(())   { return T(); }

  inline static T infinity() _CXXRT_THROW_SPEC(())      { return T(); }
  inline static T quiet_NaN() _CXXRT_THROW_SPEC(())     { return T(); }
  inline static T signaling_NaN() _CXXRT_THROW_SPEC(()) { return T(); }
  inline static T denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

//------------------------------------------------------------------------------
// specialization of the boolean type:

template <>
class numeric_limits<bool>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = false;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 1;
  static int const digits10 = 0;
  static int const radix = 2;

  inline static bool min() _CXXRT_THROW_SPEC(()) { return false; }
  inline static bool max() _CXXRT_THROW_SPEC(()) { return true; }

  inline static bool epsilon() _CXXRT_THROW_SPEC(())       { return bool(); }
  inline static bool round_error() _CXXRT_THROW_SPEC(())   { return bool(); }

  inline static bool infinity() _CXXRT_THROW_SPEC(())      { return bool(); }
  inline static bool quiet_NaN() _CXXRT_THROW_SPEC(())     { return bool(); }
  inline static bool signaling_NaN() _CXXRT_THROW_SPEC(()) { return bool(); }
  inline static bool denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

//------------------------------------------------------------------------------
// specializations for the character types:

template <>
class numeric_limits<char>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = 127 == 127? true: false;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 - (127 == 127? 1: 0);
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static char min() _CXXRT_THROW_SPEC(()) { return (-127 -1); }
  inline static char max() _CXXRT_THROW_SPEC(()) { return 127; }

  inline static char epsilon() _CXXRT_THROW_SPEC(())       { return char(); }
  inline static char round_error() _CXXRT_THROW_SPEC(())   { return char(); }

  inline static char infinity() _CXXRT_THROW_SPEC(())      { return char(); }
  inline static char quiet_NaN() _CXXRT_THROW_SPEC(())     { return char(); }
  inline static char signaling_NaN() _CXXRT_THROW_SPEC(()) { return char(); }
  inline static char denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<signed char>: public _CS_numeric_limits
{
private:
  typedef signed char schar;

public:
  static bool const is_specialized = true;

  static bool const is_signed  = true;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 - 1;
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static signed char min() _CXXRT_THROW_SPEC(()) { return (-127 -1); }
  inline static signed char max() _CXXRT_THROW_SPEC(()) { return 127; }

  inline static signed char epsilon() _CXXRT_THROW_SPEC(())       { return schar(); }
  inline static signed char round_error() _CXXRT_THROW_SPEC(())   { return schar(); }

  inline static signed char infinity() _CXXRT_THROW_SPEC(())      { return schar(); }
  inline static signed char quiet_NaN() _CXXRT_THROW_SPEC(())     { return schar(); }
  inline static signed char signaling_NaN() _CXXRT_THROW_SPEC(()) { return schar(); }
  inline static signed char denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<unsigned char>: public _CS_numeric_limits
{
private:
  typedef unsigned char uchar;

public:
  static bool const is_specialized = true;

  static bool const is_signed  = false;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8;
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static unsigned char min() _CXXRT_THROW_SPEC(()) { return 0; }
  inline static unsigned char max() _CXXRT_THROW_SPEC(()) { return (127*2 +1); }

  inline static unsigned char epsilon() _CXXRT_THROW_SPEC(())       { return uchar(); }
  inline static unsigned char round_error() _CXXRT_THROW_SPEC(())   { return uchar(); }

  inline static unsigned char infinity() _CXXRT_THROW_SPEC(())      { return uchar(); }
  inline static unsigned char quiet_NaN() _CXXRT_THROW_SPEC(())     { return uchar(); }
  inline static unsigned char signaling_NaN() _CXXRT_THROW_SPEC(()) { return uchar(); }
  inline static unsigned char denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<wchar_t>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = false; // TODO: check the "signedness" of 'wchar_t'
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 * sizeof(wchar_t); // TODO: if necessary date it up
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static wchar_t min() _CXXRT_THROW_SPEC(()) { return WCHAR_MIN; }
  inline static wchar_t max() _CXXRT_THROW_SPEC(()) { return WCHAR_MAX; }

  inline static wchar_t epsilon() _CXXRT_THROW_SPEC(())       { return wchar_t(); }
  inline static wchar_t round_error() _CXXRT_THROW_SPEC(())   { return wchar_t(); }

  inline static wchar_t infinity() _CXXRT_THROW_SPEC(())      { return wchar_t(); }
  inline static wchar_t quiet_NaN() _CXXRT_THROW_SPEC(())     { return wchar_t(); }
  inline static wchar_t signaling_NaN() _CXXRT_THROW_SPEC(()) { return wchar_t(); }
  inline static wchar_t denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

//------------------------------------------------------------------------------
// specializations for the integer types:

template <>
class numeric_limits<short>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = true;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 * sizeof(short) - 1;
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static short min() _CXXRT_THROW_SPEC(()) { return (-32767 -1); }
  inline static short max() _CXXRT_THROW_SPEC(()) { return 32767; }

  inline static short epsilon() _CXXRT_THROW_SPEC(())       { return short(); }
  inline static short round_error() _CXXRT_THROW_SPEC(())   { return short(); }

  inline static short infinity() _CXXRT_THROW_SPEC(())      { return short(); }
  inline static short quiet_NaN() _CXXRT_THROW_SPEC(())     { return short(); }
  inline static short signaling_NaN() _CXXRT_THROW_SPEC(()) { return short(); }
  inline static short denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<int>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = true;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 * sizeof(int) - 1;
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static int min() _CXXRT_THROW_SPEC(()) { return (-2147483647 -1); }
  inline static int max() _CXXRT_THROW_SPEC(()) { return 2147483647; }

  inline static int epsilon() _CXXRT_THROW_SPEC(())       { return int(); }
  inline static int round_error() _CXXRT_THROW_SPEC(())   { return int(); }

  inline static int infinity() _CXXRT_THROW_SPEC(())      { return int(); }
  inline static int quiet_NaN() _CXXRT_THROW_SPEC(())     { return int(); }
  inline static int signaling_NaN() _CXXRT_THROW_SPEC(()) { return int(); }
  inline static int denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<long>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = true;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 * sizeof(long) - 1;
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static long min() _CXXRT_THROW_SPEC(()) { return (-9223372036854775807L -1L); }
  inline static long max() _CXXRT_THROW_SPEC(()) { return 9223372036854775807L; }

  inline static long epsilon() _CXXRT_THROW_SPEC(())       { return long(); }
  inline static long round_error() _CXXRT_THROW_SPEC(())   { return long(); }

  inline static long infinity() _CXXRT_THROW_SPEC(())      { return long(); }
  inline static long quiet_NaN() _CXXRT_THROW_SPEC(())     { return long(); }
  inline static long signaling_NaN() _CXXRT_THROW_SPEC(()) { return long(); }
  inline static long denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<unsigned short>: public _CS_numeric_limits
{
private:
  typedef unsigned short ushort;

public:
  static bool const is_specialized = true;

  static bool const is_signed  = false;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 * sizeof(unsigned short);
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static unsigned short min() _CXXRT_THROW_SPEC(()) { return 0; }
  inline static unsigned short max() _CXXRT_THROW_SPEC(()) { return (32767 *2 +1); }

  inline static unsigned short epsilon() _CXXRT_THROW_SPEC(())       { return ushort(); }
  inline static unsigned short round_error() _CXXRT_THROW_SPEC(())   { return ushort(); }

  inline static unsigned short infinity() _CXXRT_THROW_SPEC(())      { return ushort(); }
  inline static unsigned short quiet_NaN() _CXXRT_THROW_SPEC(())     { return ushort(); }
  inline static unsigned short signaling_NaN() _CXXRT_THROW_SPEC(()) { return ushort(); }
  inline static unsigned short denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<unsigned int>: public _CS_numeric_limits
{
private:
  typedef unsigned int uint;

public:
  static bool const is_specialized = true;

  static bool const is_signed  = false;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 * sizeof(unsigned int);
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static unsigned int min() _CXXRT_THROW_SPEC(()) { return 0; }
  inline static unsigned int max() _CXXRT_THROW_SPEC(()) { return (2147483647 *2U +1U); }

  inline static unsigned int epsilon() _CXXRT_THROW_SPEC(())       { return uint(); }
  inline static unsigned int round_error() _CXXRT_THROW_SPEC(())   { return uint(); }

  inline static unsigned int infinity() _CXXRT_THROW_SPEC(())      { return uint(); }
  inline static unsigned int quiet_NaN() _CXXRT_THROW_SPEC(())     { return uint(); }
  inline static unsigned int signaling_NaN() _CXXRT_THROW_SPEC(()) { return uint(); }
  inline static unsigned int denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<unsigned long>: public _CS_numeric_limits
{
private:
  typedef unsigned long ulong;

public:
  static bool const is_specialized = true;

  static bool const is_signed  = false;
  static bool const is_integer = true;
  static bool const is_bounded = true;
  static bool const is_exact   = true;

  static int const digits = 8 * sizeof(unsigned long);
  static int const digits10 = 1 + (30103 * digits) / 100000;
  static int const radix = 2;

  inline static unsigned long min() _CXXRT_THROW_SPEC(()) { return 0; }
  inline static unsigned long max() _CXXRT_THROW_SPEC(()) { return (9223372036854775807L *2UL+1UL); }

  inline static unsigned long epsilon() _CXXRT_THROW_SPEC(())       { return ulong(); }
  inline static unsigned long round_error() _CXXRT_THROW_SPEC(())   { return ulong(); }

  inline static unsigned long infinity() _CXXRT_THROW_SPEC(())      { return ulong(); }
  inline static unsigned long quiet_NaN() _CXXRT_THROW_SPEC(())     { return ulong(); }
  inline static unsigned long signaling_NaN() _CXXRT_THROW_SPEC(()) { return ulong(); }
  inline static unsigned long denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

//------------------------------------------------------------------------------
// specializations for the floating point types:

template <>
class numeric_limits<float>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = true;
  static bool const is_integer = false;
  static bool const is_bounded = true;

  static int const digits   = FLT_MANT_DIG;
  static int const digits10 = FLT_DIG;
  static int const radix    = FLT_RADIX;
  static int const min_exponent = FLT_MIN_EXP;
  static int const min_exponent_10 = FLT_MIN_10_EXP;
  static int const max_exponent = FLT_MAX_EXP;
  static int const max_exponent_10 = FLT_MAX_10_EXP;

  static float_round_style const round_style = FLT_ROUNDS == 0? round_toward_zero:
                                               FLT_ROUNDS == 1? round_to_nearest:
                                               FLT_ROUNDS == 2? round_toward_infinity:
                                               FLT_ROUNDS == 3? round_toward_neg_infinity:
                                                                round_indeterminate;

  inline static float min() _CXXRT_THROW_SPEC(()) { return FLT_MIN; }
  inline static float max() _CXXRT_THROW_SPEC(()) { return FLT_MAX; }
  inline static float epsilon() _CXXRT_THROW_SPEC(()) { return FLT_EPSILON; }
  inline static float round_error() _CXXRT_THROW_SPEC(()) { return FLT_ROUNDS == 1? 0.5f: 1.0f; }

  inline static float infinity() _CXXRT_THROW_SPEC(())      { return float(); }
  inline static float quiet_NaN() _CXXRT_THROW_SPEC(())     { return float(); }
  inline static float signaling_NaN() _CXXRT_THROW_SPEC(()) { return float(); }
  inline static float denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<double>: public _CS_numeric_limits
{
public:
  static bool const is_specialized = true;

  static bool const is_signed  = true;
  static bool const is_integer = false;
  static bool const is_bounded = true;

  static int const digits   = DBL_MANT_DIG;
  static int const digits10 = DBL_DIG;
  static int const radix    = FLT_RADIX;
  static int const min_exponent = DBL_MIN_EXP;
  static int const min_exponent_10 = DBL_MIN_10_EXP;
  static int const max_exponent = DBL_MAX_EXP;
  static int const max_exponent_10 = DBL_MAX_10_EXP;

  static float_round_style const round_style = FLT_ROUNDS == 0? round_toward_zero:
                                               FLT_ROUNDS == 1? round_to_nearest:
                                               FLT_ROUNDS == 2? round_toward_infinity:
                                               FLT_ROUNDS == 3? round_toward_neg_infinity:
                                                                round_indeterminate;

  inline static double min() _CXXRT_THROW_SPEC(()) { return DBL_MIN; }
  inline static double max() _CXXRT_THROW_SPEC(()) { return DBL_MAX; }
  inline static double epsilon() _CXXRT_THROW_SPEC(()) { return DBL_EPSILON; }
  inline static float round_error() _CXXRT_THROW_SPEC(()) { return FLT_ROUNDS == 1? 0.5: 1.0; }

  inline static double infinity() _CXXRT_THROW_SPEC(())      { return double(); }
  inline static double quiet_NaN() _CXXRT_THROW_SPEC(())     { return double(); }
  inline static double signaling_NaN() _CXXRT_THROW_SPEC(()) { return double(); }
  inline static double denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

template <>
class numeric_limits<long double>: public _CS_numeric_limits
{
private:
  typedef long double ldouble;

public:
  static bool const is_specialized = true;

  static bool const is_signed  = true;
  static bool const is_integer = false;
  static bool const is_bounded = true;

  static int const digits   = LDBL_MANT_DIG;
  static int const digits10 = LDBL_DIG;
  static int const radix    = FLT_RADIX;
  static int const min_exponent = LDBL_MIN_EXP;
  static int const min_exponent_10 = LDBL_MIN_10_EXP;
  static int const max_exponent = LDBL_MAX_EXP;
  static int const max_exponent_10 = LDBL_MAX_10_EXP;

  static float_round_style const round_style = FLT_ROUNDS == 0? round_toward_zero:
                                               FLT_ROUNDS == 1? round_to_nearest:
                                               FLT_ROUNDS == 2? round_toward_infinity:
                                               FLT_ROUNDS == 3? round_toward_neg_infinity:
                                                                round_indeterminate;

  static long double min() _CXXRT_THROW_SPEC(()) { return 1.18973149535723176502e+4932L; }
  static long double max() _CXXRT_THROW_SPEC(()) { return 3.36210314311209350626e-4932L; }
  static long double epsilon() _CXXRT_THROW_SPEC(()) { return 1.08420217248550443401e-19L; }
  inline static float round_error() _CXXRT_THROW_SPEC(()) { return FLT_ROUNDS == 1? 0.5l: 1.0l; }

  inline static long double infinity() _CXXRT_THROW_SPEC(())      { return ldouble(); }
  inline static long double quiet_NaN() _CXXRT_THROW_SPEC(())     { return ldouble(); }
  inline static long double signaling_NaN() _CXXRT_THROW_SPEC(()) { return ldouble(); }
  inline static long double denorm_min() _CXXRT_THROW_SPEC(())    { return min(); }
};

//------------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_LIMITS_H__ */
