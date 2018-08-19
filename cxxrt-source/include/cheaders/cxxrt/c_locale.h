// -*-C++-*- cxxrt/c_locale.h
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
// Title:   Declarations for a thin layer to use platform specific locales
// Version: $Id: c_locale.h,v 1.1.1.1 2002/06/05 01:01:51 dietmar_kuehl Exp $

// Note: The implementation from Matt Austern (SGI) uses the same
// layer. Is is done to support switching implementations. The
// definition of this interface is from Matt Austern.

// This implementation uses a portable approach despite what is said
// in Matt's documentation: The "trick" is to ignore what is already
// provided by the platform.
// --------------------------------------------------------------------------

#if !defined(_CXXRT_C_LOCALE_H__)
#define _CXXRT_C_LOCALE_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif

#include <wchar.h>
#if !defined(_CXXRT_C_LOCDEF_H__)
#  include <cxxrt/c_locdef.h>
#endif
#if !defined(_STDIO_H_CXX__)
#  include <stdio.h>
#endif
#if !defined(_WCHAR_H_CXX__)
#  include <wchar.h>
#endif

#ifdef __cplusplus
extern "C"
{
  using ::_CXXRT_STD_NAME::mbstate_t;
#if !defined(_MSC_VER)
  //using ::_CXXRT_STD_NAME::size_t;
#endif
  using ::_CXXRT_STD_NAME::wint_t;
#endif

// --------------------------------------------------------------------------

#define _Locale_MAX_SIMPLE_NAME 256
#define _Locale_MAX_COMPOSITE_NAME (6*(_Locale_MAX_SIMPLE_NAME+3))

#if !defined(__cplusplus)
typedef struct mbstate_t mbstate_t;
#endif
typedef struct _Locale_filepos _Locale_filepos;

// --------------------------------------------------------------------------
// global stuff

char* _Locale_compose_name(char*, char const*, char const*, char const*, char const*,
                                                                                                         char const*, char const*);

// --- stuff specific to this implementation --------------------------------

struct _Locale_file;
void _Locale_openfile(_Locale_file*, char const*, char const*);

// --------------------------------------------------------------------------

struct _Locale_ctype;
struct _Locale_numeric;
struct _Locale_time;
struct _Locale_collate;
struct _Locale_monetary;
struct _Locale_messages;

// --------------------------------------------------------------------------
// ctype stuff

// general interface:
struct _Locale_ctype* _Locale_ctype_create(char const*);
void _Locale_ctype_destroy(struct _Locale_ctype*);
char* _Locale_ctype_name(struct _Locale_ctype const*, char*);
char* _Locale_extract_ctype_name(char const*, char*);

// narrow characters:
_Locale_mask_t* _Locale_ctype_table(struct _Locale_ctype*);
int _Locale_toupper(struct _Locale_ctype*, int);
int _Locale_tolower(struct _Locale_ctype*, int);

// wide characters:
_Locale_mask_t _Locale_wchar_ctype(struct _Locale_ctype*, wint_t);
wint_t _Locale_wchar_tolower(struct _Locale_ctype*, wint_t);
wint_t _Locale_wchar_toupper(struct _Locale_ctype*, wint_t);
wint_t _Locale_btowc(struct _Locale_ctype*, int);
int _Locale_wctob(struct _Locale_ctype*, wint_t);
_CXXRT_STD size_t _Locale_mbtowc(struct _Locale_ctype*, wchar_t*, char const*, _CXXRT_STD size_t, _CXXRT_STD mbstate_t*);
_CXXRT_STD size_t _Locale_wctomb(struct _Locale_ctype*, char*, _CXXRT_STD size_t, wchar_t, _CXXRT_STD mbstate_t*);
_CXXRT_STD size_t _Locale_unshift(struct _Locale_ctype*, _CXXRT_STD mbstate_t*, char*, _CXXRT_STD size_t, char**);

int _Locale_mb_cur_max (struct _Locale_ctype*);
int _Locale_mb_cur_min (struct _Locale_ctype*);
int _Locale_is_stateless (struct _Locale_ctype*);

wint_t _Locale_getwc(struct _Locale_ctype*, FILE*);
wint_t _Locale_putwc(struct _Locale_ctype*, wchar_t, FILE*);
wint_t _Locale_ungetwc(struct _Locale_ctype*, wchar_t, FILE*);
int _Locale_fsetpos(struct _Locale_ctype*, FILE*, _Locale_filepos const*);
int _Locale_fgetpos(struct _Locale_ctype*, FILE*, _Locale_filepos*);

// --------------------------------------------------------------------------

struct _Locale_monetary* _Locale_monetary_create(char const*);
char* _Locale_monetary_name(struct _Locale_monetary const*, char*);
void _Locale_monetary_destroy(struct _Locale_monetary*);
char* _Locale_extract_monetary_name(char const*, char*);

char const*  _Locale_int_curr_symbol(struct _Locale_monetary*);
char const*  _Locale_currency_symbol(struct _Locale_monetary*);
char         _Locale_mon_decimal_point(struct _Locale_monetary*);
char         _Locale_mon_thousands_sep(struct _Locale_monetary*);
char const*  _Locale_mon_grouping(struct _Locale_monetary*);
char const*  _Locale_positive_sign(struct _Locale_monetary*);
char const*  _Locale_negative_sign(struct _Locale_monetary*);
char         _Locale_int_frac_digits(struct _Locale_monetary*);
char         _Locale_frac_digits(struct _Locale_monetary*);
int          _Locale_p_cs_precedes(struct _Locale_monetary*);
int          _Locale_p_sep_by_space(struct _Locale_monetary*);
int          _Locale_p_sign_pos(struct _Locale_monetary*);
int          _Locale_n_cs_precedes(struct _Locale_monetary*);
int          _Locale_n_sep_by_space(struct _Locale_monetary*);
int          _Locale_n_sign_pos(struct _Locale_monetary*);
int          _Locale_p_sign_posn(struct _Locale_monetary*);
int          _Locale_n_sign_posn(struct _Locale_monetary*);

// --------------------------------------------------------------------------

struct _Locale_numeric* _Locale_numeric_create(char const*);
char* _Locale_numeric_name(struct _Locale_numeric const*, char*);
void _Locale_numeric_destroy(struct _Locale_numeric*);
char* _Locale_extract_numeric_name(char const*, char*);

char _Locale_decimal_point(struct _Locale_numeric*);
char _Locale_thousands_sep(struct _Locale_numeric*);
char const* _Locale_grouping(struct _Locale_numeric*);
char const* _Locale_true(struct _Locale_numeric*);
char const* _Locale_false(struct _Locale_numeric*);

// --------------------------------------------------------------------------

struct _Locale_time* _Locale_time_create(char const*);
char* _Locale_time_name(struct _Locale_time const*, char*);
void _Locale_time_destroy(struct _Locale_time*);
char* _Locale_extract_time_name(char const*, char*);

char const** _Locale_full_monthname(struct _Locale_time*);
char const** _Locale_abbrev_monthname(struct _Locale_time*);
char const** _Locale_full_dayofweek(struct _Locale_time*);
char const** _Locale_abbrev_dayofweek(struct _Locale_time*);
int _Locale_dateorder(struct _Locale_time*);
_CXXRT_STD size_t _Locale_time_format(char*, _CXXRT_STD size_t, struct tm const*, char, char, char);

char const* _Locale_d_t_fmt(struct _Locale_time*);
char const* _Locale_d_fmt(struct _Locale_time*);
char const* _Locale_t_fmt(struct _Locale_time*);
char const* _Locale_am_str(struct _Locale_time*);
char const* _Locale_pm_str(struct _Locale_time*);
char const* _Locale_t_fmt_ampm(struct _Locale_time*);

// --------------------------------------------------------------------------

struct _Locale_collate* _Locale_collate_create(char const*);
char* _Locale_collate_name(struct _Locale_collate const*, char*);
void _Locale_collate_destroy(struct _Locale_collate*);
char* _Locale_extract_collate_name(char const*, char*);

int _Locale_strcmp(struct _Locale_collate*, char const*, _CXXRT_STD size_t, char const*, _CXXRT_STD size_t);
int _Locale_strwcmp(struct _Locale_collate*, wchar_t const*, _CXXRT_STD size_t, wchar_t const*, _CXXRT_STD size_t);
_CXXRT_STD size_t _Locale_strxfrm(struct _Locale_collate*, char*, _CXXRT_STD size_t, char const*, _CXXRT_STD size_t);
_CXXRT_STD size_t _Locale_strwxfrm(struct _Locale_collate*, wchar_t*, _CXXRT_STD size_t, wchar_t const*, _CXXRT_STD size_t);

// --------------------------------------------------------------------------

struct _Locale_messages* _Locale_messages_create(char const*);
char* _Locale_messages_name(struct _Locale_messages const*, char*);
void _Locale_messages_destroy(struct _Locale_messages*);
char* _Locale_extract_messages_name(char const*, char*);

int _Locale_catopen(struct _Locale_messages*, char const*);
void _Locale_catclose(struct _Locale_messages*, int);
char const* _Locale_catgets(struct _Locale_messages*, int, int, int, char const*);

// --------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* _CXXRT_C_LOCALE_H__ */
