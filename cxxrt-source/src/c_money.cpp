// -*-C++-*- src/c_monetary.cc
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
// Title:   Definition of the low-level interface to the monetary facet
// Version: $Id: c_money.cpp,v 1.1.1.1 2002/06/05 01:00:03 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#include "cxxrt/c_locale.h"
#include "cxxrt/c_lfile.h"
#include <cstring>

// -------------------------------------------------------------------------- 

struct _Locale_monetary
{
  char        m_name[_Locale_MAX_SIMPLE_NAME];
  char const* m_int_curr_symbol;
  char const* m_currency_symbol;
  char const* m_mon_grouping;
  char const* m_positive_sign;
  char const* m_negative_sign;
  char        m_mon_decimal_point;
  char        m_mon_thousands_sep;
  char        m_int_frac_digits;
  char        m_frac_digits;
  int         m_p_cs_precedes;
  int         m_p_sep_by_space;
  int         m_p_sign_pos;
  int         m_p_sign_posn;
  int         m_n_cs_precedes;
  int         m_n_sep_by_space;
  int         m_n_sign_pos;
  int         m_n_sign_posn;
};

// -------------------------------------------------------------------------- 

struct _Locale_monetary* _Locale_monetary_create(char const* name)
{
  _Locale_monetary* rc = new _Locale_monetary;
  _CXXRT_STD strcpy(rc->m_name, name);

  rc->m_int_curr_symbol = 0;
  rc->m_currency_symbol = 0;
  rc->m_mon_grouping    = 0;
  rc->m_positive_sign   = 0;
  rc->m_negative_sign   = 0;

  _Locale_file file;
  _Locale_openfile(&file, name, "monetary");
  
  char dummy;
  rc->m_int_curr_symbol = file._CS_read_string();
  rc->m_currency_symbol = file._CS_read_string();
  file._CS_read(rc->m_mon_decimal_point);
  file._CS_read(dummy);
  file._CS_read(rc->m_mon_thousands_sep);
  file._CS_read(dummy);
  rc->m_mon_grouping = file._CS_read_string();
  rc->m_positive_sign = file._CS_read_string();
  rc->m_negative_sign = file._CS_read_string();
  
  int tmp;
  file._CS_read(tmp);
  rc->m_int_frac_digits = tmp;
  file._CS_read(tmp);
  rc->m_frac_digits = tmp;

  file._CS_read(rc->m_p_cs_precedes);
  file._CS_read(rc->m_p_sep_by_space);
  file._CS_read(rc->m_p_sign_pos);
  file._CS_read(rc->m_p_sign_posn);
  file._CS_read(rc->m_n_cs_precedes);
  file._CS_read(rc->m_n_sep_by_space);
  file._CS_read(rc->m_n_sign_pos);
  file._CS_read(rc->m_n_sign_posn);

  if (file._CS_failed())
    {
      _Locale_monetary_destroy(rc);
      return 0;
    }

  return rc;
}

// -------------------------------------------------------------------------- 

char* _Locale_monetary_name(const struct _Locale_monetary* rec, char* buffer)
{
  return _CXXRT_STD strcpy(buffer, rec->m_name);
}

void _Locale_monetary_destroy(struct _Locale_monetary* rec)
{
  delete[] const_cast<char*>(rec->m_int_curr_symbol);
  delete[] const_cast<char*>(rec->m_currency_symbol);
  delete[] const_cast<char*>(rec->m_mon_grouping);
  delete[] const_cast<char*>(rec->m_positive_sign);
  delete[] const_cast<char*>(rec->m_negative_sign);
  delete rec;
}

// -------------------------------------------------------------------------- 

char const* _Locale_int_curr_symbol(struct _Locale_monetary* rec)
{
  return rec->m_int_curr_symbol;
}

char const* _Locale_currency_symbol(struct _Locale_monetary* rec)
{
  return rec->m_currency_symbol;
}

char _Locale_mon_decimal_point(struct _Locale_monetary* rec)
{
  return rec->m_mon_decimal_point;
}

char _Locale_mon_thousands_sep(struct _Locale_monetary* rec)
{
  return rec->m_mon_thousands_sep;
}

char const* _Locale_mon_grouping(struct _Locale_monetary* rec)
{
  return rec->m_mon_grouping;
}

char const* _Locale_positive_sign(struct _Locale_monetary* rec)
{
  return rec->m_positive_sign;
}

char const* _Locale_negative_sign(struct _Locale_monetary* rec)
{
  return rec->m_negative_sign;
}

char _Locale_int_frac_digits(struct _Locale_monetary* rec)
{
  return rec->m_int_frac_digits;
}

char _Locale_frac_digits(struct _Locale_monetary* rec)
{
  return rec->m_frac_digits;
}

int _Locale_p_cs_precedes(struct _Locale_monetary* rec)
{
  return rec->m_p_cs_precedes;
}

int _Locale_p_sep_by_space(struct _Locale_monetary* rec)
{
  return rec->m_p_sep_by_space;
}

int _Locale_p_sign_pos(struct _Locale_monetary* rec)
{
  return rec->m_p_sign_pos;
}

int _Locale_n_cs_precedes(struct _Locale_monetary* rec)
{
  return rec->m_n_cs_precedes;
}

int _Locale_n_sep_by_space(struct _Locale_monetary* rec)
{
  return rec->m_n_sep_by_space;
}

int _Locale_n_sign_pos(struct _Locale_monetary* rec)
{
  return rec->m_n_sign_pos;
}

int _Locale_p_sign_posn(struct _Locale_monetary* rec)
{
  return rec->m_p_sign_posn;
}

int _Locale_n_sign_posn(struct _Locale_monetary* rec)
{
  return rec->m_n_sign_posn;
}
