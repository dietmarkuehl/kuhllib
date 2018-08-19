// -*-C++-*- cxxrt/sbuf_it.h
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
// Title:   Forward declaration of basic_streambuf (and some relatives) 
// Version: $Id: sbuf_it.h,v 1.4 2003/04/13 19:36:00 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_SBUF_IT_H__)
#define _CXXRT_SBUF_IT_H__ 1

#if !defined(_CXXRT_SBIT_FWD_H__)
# include <cxxrt/sbit_fwd.h>
#endif
#if !defined(_CXXRT_IOSFWD_H__)
# include <cxxrt/iosfwd.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
# include <cxxrt/ctraits.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
# include <cxxrt/streambuf.h>
#endif
#if 0
#if !defined(_CXXRT_ITERATOR_H__)
# include <cxxrt/iterator.h>
#endif
#else
# include <stl_iterator_base.h>
#endif
#if !defined(_CXXRT_ITERHELP_H_)
#  include <cxxrt/iterhelp.h>
#endif
#include <iostream>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
class istreambuf_iterator
  : public iterator<input_iterator_tag, _CS_cT, typename _CS_traits::off_type, _CS_cT*, _CS_cT&>
{
public:
  typedef _CS_cT                              char_type;
  typedef _CS_traits                          traits_type;
  typedef typename _CS_traits::int_type       int_type;
  typedef basic_streambuf<_CS_cT, _CS_traits> streambuf_type;
  typedef basic_istream<_CS_cT, _CS_traits>   istream_type;

  istreambuf_iterator() throw();
  istreambuf_iterator(streambuf_type*) throw();
  istreambuf_iterator(istream_type&) throw();

  istreambuf_iterator(istreambuf_iterator const&);
  istreambuf_iterator& operator= (istreambuf_iterator const&);
  ~istreambuf_iterator();

  _CS_cT operator*() const;
  istreambuf_iterator& operator++();
  struct proxy
  {
    proxy(typename traits_type::int_type _CS_c): _CS_m_c(_CS_c) {}
    char_type operator* () const { return traits_type::to_char_type(_CS_m_c); }
    typename traits_type::int_type _CS_m_c;
  };
  proxy operator++(int);

  bool equal(istreambuf_iterator const&) const;

  // --- non-standard function used to get the streambuf:
  streambuf_type* _CS_sbuf() const { return _CS_m_sbuf; }

private:
  streambuf_type*                        _CS_m_sbuf;
};

template <class _CS_cT, class _CS_traits>
bool operator== (istreambuf_iterator<_CS_cT, _CS_traits> const&,
                 istreambuf_iterator<_CS_cT, _CS_traits> const&);
template <class _CS_cT, class _CS_traits>
bool operator!= (istreambuf_iterator<_CS_cT, _CS_traits> const&,
                 istreambuf_iterator<_CS_cT, _CS_traits> const&);

// --------------------------------------------------------------------------

#if 0
template <>
class istreambuf_iterator<char, char_traits<char> >
  : public iterator<input_iterator_tag, char, char_traits<char>::off_type, char*, char&>
{
public:
  typedef char                               char_type;
  typedef char_traits<char>                  traits_type;
  typedef traits_type::int_type              int_type;
  typedef basic_streambuf<char, traits_type> streambuf_type;
  typedef basic_istream<char, traits_type>   istream_type;

  istreambuf_iterator() throw(): _CS_m_sbuf(0), _CS_m_ok(false) {}
  istreambuf_iterator(streambuf_type* _CS_sb) throw(): _CS_m_sbuf(_CS_sb), _CS_m_ok(true) {
    if (_CS_m_sbuf == 0 || traits_type::eq_int_type(traits_type::eof(), _CS_m_sbuf->sgetc()))
      _CS_m_ok = false;
  }
  istreambuf_iterator(istream_type& _CS_stream) throw();

  char operator*() const { return *_CS_m_sbuf->_CS_begin(); }
  istreambuf_iterator& operator++() {
    if (_CS_m_sbuf->_CS_gnext() == _CS_m_sbuf->_CS_end())
      if (traits_type::eq_int_type(traits_type::eof(), _CS_m_sbuf->_CS_underflow()))
        _CS_m_ok = false;
    return *this;
  }
  struct proxy
  {
    proxy(traits_type::int_type _CS_c): _CS_m_c(_CS_c) {}
    char_type operator* () const { return traits_type::to_char_type(_CS_m_c); }
    traits_type::int_type _CS_m_c;
  };
  proxy operator++(int) { int_type _CS_rc = operator*(); operator++(); return _CS_rc; }

  bool equal(istreambuf_iterator const& _CS_it) const {
    return _CS_m_ok == _CS_it._CS_m_ok;
  }

  // --- non-standard function used to get the streambuf:
  streambuf_type* _CS_sbuf() const { return _CS_m_sbuf; }

private:
  streambuf_type* _CS_m_sbuf;
  bool            _CS_m_ok;
};

template <>
inline bool operator== (istreambuf_iterator<char, char_traits<char> > const& _CS_it1,
                 istreambuf_iterator<char, char_traits<char> > const& _CS_it2)
{
  return _CS_it1.equal(_CS_it2);
}
template <>
inline bool operator!= (istreambuf_iterator<char, char_traits<char> > const& _CS_it1,
                 istreambuf_iterator<char, char_traits<char> > const& _CS_it2)
{
  return !_CS_it1.equal(_CS_it2);
}
#endif

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
class ostreambuf_iterator
  : public iterator<output_iterator_tag, void, void, void, void>
{
public:
  typedef _CS_cT                              char_type;
  typedef _CS_traits                          traits_type;
  typedef typename _CS_traits::int_type       int_type;
  typedef basic_streambuf<_CS_cT, _CS_traits> streambuf_type;
  typedef basic_ostream<_CS_cT, _CS_traits>   ostream_type;

  ostreambuf_iterator(streambuf_type*) throw();
  ostreambuf_iterator(ostream_type&) throw();

  ostreambuf_iterator(ostreambuf_iterator const& _CS_sb) throw():
    _CS_m_sbuf(_CS_sb._CS_m_sbuf),
    _CS_fail(_CS_sb._CS_fail)
    {
    }
  ostreambuf_iterator& operator= (ostreambuf_iterator const&) throw();
  ~ostreambuf_iterator() throw();

  ostreambuf_iterator& operator*() throw();
  ostreambuf_iterator& operator= (_CS_cT);

  ostreambuf_iterator& operator++() throw();
  ostreambuf_iterator  operator++(int) throw();

  bool failed() const throw();

  // --- non-standard function used to get the streambuf:
  streambuf_type* _CS_sbuf() const { return _CS_m_sbuf; }

private:
  streambuf_type* _CS_m_sbuf;
  bool            _CS_fail;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

// --------------------------------------------------------------------------

namespace _Cxxrt_algorithm
{
  template <class _CS_cT, class _CS_traits>
  class _Cxxrt_empty_streambuf:
    public _CXXRT_STD basic_streambuf<_CS_cT, _CS_traits>
  {
  };

  template <class _CS_cT, class _CS_traits>
  _CXXRT_STD basic_streambuf<_CS_cT, _CS_traits>*
  _Cxxrt_get_empty_streambuf()
  {
    static _Cxxrt_empty_streambuf<_CS_cT, _CS_traits> _CS_rc;
    return &_CS_rc;
  }

  template <class _CS_cT, class _CS_traits>
  class _Cxxrt_iterhelper<
  _CXXRT_STD istreambuf_iterator<_CS_cT, _CS_traits> >
  {
  public:
    typedef _Cxxrt_bool<true> _Cxxrt_fundamental;
    typedef _CS_cT*           iterator;
    
    _Cxxrt_iterhelper(_CXXRT_STD istreambuf_iterator<_CS_cT, _CS_traits> _CS_beg,
                      _CXXRT_STD istreambuf_iterator<_CS_cT, _CS_traits> _CS_end):
      _CS_m_sbuf(_CS_beg == _CS_end? _Cxxrt_get_empty_streambuf<_CS_cT, _CS_traits>(): _CS_beg._CS_sbuf())
    {
      typename _CXXRT_STD basic_streambuf<_CS_cT, _CS_traits>::int_type _CS_c = _CS_m_sbuf->sgetc();
      if (_CS_traits::eq_int_type(_CS_traits::eof(), _CS_c))
        _CS_m_beg = _CS_m_end = 0;
      else
        if (_CS_m_sbuf->_CS_begin() != _CS_m_sbuf->_CS_end())
          {
            _CS_m_beg = _CS_m_sbuf->_CS_begin();
            _CS_m_end = _CS_m_sbuf->_CS_end();
          }
        else
          {
            _CS_traits::assign(_CS_m_buf, _CS_traits::to_char_type(_CS_c));
            _CS_m_end = (_CS_m_beg = &_CS_m_buf) + 1;
          }
    }

    operator void const*() const { return _CS_m_beg == _CS_m_end? 0: this; }
    _Cxxrt_iterhelper& operator++()
    {
      _CS_m_sbuf->_CS_commit(_CS_m_sbuf->_CS_end());

      typename _CXXRT_STD basic_streambuf<_CS_cT, _CS_traits>::int_type _CS_c = _CS_m_sbuf->sgetc();

      if (_CS_traits::eq_int_type(_CS_traits::eof(), _CS_c))
        _CS_m_beg = _CS_m_end = 0;
      else
        if (_CS_m_sbuf->_CS_begin() != _CS_m_sbuf->_CS_end())
          {
            _CS_m_beg = _CS_m_sbuf->_CS_begin();
            _CS_m_end = _CS_m_sbuf->_CS_end();
          }
        else
          {
            _CS_traits::assign(_CS_m_buf, _CS_traits::to_char_type(_CS_c));
            _CS_m_end = (_CS_m_beg = &_CS_m_buf) + 1;
          }

      return *this;
    }
    iterator begin() { return _CS_m_beg; }
    iterator end()   { return _CS_m_end; }
    _CXXRT_STD istreambuf_iterator<_CS_cT, _CS_traits> _Cxxrt_done(iterator _CS_it)
    {
      if (_CS_m_sbuf->_CS_begin() != _CS_m_sbuf->_CS_end())
        _CS_m_sbuf->_CS_commit(_CS_it);
      return _CXXRT_STD istreambuf_iterator<_CS_cT, _CS_traits>(_CS_m_sbuf);
    }

  private:
    _CXXRT_STD basic_streambuf<_CS_cT, _CS_traits>* _CS_m_sbuf;
    _CS_cT        _CS_m_buf;
    iterator _CS_m_beg;
    iterator _CS_m_end;
  };
}

// --------------------------------------------------------------------------

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_ISBUF_IT_CC__)
#  include <srccxx/isbuf_it.cc>
#endif
#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_OSBUF_IT_CC__)
#  include <srccxx/osbuf_it.cc>
#endif

#endif /* _CXXRT_SBUF_IT_H__ */
