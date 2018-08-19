// -*-C++-*- cxxrt/ctype.h
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
// Title:   Declarations for the ctype category (22.2.1)
// Version: $Id: ctype.h,v 1.1.1.1 2002/06/05 01:01:45 dietmar_kuehl Exp $
// -------------------------------------------------------------------------

#if !defined(_CXXRT_CTYPE_H__)
#define _CXXRT_CTYPE_H__

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(_CXXRT_CTYPE_BASE_H__)
#  include <cxxrt/ctype_base.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(_CXXRT_C_LOCDEF_H__)
#  include <cxxrt/c_locdef.h>
#endif
#if !defined(__CCTYPE__)
#  include <cctype>
#endif

// -------------------------------------------------------------------------

_Locale_mask_t const _Locale_HAS_NARROW = 1 << 15;

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT>
struct _CS_ctype_info
{
  static _CS_ctype_info<_CS_cT> _CS_info[];
  static size_t                 _CS_size;
  _CS_cT                        _CS_code;    // the code of the described character
  _CS_cT                        _CS_convert; // lower/upper equivalent (depending on mask)
  _Locale_mask_t                _CS_mask;    // character attribute mask (including "has narrow")
  char                          _CS_narrow;  // narrow character equivalent if "has narrow" is set
};

// -------------------------------------------------------------------------
// --- Template class ctype (22.2.1.1) -------------------------------------
// -------------------------------------------------------------------------

template <class _CS_cT>
class ctype: public locale::facet, public ctype_base
{
public:
  typedef _CS_cT char_type;

  _CXXRT_EXPLICIT ctype(size_t = 0);
  // NOTE: the last element in the array has to contain a dummy
  // element with a zero mask. This is used to avoid a conditional in
  // the 'do_*()' functions.
  ctype(_CS_ctype_info<_CS_cT>*, size_t, bool, size_t = 0);

  bool is(mask, _CS_cT) const;
  _CS_cT const* is(_CS_cT const*, _CS_cT const*, mask*) const;
  _CS_cT const* scan_is(mask, _CS_cT const*, _CS_cT const*) const;
  _CS_cT const* scan_not(mask, _CS_cT const*, _CS_cT const*) const;

  _CS_cT        toupper(_CS_cT) const;
  _CS_cT const* toupper(_CS_cT*, _CS_cT const*) const;
  _CS_cT        tolower(_CS_cT) const;
  _CS_cT const* tolower(_CS_cT*, _CS_cT const*) const;
  _CS_cT        widen(char) const;
  char const*         widen(char const*, char const*, _CS_cT*) const;
  char                narrow(_CS_cT, char) const;
  _CS_cT const* narrow(_CS_cT const*, _CS_cT const*, char, char*) const;

  static locale::id id;

protected:
  ~ctype();

  virtual bool do_is(mask, _CS_cT) const;
  virtual _CS_cT const* do_is(_CS_cT const*, _CS_cT const*, mask*) const;
  virtual _CS_cT const* do_scan_is(mask, _CS_cT const*, _CS_cT const*) const;
  virtual _CS_cT const* do_scan_not(mask, _CS_cT const*, _CS_cT const*) const;

  virtual _CS_cT        do_toupper(_CS_cT) const;
  virtual _CS_cT const* do_toupper(_CS_cT*, _CS_cT const*) const;
  virtual _CS_cT        do_tolower(_CS_cT) const;
  virtual _CS_cT const* do_tolower(_CS_cT*, _CS_cT const*) const;
  virtual _CS_cT        do_widen(char) const;
  virtual char const*         do_widen(char const*, char const*, _CS_cT*) const;
  virtual char                do_narrow(_CS_cT, char) const;
  virtual _CS_cT const* do_narrow(_CS_cT const*, _CS_cT const*, char, char*) const;

private:
  ctype(ctype const&);          // deliberatly not accessible
  void operator=(ctype const&); // deliberatly not accessible

  static bool _CS_compare(_CS_ctype_info<_CS_cT> const&,
                                _CS_ctype_info<_CS_cT> const&);
  _CS_ctype_info<_CS_cT> const* _CS_lookup(_CS_cT) const;

  _CS_ctype_info<_CS_cT>* _CS_m_info_begin;
  _CS_ctype_info<_CS_cT>* _CS_m_info_end;
  bool                    _CS_m_owner;
};

// --- The specialization of the ctype facet for char --------------------

_CXXRT_FULL_SPECIALIZATION
class ctype<char>: public locale::facet, public ctype_base
{
  friend class locale;

private:
  ctype(ctype const&);              // inaccessible
  ctype& operator= (ctype const& ); // inaccessible

public:
  typedef char char_type;

  _CXXRT_EXPLICIT ctype(mask const* = 0, bool = false, size_t = 0);

  bool is(mask _CS_m, char _CS_c) const
    {
      return (_CS_m_tab[static_cast<unsigned char>(_CS_c)] & _CS_m) != 0;
    }

  char const* is(char const*, char const*, mask*) const;
  char const* scan_is(mask, char const*, char const*) const;
  char const* scan_not(mask, char const*, char const*) const;

  char        toupper(char _CS_c) const { return do_toupper(_CS_c); }
  char const* toupper(char*, char const*) const;
  char        tolower(char _CS_c) const { return do_tolower(_CS_c); }
  char const* tolower(char*, char const*) const;

  char        widen(char _CS_c) const { return do_widen(_CS_c); }
  char const* widen(char const*, char const*, char*) const;
  char        narrow(char _CS_c, char _CS_d) const { return do_narrow(_CS_c, _CS_d); }
  char const* narrow(char const*, char const*, char, char*) const;

  static locale::id id;
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 0
  enum { table_size = 256 };
#else
  static const size_t table_size = 256;
#endif
  mask const*         _CS_table() const throw() { return _CS_m_tab; }

protected:
  ~ctype();

  mask const*         table() const throw() { return _CS_m_tab; }
  static mask const*  classic_table() throw();

  virtual char        do_toupper(char) const;
  virtual char const* do_toupper(char*, char const*) const;
  virtual char        do_tolower(char) const;
  virtual char const* do_tolower(char*, char const*) const;

  virtual char        do_widen(char) const;
  virtual char const* do_widen(char const*, char const*, char*) const;
  virtual char        do_narrow(char, char) const;
  virtual char const* do_narrow(char const*, char const*, char, char*) const;

private:
  mask const* _CS_m_tab;
  bool        _CS_m_owner;
};

// ----------------------------------------------------------------------------

template <class _CS_cT> class ctype_byname;

_CXXRT_FULL_SPECIALIZATION
class ctype_byname<char>: public ctype<char>
{
public:
  explicit ctype_byname(char const *, size_t = 0);
  ~ctype_byname();
  // --dk: TODO
};

_CXXRT_FULL_SPECIALIZATION
class ctype_byname<wchar_t>: public ctype<wchar_t>
{
public:
  explicit ctype_byname(char const *, size_t = 0);
  ~ctype_byname();
  // --dk: TODO
};

// -------------------------------------------------------------------------

template <>
inline bool has_facet<ctype<char> >(locale const&) 
{
  return true;
}

template <>
inline ctype<char> const& use_facet<ctype<char> >(locale const& _CS_l)
{
  return _CS_use_facet<ctype<char> >(_CS_l);
}

template <>
inline bool has_facet<ctype<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline ctype<wchar_t> const& use_facet<ctype<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<ctype<wchar_t> >(_CS_l);
}

// -------------------------------------------------------------------------
// --- ctype convenience functions (22.1.3) --------------------------------
// -------------------------------------------------------------------------
// --- character classification (22.1.3.1) ---------------------------------

#define _CXXRT_ISFUNCTION(what)                                          \
  template <class _CS_cT> inline                                  \
  bool is ## what(_CS_cT _CS_c, locale const &_CS_l)  \
  {                                                                        \
    return use_facet<ctype<_CS_cT> >( _CS_l)                \
                                       .is(ctype_base::what, _CS_c); \
  }

_CXXRT_ISFUNCTION(space)
_CXXRT_ISFUNCTION(print)
_CXXRT_ISFUNCTION(cntrl)
_CXXRT_ISFUNCTION(upper)
_CXXRT_ISFUNCTION(lower)
_CXXRT_ISFUNCTION(alpha)
_CXXRT_ISFUNCTION(digit)
_CXXRT_ISFUNCTION(punct)
_CXXRT_ISFUNCTION(xdigit)
_CXXRT_ISFUNCTION(alnum)
_CXXRT_ISFUNCTION(graph)

#undef _CXXRT_ISFUNCTION

// --- character conversions (22.1.3.2) ------------------------------------

template <class _CS_cT>
inline _CS_cT tolower(_CS_cT c, locale const &loc)
{
  return use_facet<ctype<_CS_cT> >(loc).tolower(c);
}
template <class _CS_cT>
inline _CS_cT toupper(_CS_cT c, locale const &loc)
{
  return use_facet<ctype<_CS_cT> >(loc).toupper(c);
}

// -------------------------------------------------------------------------
// ... and the version of the auxiliary functions for the character
// type char are just a little bit more special:

#define _CXXRT_ISFUNCTION(_CXXRT_what)                                 \
  template <> inline                                                       \
  bool is##_CXXRT_what<char>(char _CS_c, locale const &_CS_l)\
  {                                                                        \
    ctype<char> const& _CS_ct = use_facet<ctype<char> >(_CS_l);\
    return _CS_ct.is(ctype_base::_CXXRT_what, _CS_c);        \
  }

_CXXRT_ISFUNCTION(space)
_CXXRT_ISFUNCTION(print)
_CXXRT_ISFUNCTION(cntrl)
_CXXRT_ISFUNCTION(upper)
_CXXRT_ISFUNCTION(lower)
_CXXRT_ISFUNCTION(alpha)
_CXXRT_ISFUNCTION(digit)
_CXXRT_ISFUNCTION(punct)
_CXXRT_ISFUNCTION(xdigit)
_CXXRT_ISFUNCTION(alnum)
_CXXRT_ISFUNCTION(graph)

#undef _CXXRT_ISFUNCTION

// -------------------------------------------------------------------------

#if defined(_CXXRT_MIN_IO)
  // extern template class ctype<char>;
  // extern template class ctype_byname<char>;
extern template class ctype<wchar_t>;
  // extern template class ctype_byname<wchar_t>;
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_CTYPE_CC__)
#  include <srccxx/ctype.cc>
#endif

#endif /* _CXXRT_CTYPE_H__ */
