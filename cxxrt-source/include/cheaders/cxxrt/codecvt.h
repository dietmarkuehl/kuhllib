// -*-C++-*- cxxrt/codecvt.h
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
// Title:   Declarations for the code conversion facet
// Version: $Id: codecvt.h,v 1.1.1.1 2002/06/05 01:02:03 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_CODECVT_H__)
#define _CXXRT_CODECVT_H__ 1

#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif 
#if !defined(__CWCHAR__)
#  include <cwchar>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

class codecvt_base
{
public:
  enum result
  {
    ok,
    partial,
    error,
    noconv
  };
};

// --------------------------------------------------------------------------

template <class _CS_internT, class _CS_externT, class _CS_stateT>
class codecvt: public locale::facet, public codecvt_base
{
public:
  static locale::id id;

  typedef _CS_internT intern_type;
  typedef _CS_externT extern_type;
  typedef _CS_stateT  state_type;

  explicit codecvt(size_t = 0);

  result out(state_type&,
             intern_type const*, intern_type const*, intern_type const*&,
             extern_type*, extern_type*, extern_type*&) const;
  result unshift(state_type&,
                 extern_type*, extern_type*, extern_type*&) const;
  result in(state_type&,
            extern_type const*, extern_type const*, extern_type const*&,
            intern_type*, intern_type*, intern_type*&) const;
  int encoding() const throw();
  bool always_noconv() const throw();
  int length(state_type const&, extern_type const*, extern_type const*, size_t) const;
  int max_length() const throw();

protected:
  virtual ~codecvt();

  virtual result do_out(state_type&,
                        intern_type const*, intern_type const*, intern_type const*&,
                        extern_type*, extern_type*, extern_type*&) const;
  virtual result do_unshift(state_type&,
                            extern_type*, extern_type*, extern_type*&) const;
  virtual result do_in(state_type&,
                       extern_type const*, extern_type const*, extern_type const*&,
                       intern_type*, intern_type*, intern_type*&) const;
  virtual int do_encoding() const throw();
  virtual bool do_always_noconv() const throw();
  virtual int do_length(state_type const&,
                        extern_type const*, extern_type const*, size_t max) const;
  virtual int do_max_length() const throw();
};

// --------------------------------------------------------------------------

_CXXRT_FULL_SPECIALIZATION
class codecvt<char, char, mbstate_t>:
  public locale::facet,
  public codecvt_base
{
public:
  static locale::id id;

  typedef char      intern_type;
  typedef char      extern_type;
  typedef mbstate_t state_type;

  explicit codecvt(size_t = 0);

  result out(state_type&,
             intern_type const*, intern_type const*, intern_type const*&,
             extern_type*, extern_type*, extern_type*&) const;
  result unshift(state_type&,
                 extern_type*, extern_type*, extern_type*&) const;
  result in(state_type&,
            extern_type const*, extern_type const*, extern_type const*&,
            intern_type*, intern_type*, intern_type*&) const;
  int encoding() const throw();
  bool always_noconv() const throw();
  int length(state_type const&, extern_type const*, extern_type const*, size_t) const;
  int max_length() const throw();

protected:
  virtual result do_out(state_type&,
                        intern_type const*, intern_type const*, intern_type const*&,
                        extern_type*, extern_type*, extern_type*&) const;
  virtual result do_unshift(state_type&,
                            extern_type*, extern_type*, extern_type*&) const;
  virtual result do_in(state_type&,
                       extern_type const*, extern_type const*, extern_type const*&,
                       intern_type*, intern_type*, intern_type*&) const;
  virtual int do_encoding() const throw();
  virtual bool do_always_noconv() const throw();
  virtual int do_length(state_type const&,
                        extern_type const*, extern_type const*, size_t max) const;
  virtual int do_max_length() const throw();
};

// --------------------------------------------------------------------------

_CXXRT_FULL_SPECIALIZATION
class codecvt<wchar_t, char, mbstate_t>:
  public locale::facet,
  public codecvt_base
{
public:
  static locale::id id;

  typedef wchar_t   intern_type;
  typedef char      extern_type;
  typedef mbstate_t state_type;

  explicit codecvt(size_t = 0);

  result out(state_type&,
             intern_type const*, intern_type const*, intern_type const*&,
             extern_type*, extern_type*, extern_type*&) const;
  result unshift(state_type&,
                 extern_type*, extern_type*, extern_type*&) const;
  result in(state_type&,
            extern_type const*, extern_type const*, extern_type const*&,
            intern_type*, intern_type*, intern_type*&) const;
  int encoding() const throw();
  bool always_noconv() const throw();
  int length(state_type const&, extern_type const*, extern_type const*, size_t) const;
  int max_length() const throw();

protected:
  virtual result do_out(state_type&,
                        intern_type const*, intern_type const*, intern_type const*&,
                        extern_type*, extern_type*, extern_type*&) const;
  virtual result do_unshift(state_type&,
                            extern_type*, extern_type*, extern_type*&) const;
  virtual result do_in(state_type&,
                       extern_type const*, extern_type const*, extern_type const*&,
                       intern_type*, intern_type*, intern_type*&) const;
  virtual int do_encoding() const throw();
  virtual bool do_always_noconv() const throw();
  virtual int do_length(state_type const&,
                        extern_type const*, extern_type const*, size_t max) const;
  virtual int do_max_length() const throw();
};

// --------------------------------------------------------------------------

template <class _CS_codecvt>
struct _CS_external_size
{
  enum { _CS_size = sizeof(typename _CS_codecvt::extern_type) };
};

template <>
struct _CS_external_size<codecvt<wchar_t, char, mbstate_t> >
{
  enum { _CS_size = sizeof(wchar_t) };
};

// --------------------------------------------------------------------------

template <class _CS_Intern, class _CS_Extern, class _CS_State>
class codecvt_byname: public codecvt<_CS_Intern, _CS_Extern, _CS_State>
{
  //-dk:TODO
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<codecvt<char, char, mbstate_t> >(locale const&) 
{
  return true;
}

template <>
inline codecvt<char, char, mbstate_t> const& use_facet<codecvt<char, char, mbstate_t> >(locale const& _CS_l)
{
  return _CS_use_facet<codecvt<char, char, mbstate_t> >(_CS_l);
}

template <>
inline bool has_facet<codecvt<wchar_t, char, mbstate_t> >(locale const&) 
{
  return true;
}
template <>
inline codecvt<wchar_t, char, mbstate_t> const& use_facet<codecvt<wchar_t, char, mbstate_t> >(locale const& _CS_l)
{
  return _CS_use_facet<codecvt<wchar_t, char, mbstate_t> >(_CS_l);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_CODECVT_CC__)
#  include <srccxx/codecvt.cc>
#endif

#endif /* _CXXRT_CODECVT_H__ */
