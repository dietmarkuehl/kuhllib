// -*-C++-*- c_locale.cc
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
// Title:   Implementation of the locale abstraction
// Version: $Id: c_locale.cpp,v 1.1.1.1 2002/06/05 01:00:21 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <wchar.h>
#include "cxxrt/c_locale.h"
#include <cstring>
#include <cstdlib>
#include "cxxrt/c_lfile.h"

// -------------------------------------------------------------------------- 

void _Locale_openfile(_Locale_file* lfile, char const* loc, char const* fac)
{
  char buffer[8192];

  _CXXRT_STD strcpy(buffer, "/usr/local/lib/cxxrt/");
  _CXXRT_STD strcat(_CXXRT_STD strcat(buffer, loc), "/");
  _CXXRT_STD strcat(buffer, fac);

  lfile->_CS_open(buffer);
}

// -------------------------------------------------------------------------- 

static char const separator = ';';

static char *extract_part(char const *full, char *part, char const *fname, int count)
{
  if (_CXXRT_STD strcmp(full, "") == 0)
    {
      char const *tmp = _CXXRT_STD getenv(fname);
      if (tmp == 0)
        tmp = _CXXRT_STD getenv("LOCALE");
      return tmp == 0? _CXXRT_STD strcpy(part, "C"): _CXXRT_STD strcpy(part, tmp);
    }
  else if (_CXXRT_STD strchr(full, separator) != 0)
    {
      char const *tmp = full;
      for (;count != 0; --count)
        {
          tmp = _CXXRT_STD strchr(tmp, separator);
          if (tmp == 0)
            return 0;
          tmp = tmp + 1;
        }

      char const *end = _CXXRT_STD strchr(tmp, separator);

      if (end != 0)
        {
          _CXXRT_STD strncpy(part, tmp, end - tmp);
          part[end - tmp] = 0;
          return part;
        }
      else
        return _CXXRT_STD strlen(tmp) == 0? 0: _CXXRT_STD strcpy(part, tmp);
    }
  else
    return _CXXRT_STD strcpy(part, full);

  return part;
}

// -------------------------------------------------------------------------- 

char * _Locale_extract_ctype_name(char const *full, char *part)
{
  return extract_part(full, part, "LC_CTYPE", 0);
}

char * _Locale_extract_numeric_name(char const *full, char *part)
{
  return extract_part(full, part, "LC_NUMERIC", 1);
}

char * _Locale_extract_time_name(char const *full, char *part)
{
  return extract_part(full, part, "LC_TIME", 2);
}

char * _Locale_extract_collate_name(char const *full, char *part)
{
  return extract_part(full, part, "LC_COLLATE", 3);
}

char * _Locale_extract_monetary_name(char const *full, char *part)
{
  return extract_part(full, part, "LC_MONETARY", 4);
}

char * _Locale_extract_messages_name(char const *full, char *part)
{
  return extract_part(full, part, "LC_MESSAGES", 5);
}

// -------------------------------------------------------------------------- 

char *_Locale_compose_name(char *buf,
                           const char *ctype, const char *numeric,
                           const char *time, const char *collate,
                           const char *monetary, const char *messages)
{
  char sep[2] = { separator, 0 };

  _CXXRT_STD strcat(_CXXRT_STD strcpy(buf, ctype), sep);
  _CXXRT_STD strcat(_CXXRT_STD strcat(buf, numeric), sep);
  _CXXRT_STD strcat(_CXXRT_STD strcat(buf, time), sep);
  _CXXRT_STD strcat(_CXXRT_STD strcat(buf, collate), sep);
  _CXXRT_STD strcat(_CXXRT_STD strcat(buf, monetary), sep);
  _CXXRT_STD strcat(buf, messages);

  return buf;
}
