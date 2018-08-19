// -*-C++-*- src/c_numeric.cc
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
// Title:   Definition of the low-level interface to the numeric facet
// Version: $Id: c_num.cpp,v 1.1.1.1 2002/06/05 01:00:23 dietmar_kuehl Exp $ 

// -------------------------------------------------------------------------- 

#include "cxxrt/c_locale.h"
#include "cxxrt/c_lfile.h"
#include <cstring>

// -------------------------------------------------------------------------- 

struct _Locale_numeric
{
  char        m_name[_Locale_MAX_SIMPLE_NAME];
  char const* m_grouping;
  char const* m_truename;
  char const* m_falsename;
  char        m_decimal_point;
  char        m_thousands_sep;
};

// -------------------------------------------------------------------------- 

struct _Locale_numeric* _Locale_numeric_create(char const* name)
{
  _Locale_numeric* rc = new _Locale_numeric;
  _CXXRT_STD strcpy(rc->m_name, name);

  rc->m_grouping  = 0;
  rc->m_truename  = 0;
  rc->m_falsename = 0;

  _Locale_file file;
  _Locale_openfile(&file, name, "numeric");
  
  char dummy;
  file._CS_read(rc->m_decimal_point);
  file._CS_read(dummy);
  file._CS_read(rc->m_thousands_sep);
  file._CS_read(dummy);
  rc->m_truename  = file._CS_read_string();
  rc->m_falsename = file._CS_read_string();
  rc->m_grouping  = file._CS_read_string();

  if (file._CS_failed())
    {
      _Locale_numeric_destroy(rc);
      return 0;
    }

  return rc;
}

char* _Locale_numeric_name(const struct _Locale_numeric* rec, char* buffer)
{
  return _CXXRT_STD strcpy(buffer, rec->m_name);
}

void _Locale_numeric_destroy(struct _Locale_numeric* rec)
{
  delete[] const_cast<char*>(rec->m_grouping);
  delete[] const_cast<char*>(rec->m_truename);
  delete[] const_cast<char*>(rec->m_falsename);
  delete rec;
}

// -------------------------------------------------------------------------- 

char _Locale_decimal_point(struct _Locale_numeric* rec)
{
  return rec->m_decimal_point;
}

char _Locale_thousands_sep(struct _Locale_numeric* rec)
{
  return rec->m_thousands_sep;
}

char const* _Locale_grouping(struct _Locale_numeric* rec)
{
  return rec->m_grouping;
}

char const* _Locale_true(struct _Locale_numeric* rec)
{
  return rec->m_truename;
}

char const* _Locale_false(struct _Locale_numeric* rec)
{
  return rec->m_falsename;
}
