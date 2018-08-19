// -*-C++-*- srcxx/ctype.cc
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
// Title:   Implementations for the ctype category (22.2.1)
// Version: $Id: ctype.cc,v 1.1.1.1 2002/06/05 01:02:29 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_CTYPE_CC__)
#define __SRCCXX_CTYPE_CC__ 1

#if !defined(_CXXRT_CTYPE_H__)
#  include <cxxrt/ctype.h>
#endif
#if !defined(__CSTDLIB__)
#  include <algorithm>
#endif
#if !defined(__CWCHAR__)
#  include <cwchar>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_charT>
locale::id ctype<_CS_charT>::id;

// --------------------------------------------------------------------------

template <class _CS_cT>
bool
ctype<_CS_cT>::_CS_compare(_CS_ctype_info<_CS_cT> const& _CS_i1,
                                       _CS_ctype_info<_CS_cT> const& _CS_i2)
{
  return _CS_i1._CS_code < _CS_i2._CS_code;
}

template <class _CS_cT>
struct _CS_compare_t
{
  bool operator()(_CS_ctype_info<_CS_cT> const& _CS_v1, _CS_ctype_info<_CS_cT> const& _CS_v2) const
    {
      return _CS_v1._CS_code < _CS_v2._CS_code;
    }
};

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT>
ctype<_CS_cT>::ctype(ctype const&)
{
}

template <class _CS_cT>
void
ctype<_CS_cT>::operator= (ctype const&)
{
}
#endif

template <class _CS_cT>
ctype<_CS_cT>::ctype(size_t _CS_refs):
  locale::facet(_CS_refs),
  _CS_m_info_begin(_CS_ctype_info<_CS_cT>::_CS_info),
  _CS_m_info_end(_CS_ctype_info<_CS_cT>::_CS_info + _CS_ctype_info<_CS_cT>::_CS_size - 1),
  _CS_m_owner(false)
{
  sort(_CS_m_info_begin, _CS_m_info_end, _CS_compare_t<_CS_cT>());
}


template <class _CS_cT>
ctype<_CS_cT>::ctype(_CS_ctype_info<_CS_cT>* _CS_info,
         size_t _CS_size,
         bool _CS_owner,
         size_t _CS_refs):
  locale::facet(_CS_refs),
  _CS_m_info_begin(_CS_info),
  _CS_m_info_end(_CS_info + _CS_size - 1),
  _CS_m_owner(_CS_owner)
{
  sort(_CS_m_info_begin, _CS_m_info_end, _CS_compare_t<_CS_cT>());
}

template <class _CS_cT>
ctype<_CS_cT>::~ctype()
{
  if (_CS_m_owner)
    delete[] const_cast<_CS_ctype_info<_CS_cT>*>(_CS_m_info_begin);
}

// --------------------------------------------------------------------------

template <class _CS_cT>
inline _CS_ctype_info<_CS_cT> const*
ctype<_CS_cT>::_CS_lookup(_CS_cT _CS_c) const
{
  _CS_ctype_info<_CS_cT> _CS_info;
  _CS_info._CS_code = _CS_c;
  _CS_ctype_info<_CS_cT> const* _CS_rc = lower_bound(_CS_m_info_begin, _CS_m_info_end, _CS_info, _CS_compare_t<_CS_cT>());
  return (_CS_rc == _CS_m_info_end || _CS_rc->_CS_code != _CS_c)? _CS_m_info_end: _CS_rc;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
bool
ctype<_CS_cT>::is(ctype_base::mask _CS_mask, _CS_cT _CS_c) const
{
  return do_is(_CS_mask, _CS_c);
}

template <class _CS_cT>
bool
ctype<_CS_cT>::do_is(ctype_base::mask _CS_m, _CS_cT _CS_c) const
{
  return (_CS_lookup(_CS_c)->_CS_mask & _CS_m) != 0;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::is(_CS_cT const* _CS_beg, _CS_cT const* _CS_end,
                  ctype_base::mask* _CS_vec) const
{
  return do_is(_CS_beg, _CS_end, _CS_vec);
}

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::do_is(_CS_cT const* _CS_beg, _CS_cT const* _CS_end,
                     ctype_base::mask* _CS_vec) const
{
  for (; _CS_beg != _CS_end; ++_CS_beg, ++_CS_vec)
    *_CS_vec = static_cast<mask>(_CS_lookup(*_CS_beg)->_CS_mask & ~_Locale_HAS_NARROW);
  return _CS_beg;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::scan_is(ctype_base::mask _CS_mask,
                             _CS_cT const* _CS_beg,
                             _CS_cT const* _CS_end) const
{
  return do_scan_is(_CS_mask, _CS_beg, _CS_end);
}

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::do_scan_is(ctype_base::mask _CS_mask,
                                _CS_cT const* _CS_beg,
                                _CS_cT const* _CS_end) const
{
  while (_CS_beg != _CS_end
         && (_CS_lookup(*_CS_beg)->_CS_mask & _CS_mask) == 0)
    ++_CS_beg;
  return _CS_beg;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::scan_not(ctype_base::mask _CS_mask,
                             _CS_cT const* _CS_beg,
                             _CS_cT const* _CS_end) const
{
  return do_scan_not(_CS_mask, _CS_beg, _CS_end);
}

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::do_scan_not(ctype_base::mask _CS_mask,
                                 _CS_cT const* _CS_beg,
                                 _CS_cT const* _CS_end) const
{
  while (_CS_beg != _CS_end
         && (_CS_lookup(*_CS_beg)->_CS_mask & _CS_mask) != 0)
    ++_CS_beg;
  return _CS_beg;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT
ctype<_CS_cT>::toupper(_CS_cT _CS_c) const
{
  return do_toupper(_CS_c);
}

template <class _CS_cT>
_CS_cT
ctype<_CS_cT>::do_toupper(_CS_cT _CS_c) const
{
  _CS_ctype_info<_CS_cT> const* _CS_tmp = _CS_lookup(_CS_c);
  if ((_CS_tmp->_CS_mask & lower) != 0)
    return _CS_tmp->_CS_convert;
  return _CS_c;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::toupper(_CS_cT* _CS_beg, _CS_cT const* _CS_end) const
{
  return do_toupper(_CS_beg, _CS_end);
}

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::do_toupper(_CS_cT* _CS_beg, _CS_cT const* _CS_end) const
{
  for (; _CS_beg != _CS_end; ++_CS_beg)
    {
      _CS_ctype_info<_CS_cT> const* _CS_tmp = _CS_lookup(*_CS_beg);
      if ((_CS_tmp->_CS_mask & lower) != 0)
        *_CS_beg = _CS_tmp->_CS_convert;
    }
  return _CS_beg;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT
ctype<_CS_cT>::tolower(_CS_cT _CS_c) const
{
  return do_tolower(_CS_c);
}

template <class _CS_cT>
_CS_cT
ctype<_CS_cT>::do_tolower(_CS_cT _CS_c) const
{
  _CS_ctype_info<_CS_cT> const* _CS_tmp = _CS_lookup(_CS_c);
  if ((_CS_tmp->_CS_mask & upper) != 0)
    return _CS_tmp->_CS_convert;
  return _CS_c;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::tolower(_CS_cT* _CS_beg, _CS_cT const* _CS_end) const
{
  return do_tolower(_CS_beg, _CS_end);
}

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::do_tolower(_CS_cT* _CS_beg, _CS_cT const* _CS_end) const
{
  for (; _CS_beg != _CS_end; ++_CS_beg)
    {
      _CS_ctype_info<_CS_cT> const* _CS_tmp = _CS_lookup(*_CS_beg);
      if ((_CS_tmp->_CS_mask & upper) != 0)
        *_CS_beg = _CS_tmp->_CS_convert;
    }
  return _CS_beg;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT
ctype<_CS_cT>::widen(char _CS_c) const
{
  return do_widen(_CS_c);
}

template <class _CS_cT>
_CS_cT
ctype<_CS_cT>::do_widen(char _CS_c) const
{
  return _CS_cT(_CS_c);
}

// --------------------------------------------------------------------------

template <class _CS_cT>
char const*
ctype<_CS_cT>::widen(char const* _CS_beg,
                           char const* _CS_end,
                           _CS_cT* _CS_to) const
{
  return do_widen(_CS_beg, _CS_end, _CS_to);
}

template <class _CS_cT>
char const*
ctype<_CS_cT>::do_widen(char const* _CS_beg,
                              char const* _CS_end,
                              _CS_cT* _CS_to) const
{
  for (; _CS_beg != _CS_end; ++_CS_beg, ++_CS_to)
    *_CS_to = _CS_cT(*_CS_beg);
  return _CS_beg;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
char
ctype<_CS_cT>::narrow(_CS_cT _CS_c, char _CS_def) const
{
  return do_narrow(_CS_c, _CS_def);
}

template <class _CS_cT>
char
ctype<_CS_cT>::do_narrow(_CS_cT _CS_c, char _CS_def) const
{
  _CS_ctype_info<_CS_cT> const* _CS_tmp = _CS_lookup(_CS_c);
  if ((_CS_tmp->_CS_mask & _Locale_HAS_NARROW) != 0)
    return _CS_tmp->_CS_narrow;
  return _CS_def;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::narrow(_CS_cT const* _CS_beg,
                            _CS_cT const* _CS_end,
                            char _CS_def,
                            char* _CS_to) const
{
  return do_narrow(_CS_beg, _CS_end, _CS_def, _CS_to);
}

template <class _CS_cT>
_CS_cT const*
ctype<_CS_cT>::do_narrow(_CS_cT const* _CS_beg,
                               _CS_cT const* _CS_end,
                               char _CS_def,
                               char* _CS_to) const
{
  for (; _CS_beg != _CS_end; ++_CS_beg, ++_CS_to)
    {
      _CS_ctype_info<_CS_cT> const* _CS_tmp = _CS_lookup(*_CS_beg);
      if ((_CS_tmp->_CS_mask & _Locale_HAS_NARROW) != 0)
        *_CS_to = _CS_tmp->_CS_narrow;
      else
        *_CS_to = _CS_def;
    }
  return _CS_beg;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_CTYPE_CC__ */
