// <!!-*-C++-*- cxxrt/locale.h --->
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
// Title:   Basic Declaration for the Locales Library
// Version: $Id: locale.h,v 1.3 2003/04/13 19:36:00 dietmar_kuehl Exp $

// locale is actually just a frontend to the representation of a
// locale. The actual representation is _CS_intern: This is a
// reference counted struct containing some form of a map to store the
// known facets.

//------------------------------------------------------------------------

#if !defined(_CXXRT_LOCALE_H__)
#define _CXXRT_LOCALE_H__

#if !defined(_CXXRTCF_H__)
#  include "cxxrtcf.h"
#endif
#if !defined(_CXXRT_BSTR_FWD_H__)
#  include <cxxrt/bstr_fwd.h>
#endif

_CXXRT_NAMESPACE_BEGIN

// -----------------------------------------------------------------------

class locale;
template <class _CS_Facet> _CS_Facet const& use_facet(locale const&);
template <class _CS_cT> class _CS_cache;

void _CS_throw_bad_cast();

// -----------------------------------------------------------------------
// The class locale is basically a container indexed by type (which is
// a very clever idea, IMO; however, it relies on a member called id which
// could be avoided with some additional template operation, I think...).
// However, the container does not provide operations for modifications:
// If a new locales with different facets is needed, it is created as a
// copy of an existing facet with some facets replaced.
// In addition to the container functionality, locale also contains some
// declarations used by the component.

template <class _CS_Facet> _CS_Facet const &use_facet(locale const&);
template <class _CS_Facet> bool has_facet(locale const&);

class locale
{
#if _CXXRT_HAS_FRIEND_TEMPLATES == 1
#if 0
  template <class _CS_Facet> friend struct _CS_use_facet;
#endif
  template <class _CS_Facet> friend _CS_Facet const &use_facet(locale const&);
  template <class _CS_Facet> friend bool has_facet(locale const&);
  template <class _CS_cT> friend class _CS_cache;
#else
#endif
public:
  class _CS_invalid_facet_name;
  class _CS_intern;

  friend _CS_intern* _CS_classic_rep();

public:
  class id
  {
    friend class _CS_intern; // needs access to the internal id

  public:
    id();
    static size_t _CS_init(id&); // for internal use only
  
  private:
    id(id const&);
    void operator= (id const&);

    size_t _CS_identifier;
    static id*      _CS_list; // used for proper initialization
    id*             _CS_next; // used for proper initialization
  };

  class facet
  {
    friend class _CS_intern;

  protected:
    _CXXRT_EXPLICIT facet(size_t = 0);
    virtual ~facet();

  private:
    facet(facet const&);
    void operator= (facet const&);

    void                 _CS_reference() const;
    bool                 _CS_release() const;
    virtual facet const* _CS_object() const;
    virtual facet const* _CS_init(_CS_intern const&) const;

    mutable size_t _CS_ref_count;
  };

  enum category
  {
    none     = 0x00,
    collate  = 0x01,
    ctype    = 0x02,
    monetary = 0x04,
    numeric  = 0x08,
    time     = 0x10,
    messages = 0x20,
    all      = 0x3f
  };
  
  locale() _CXXRT_THROW_SPEC(());
  locale(locale const&) _CXXRT_THROW_SPEC(());
  _CXXRT_EXPLICIT locale(char const*);
  locale(locale const&, char const*, category);

  template<class F> locale(locale const& _CS_o, F* _CS_f):
    _CS_m_rep(_CS_combine(_CS_o, _CS_f, _CS_f->id))
  {
    ++_CS_m_rep->_CS_m_refcount;
  }
  locale(locale const &, locale const &, category);
  ~locale();
  locale const &operator= (locale const&) _CXXRT_THROW_SPEC(());

  template <class Facet> locale combine(locale const&);

  string name() const;

  bool operator== (locale const&) const;
  bool operator!= (locale const&) const;

  template <class _CS_cT, class _CS_Tr, class _CS_alloc>
  bool operator() (basic_string<_CS_cT, _CS_Tr, _CS_alloc> const&,
                   basic_string<_CS_cT, _CS_Tr, _CS_alloc> const&) const;

  static locale global(locale const &);
  static locale const &classic();

private:
#if _CXXRT_HAS_FRIEND_TEMPLATES == 0
#endif
public:
  class _CS_intern
    {
    public:
      size_t _CS_m_refcount;

      explicit _CS_intern(char const*);
      _CS_intern(_CS_intern const&, char const*);
      ~_CS_intern();

      char const* _CS_name() const { return _CS_m_name; }

      template <class _CS_F>
      facet const* _CS_add(_CS_F const*_CS_f)
        {
          return _CS_add(_CS_F::id, _CS_f);
        }
      facet const* _CS_add(id const&, facet const*);
      void _CS_init();
      facet const* _CS_find(id const& _CS_i) const
        {
          if (_CS_m_size <= _CS_i._CS_identifier)
            return 0;
          return _CS_m_array[_CS_i._CS_identifier];
        }
      template <class _CS_F>
        _CS_F const* _CS_find() const
        {
          //-dk:TODO return dynamic_cast<_CS_F const*>(_CS_find(_CS_F::id));
          return static_cast<_CS_F const*>(_CS_find(_CS_F::id));
        }
      template <class _CS_F>
        _CS_F const* _CS_get_facet() const
        {
          return static_cast<_CS_F const*>(_CS_m_array[_CS_F::id._CS_identifier]);
        }

    private:
      _CS_intern(_CS_intern const&); // cannot be used
      void operator= (_CS_intern const&);  // cannot be used

      size_t        _CS_m_size;
      facet const** _CS_m_array;
      char const*   _CS_m_name;
    };

  _CS_intern* _CS_m_rep;

private:
  explicit locale(_CS_intern*);
  static _CS_intern* _CS_combine(locale const&, facet const*, id const&);
  static _CS_intern* _CS_combine(_CS_intern const*, _CS_intern const*, category);

  static _CS_intern*& _CS_global_rep();
};

_CXXRT_BITMASK(locale::category, locale::all)

// -----------------------------------------------------------------------

template <class _CS_Facet>
inline bool has_facet(locale const &_CS_loc)
{
  return _CS_loc._CS_m_rep->_CS_find(_CS_Facet::id) != 0;
}

template <class _CS_Facet>
inline _CS_Facet const& _CS_use_facet(locale const& _CS_l)
{
#if defined(_CXXRT_USE_FACET_INSTALLS)
  locale::facet const* _CS_tmp = _CS_l._CS_m_rep->template _CS_get_facet<_CS_Facet>();
  if (_CS_tmp == 0)
    _CS_tmp = _CS_l._CS_m_rep->_CS_add(new _CS_Facet);
  return static_cast<_CS_Facet const&>(*_CS_tmp);
#else
  // return *static_cast<_CS_Facet const*>(_CS_l._CS_m_rep->_CS_find(_CS_Facet::id));
  return *((_CS_l._CS_m_rep)->template _CS_get_facet<_CS_Facet>());
#endif
}

template <class _CS_Facet>
inline _CS_Facet const& use_facet(locale const& _CS_loc)
{
  locale::facet const* _CS_tmp = _CS_loc._CS_m_rep->_CS_find(_CS_Facet::id);
  if (_CS_tmp == 0)
    _CS_throw_bad_cast();
  return *_CXXRT_DYNAMIC_CAST(_CS_Facet const *, _CS_tmp);
}

// -----------------------------------------------------------------------

template <class _CS_Facet>
inline
locale locale::combine(locale const &_CS_l)
{
  return locale(_CS_combine(*this, &use_facet<_CS_Facet>(_CS_l), _CS_Facet::id));
}

// -----------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_LOCALE_H__ */
