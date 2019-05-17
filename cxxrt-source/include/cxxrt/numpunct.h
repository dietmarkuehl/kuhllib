// -*-C++-*- cxxrt/numpunct.h
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
// Title:   Declarations for the numpunct locale
// Version: $Id: numpunct.h,v 1.1.1.1 2002/06/05 01:02:00 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_NUMPUNCT_H__)
#define _CXXRT_NUMPUNCT_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/bstr_fwd.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------

template <class _CS_cT>
class numpunct: public locale::facet
{
public:
  typedef _CS_cT               char_type;
  typedef basic_string<_CS_cT> string_type;

  _CXXRT_EXPLICIT numpunct(size_t = 0);

  char_type decimal_point() const { return do_decimal_point(); }
  char_type thousands_sep() const { return do_thousands_sep(); }
  string grouping() const;

  string_type truename() const { return do_truename(); }
  string_type falsename() const { return do_falsename(); }

  static locale::id id;

  // This stuff should probably be moved to the cache facet: It is
  // actually caching some stuff and also this would provide cleaner
  // initialization.
  void _CS_preprocess();
  bool _CS_is_preprocessed() const { return _CS_boolnames != 0; }
  _CS_cT const* _CS_common_begin() const { return _CS_boolnames; }
  _CS_cT const* _CS_common_end() const { return _CS_c_end; }
  _CS_cT const* _CS_true_begin() const { return _CS_c_end; }
  _CS_cT const* _CS_true_end() const { return _CS_t_end; }
  _CS_cT const* _CS_false_begin() const { return _CS_t_end; }
  _CS_cT const* _CS_false_end() const { return _CS_f_end; }

protected:
  ~numpunct();

  virtual char_type do_decimal_point() const;
  virtual char_type do_thousands_sep() const;
  virtual string do_grouping() const;
  virtual string_type do_truename() const;
  virtual string_type do_falsename() const;

private:
  numpunct(numpunct const&);       // deliberatly not accessible
  void operator=(numpunct const&); // deliberatly not accessible

  _CS_cT const* _CS_boolnames;
  _CS_cT const* _CS_c_end;
  _CS_cT const* _CS_t_end;
  _CS_cT const* _CS_f_end;
};

// --------------------------------------------------------------------------
// specializations of the virtual functions for numpunct<char> and numpunct<wchar_t>

template <> numpunct<char>::char_type   numpunct<char>::do_decimal_point() const;
template <> numpunct<char>::char_type   numpunct<char>::do_thousands_sep() const;
template <> numpunct<char>::string_type numpunct<char>::do_truename() const;
template <> numpunct<char>::string_type numpunct<char>::do_falsename() const;

template <> numpunct<wchar_t>::char_type   numpunct<wchar_t>::do_decimal_point() const;
template <> numpunct<wchar_t>::char_type   numpunct<wchar_t>::do_thousands_sep() const;
template <> numpunct<wchar_t>::string_type numpunct<wchar_t>::do_truename() const;
template <> numpunct<wchar_t>::string_type numpunct<wchar_t>::do_falsename() const;

// --------------------------------------------------------------------------

template <class _CS_cT>
class numpunct_byname: public numpunct<_CS_cT>
{
public:
  typedef typename numpunct<_CS_cT>::char_type   char_type;
  typedef typename numpunct<_CS_cT>::string_type string_type;

  _CXXRT_EXPLICIT numpunct_byname(char const*, size_t = 0);

protected:
  virtual char_type do_decimal_point() const;
  virtual char_type do_thousands_sep() const;
  virtual basic_string<char> do_grouping() const;
  virtual string_type do_truename() const;
  virtual string_type do_falsename() const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<numpunct<char> >(locale const&) 
{
  return true;
}

template <>
inline numpunct<char> const& use_facet<numpunct<char> >(locale const& _CS_l)
{
  locale::facet const* _CS_tmp = _CS_l._CS_m_rep->_CS_find(numpunct<char>::id);
#if defined(_CXXRT_USE_FACET_INSTALLS)
  if (_CS_tmp == 0)
    {
      numpunct<char>* _CS_np = new numpunct<char>;
      _CS_np->_CS_preprocess();
      _CS_l._CS_m_rep->_CS_add(_CS_np);
      return *_CS_np;
    }
#endif
  return static_cast<numpunct<char> const&>(*_CS_tmp);
}

template <>
inline bool has_facet<numpunct<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline numpunct<wchar_t> const& use_facet<numpunct<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<numpunct<wchar_t> >(_CS_l);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_NUMPUNCT_CC__)
#  include <srccxx/numpunct.cc>
#endif

#endif /* _CXXRT_NUMPUNCT_H__ */
