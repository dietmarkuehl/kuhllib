// -*-C++-*- srccxx/numpunct.cc
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
// Title:   File with default implementations of numpunct
// Version: $Id: numpunct.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// Note:    Most of the virtual functions are useless without being overridden!
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_NUMPUNCT_CC__)
#define __SRCCXX_NUMPUNCT_CC__ 1

#if !defined(_CXXRT_NUMPUNCT_H__)
#  include <cxxrt/numpunct.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

template <class _CS_cT>
locale::id numpunct<_CS_cT>::id;

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT>
numpunct<_CS_cT>::numpunct(numpunct const&)
{
}

template <class _CS_cT>
void 
numpunct<_CS_cT>::operator= (numpunct const&)
{
}
#endif

template <class _CS_cT>
numpunct<_CS_cT>::numpunct(size_t _CS_refs):
  facet(_CS_refs),
  _CS_boolnames(0),
  _CS_c_end(0),
  _CS_t_end(0),
  _CS_f_end(0)
{
}

template <class _CS_cT>
numpunct<_CS_cT>::~numpunct()
{
  delete[] const_cast<_CS_cT*>(_CS_boolnames);
}

// --------------------------------------------------------------------------

template <class _CS_cT>
string
numpunct<_CS_cT>::grouping() const
{
  return do_grouping();
}

// --------------------------------------------------------------------------

template <class _CS_cT>
typename numpunct<_CS_cT>::char_type
numpunct<_CS_cT>::do_decimal_point() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT>
typename numpunct<_CS_cT>::char_type
numpunct<_CS_cT>::do_thousands_sep() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT>
string
numpunct<_CS_cT>::do_grouping() const
{
  return string();
}

template <class _CS_cT>
typename numpunct<_CS_cT>::string_type
numpunct<_CS_cT>::do_truename() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT>
typename numpunct<_CS_cT>::string_type
numpunct<_CS_cT>::do_falsename() const
{
  return string_type(); //-dk:TODO
}

// --------------------------------------------------------------------------

template <class _CS_cT>
void
numpunct<_CS_cT>::_CS_preprocess()
{
  typedef typename string_type::iterator _CS_iterator;

  string_type _CS_tname = truename();
  string_type _CS_fname = falsename();

  _CS_iterator _CS_t_it = _CS_tname.begin();
  _CS_iterator _CS_f_it = _CS_fname.begin();

  while (_CS_t_it != _CS_tname.end()
         && _CS_f_it != _CS_fname.end()
         && *_CS_t_it == *_CS_f_it)
    {
      ++_CS_t_it;
      ++_CS_f_it;
    }

  typename string_type::size_type _CS_csz = _CS_f_it - _CS_fname.begin();
  typename string_type::size_type _CS_tsz = _CS_tname.end() - _CS_t_it;
  typename string_type::size_type _CS_fsz = _CS_fname.end() - _CS_f_it;

  typedef typename basic_string<_CS_cT>::traits_type traits_type;

  char_type* _CS_t = new _CS_cT[_CS_csz + _CS_tsz + _CS_fsz];
  _CS_boolnames = _CS_t;
  _CS_t = traits_type::copy(_CS_t, _CS_fname.c_str(), _CS_csz);
  _CS_c_end = _CS_t;
  _CS_t = traits_type::copy(_CS_t, _CS_tname.c_str() + _CS_csz, _CS_tsz);
  _CS_t_end = _CS_t;
  _CS_t = traits_type::copy(_CS_t, _CS_fname.c_str() + _CS_csz, _CS_fsz);
  _CS_f_end = _CS_t;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
numpunct_byname<_CS_cT>::numpunct_byname(char const*, size_t _CS_refs):
  numpunct<_CS_cT>(_CS_refs)
{
  //-dk:TODO
}


template <class _CS_cT>
typename numpunct_byname<_CS_cT>::char_type
numpunct_byname<_CS_cT>::do_decimal_point() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT>
typename numpunct_byname<_CS_cT>::char_type
numpunct_byname<_CS_cT>::do_thousands_sep() const
{
  return char_type(); //-dk:TODO
}

template <class _CS_cT>
string
numpunct_byname<_CS_cT>::do_grouping() const
{
  return string();
}

template <class _CS_cT>
typename numpunct_byname<_CS_cT>::string_type
numpunct_byname<_CS_cT>::do_truename() const
{
  return string_type(); //-dk:TODO
}

template <class _CS_cT>
typename numpunct_byname<_CS_cT>::string_type
numpunct_byname<_CS_cT>::do_falsename() const
{
  return string_type(); //-dk:TODO
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_NUMPUNCT_CC__ */
