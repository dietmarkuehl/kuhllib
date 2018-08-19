// -*-C++-*- src/locale_n.cc
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
// Title:   Locale members dealing with named entities
// Version: $Id: locale_n.cpp,v 1.1.1.1 2002/06/05 01:00:43 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <locale>
#include <cxxrt/c_locale.h>
#if defined(__EDG__)
#  include <stdexcept.h>
#else
#  include <stdexcept>
#endif
#include <cstring>
#include <string>

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_BEGIN

locale::_CS_intern *
locale::_CS_combine(_CS_intern const *l1,
		    _CS_intern const *l2,
		    locale::category cat)
{
  char buf[_Locale_MAX_COMPOSITE_NAME];
  if ((string(l1->_CS_name()) != string("*")) && (string(l2->_CS_name()) != string("*")))
    {
      string const& n1 = l1->_CS_name();
      string const& n2 = l2->_CS_name();
      string s_ct = _Locale_extract_ctype_name((cat & ctype? n2: n1).c_str(), buf);
      string s_nu = _Locale_extract_numeric_name((cat & numeric? n2: n1).c_str(), buf);
      string s_ti = _Locale_extract_time_name((cat & time? n2: n1).c_str(), buf);
      string s_co = _Locale_extract_collate_name((cat & collate? n2: n1).c_str(), buf);
      string s_mo = _Locale_extract_monetary_name((cat & monetary? n2: n1).c_str(), buf);
      string s_me = _Locale_extract_messages_name((cat & messages? n2: n1).c_str(), buf);
      _Locale_compose_name(buf, s_ct.c_str(), s_nu.c_str(), s_ti.c_str(),
                                s_co.c_str(), s_mo.c_str(), s_me.c_str());
    }
  else
    strcpy(buf, "*");

  _CS_intern *rc = 0;
  rc = new _CS_intern(*l1, buf);

#if !defined(_MSC_VER)
//-dk:TODO port for MSVC++
  if (cat & locale::collate != 0)
    {
      rc->_CS_add(l2->_CS_find<_CXXRT_STD_NAME::collate<char> >());
      rc->_CS_add(l2->_CS_find<_CXXRT_STD_NAME::collate<wchar_t> >());
    }

  if (cat & locale::ctype != 0)
    {
      rc->_CS_add(l2->_CS_find<_CXXRT_STD_NAME ::ctype<char> >());
      rc->_CS_add(l2->_CS_find<codecvt<char, char, mbstate_t> >());
      rc->_CS_add(l2->_CS_find<_CXXRT_STD_NAME ::ctype<wchar_t> >());
      rc->_CS_add(l2->_CS_find<codecvt<wchar_t, char, mbstate_t> >());
    }

  if (cat & locale::monetary != 0)
    {
      rc->_CS_add(l2->_CS_find<moneypunct<char, true> >());
      rc->_CS_add(l2->_CS_find<moneypunct<char, false> >());
      rc->_CS_add(l2->_CS_find<money_get<char, istreambuf_iterator<char> > >());
      rc->_CS_add(l2->_CS_find<money_put<char, ostreambuf_iterator<char> > >());
      rc->_CS_add(l2->_CS_find<moneypunct<wchar_t, true> >());
      rc->_CS_add(l2->_CS_find<moneypunct<wchar_t, false> >());
      rc->_CS_add(l2->_CS_find<money_get<wchar_t, istreambuf_iterator<wchar_t> > >());
      rc->_CS_add(l2->_CS_find<money_put<wchar_t, ostreambuf_iterator<wchar_t> > >());
    }

  if (cat & locale::numeric != 0)
    {
      rc->_CS_add(l2->_CS_find<numpunct<char> >());
      rc->_CS_add(l2->_CS_find<num_get<char, istreambuf_iterator<char> > >());
      rc->_CS_add(l2->_CS_find<num_put<char, ostreambuf_iterator<char> > >());
      rc->_CS_add(l2->_CS_find<numpunct<wchar_t> >());
      rc->_CS_add(l2->_CS_find<num_get<wchar_t, istreambuf_iterator<wchar_t> > >());
      rc->_CS_add(l2->_CS_find<num_put<wchar_t, ostreambuf_iterator<wchar_t> > >());
    }


  if (cat & locale::time != 0)
    {
      //-dk:TODO rc->_CS_add(l2->template _CS_find<time_get<char, istreambuf_iterator<char> > >());
      //-dk:TODO rc->_CS_add(l2->template _CS_find<time_put<char, ostreambuf_iterator<char> > >());
      //-dk:TODO rc->_CS_add(l2->template _CS_find<time_get<wchar_t, istreambuf_iterator<wchar_t> > >());
      //-dk:TODO rc->_CS_add(l2->template _CS_find<time_put<wchar_t, ostreambuf_iterator<wchar_t> > >());
    }

  if (cat & locale::time != 0)
    {
      rc->_CS_add(l2->_CS_find<_CXXRT_STD_NAME ::messages<char> >());
      rc->_CS_add(l2->_CS_find<_CXXRT_STD_NAME ::messages<wchar_t> >());
    }
#endif

  return rc;
}

// -------------------------------------------------------------------------- 

class locale::_CS_invalid_facet_name: public runtime_error
{
public:
  _CS_invalid_facet_name(char const *);
};

locale::_CS_invalid_facet_name::_CS_invalid_facet_name(char const * w)
  : runtime_error(w)
{
}

locale::locale(char const *lname):
  _CS_m_rep(0)
{
  if (!strcmp(lname, "C"))
    {
      _CS_m_rep = locale::classic()._CS_m_rep;
      ++_CS_m_rep->_CS_m_refcount;
    }
  else
    {
      char       buf[_Locale_MAX_SIMPLE_NAME];
      _CS_intern *rep = new _CS_intern(*locale::classic()._CS_m_rep, lname);

      if (_Locale_extract_ctype_name(lname, buf) == 0)
        throw _CS_invalid_facet_name(("no 'ctype' part in '" + string(lname) + "'").c_str());
      rep->_CS_add(new ctype_byname<char>(buf));
      rep->_CS_add(new ctype_byname<wchar_t>(buf));

      if (_Locale_extract_numeric_name(lname, buf) == 0)
        throw _CS_invalid_facet_name(("no 'numeric' part in '" + string(lname) + "'").c_str());
      rep->_CS_add(new numpunct_byname<char>(buf));
      rep->_CS_add(new numpunct_byname<wchar_t>(buf));

      if (_Locale_extract_time_name(lname, buf) == 0)
        throw _CS_invalid_facet_name(("no 'time' part in '" + string(lname) + "'").c_str());
      //-dk:TODO rep->_CS_add(new time_get_byname<char>(buf));
      //-dk:TODO rep->_CS_add(new time_put_byname<char>(buf));
      //-dk:TODO rep->_CS_add(new time_get_byname<wchar_t>(buf));
      //-dk:TODO rep->_CS_add(new time_put_byname<wchar_t>(buf));

      if (_Locale_extract_collate_name(lname, buf) == 0)
        throw _CS_invalid_facet_name(("no 'collate' part in '" + string(lname) + "'").c_str());
      rep->_CS_add(new collate_byname<char>(buf));
      rep->_CS_add(new collate_byname<wchar_t>(buf));

      if (_Locale_extract_monetary_name(lname, buf) == 0)
        throw _CS_invalid_facet_name(("no 'monetary' part in '" + string(lname) + "'").c_str());
      rep->_CS_add(new moneypunct_byname<char, true>(buf));
      rep->_CS_add(new moneypunct_byname<char, false>(buf));
      rep->_CS_add(new moneypunct_byname<wchar_t, true>(buf));
      rep->_CS_add(new moneypunct_byname<wchar_t, false>(buf));

      if (_Locale_extract_messages_name(lname, buf) == 0)
        throw _CS_invalid_facet_name(("no 'messages' part in '" + string(lname) + "'").c_str());
      rep->_CS_add(new messages_byname<char>(buf));
      rep->_CS_add(new messages_byname<wchar_t>(buf));

      _CS_m_rep = rep;
      ++_CS_m_rep->_CS_m_refcount;
    }
}

// -------------------------------------------------------------------------- 

locale::locale(locale const &loc, char const *n, locale::category cat):
  _CS_m_rep(_CS_combine(loc._CS_m_rep, locale(n)._CS_m_rep, cat))
{
  ++_CS_m_rep->_CS_m_refcount;
}

locale::locale(locale const &l1, locale const &l2, locale::category cat):
  _CS_m_rep(_CS_combine(l1._CS_m_rep, l2._CS_m_rep, cat))
{
  ++_CS_m_rep->_CS_m_refcount;
}

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
