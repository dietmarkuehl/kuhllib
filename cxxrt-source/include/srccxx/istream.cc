// -*-C++-*- srccxx/istream.cc
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
// Title:   Implementation of basic_istream member functions 
// Version: $Id: istream.cc,v 1.4 2006/04/07 07:07:13 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_ISTREAM_CC__)
#define __SRCCXX_ISTREAM_CC__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_ISTREAM_H__)
#  include <cxxrt/istream.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif
#if !defined(_CXXRT_OSTREAM_H__)
#  include <cxxrt/ostream.h>
#endif
#if !defined(_CXXRT_SBUF_IT_H__)
#  include <cxxrt/sbuf_it.h>
#endif
#if !defined(_CXXRT_NUM_GET_H__)
#  include <cxxrt/num_get.h>
#endif
#if !defined(_CXXRT_CTYPE_H__)
#  include <cxxrt/ctype.h>
#endif
#include <limits>
#include <iostream>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
inline bool
_CS_skip_ws(basic_istream<_CS_cT, _CS_traits>& _CS_is)
{
  typedef ctype<_CS_cT> _CS_ctype_t;
  _CS_ctype_t const& _CS_ctype = use_facet<_CS_ctype_t>(_CS_is.getloc());
  basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf = _CS_is.rdbuf();
  typename _CS_traits::int_type _CS_c = _CS_sbuf->sgetc();
  while (!_CS_traits::eq_int_type(_CS_c, _CS_traits::eof())
         && _CS_ctype.is(_CS_ctype.space, _CS_c))
    _CS_c = _CS_sbuf->snextc();
  return _CS_traits::eq_int_type(_CS_c, _CS_traits::eof());
}

template <class _CS_traits>
inline bool
_CS_skip_ws(basic_istream<char, _CS_traits>& _CS_is)
{
  ctype_base::mask const* _CS_table = use_facet<ctype<char> >(_CS_is.getloc())._CS_table();

  basic_streambuf<char, _CS_traits>* _CS_sbuf = _CS_is.rdbuf();
  typename _CS_traits::int_type _CS_c = _CS_sbuf->sgetc();
  while (!_CS_traits::eq_int_type(_CS_c, _CS_traits::eof())
         && (_CS_table[_CS_c] & ctype_base::space) != 0)
    _CS_c = _CS_sbuf->snextc();
  return _CS_traits::eq_int_type(_CS_c, _CS_traits::eof());
}

#if !defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>::sentry::sentry(basic_istream<_CS_cT, _CS_traits>& _CS_is,
                                                  bool _CS_noskipws):
  _CS_m_ok(true)
{
  _CS_init(_CS_is, _CS_noskipws);
}

template <class _CS_cT, class _CS_traits>
void
basic_istream<_CS_cT, _CS_traits>::sentry::_CS_init(basic_istream<_CS_cT, _CS_traits>& _CS_is,
                                                    bool _CS_noskipws)
{
  if (_CS_is.good())
    try
      {
        if (_CS_is.tie() != 0)
          _CS_is.tie()->flush();
        
        if (!_CS_noskipws && (_CS_is.flags() & ios_base::skipws) != 0)
          if (_CS_skip_ws(_CS_is))
            _CS_is.setstate(ios_base::eofbit | ios_base::failbit);
      }
    catch (...)
      {
        _CS_is._CS_process_exception();
      }

  _CS_m_ok = _CS_is.good();
}
#endif

#if 0
template <class _CS_cT, class _CS_traits>
_CXXRT_EXPORT
basic_istream<_CS_cT, _CS_traits>::sentry::~sentry()
{
  // No effect: potentially used for cleanup like unlocking
}
#endif

#if 0
template <class _CS_cT, class _CS_traits>
_CXXRT_EXPORT
basic_istream<_CS_cT, _CS_traits>::sentry::operator bool() const
{
  return _CS_m_ok;
}
#endif

// --------------------------------------------------------------------------

#if defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>::basic_istream(basic_istream const&)
{
}

template <class _CS_cT, class _CS_traits>
void
basic_istream<_CS_cT, _CS_traits>::operator= (basic_istream const&)
{
}
#endif

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>::basic_istream(basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf,
              basic_ostream<_CS_cT, _CS_traits>* _CS_tie):
  basic_ios<_CS_cT, _CS_traits>(_CS_sbuf),
  _CS_m_gcount(0)
{
  this->tie(_CS_tie);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>::
basic_istream(basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf):
  basic_ios<_CS_cT, _CS_traits>(_CS_sbuf),
  _CS_m_gcount(0)
{
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>::~basic_istream()
{
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (basic_istream& (*_CS_pf)(basic_istream&))
{
  return _CS_pf(*this);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (
  basic_ios<_CS_cT, _CS_traits>& (*_CS_pf)(basic_ios<_CS_cT, _CS_traits>&))
{
  _CS_pf(*this);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (ios_base& (*_CS_pf)(ios_base&))
{
  _CS_pf(*this);
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_Val>
inline basic_istream<_CS_cT, _CS_traits>&
_CS_get_numeric(basic_istream<_CS_cT, _CS_traits>& _CS_is, _CS_Val& _CS_val)
{
  typename basic_istream<_CS_cT, _CS_traits>::sentry _CS_kerberos(_CS_is, false);
  try
    {
      if (_CS_kerberos)
        {
          typedef istreambuf_iterator<_CS_cT, _CS_traits> _CS_iterator;
          typedef num_get<_CS_cT, _CS_iterator> _CS_num_get;
          ios_base::iostate _CS_err(ios_base::goodbit);
          use_facet<_CS_num_get>(_CS_is.getloc()).get(_CS_is, _CS_iterator(), _CS_is, _CS_err, _CS_val);
          _CS_is.setstate(_CS_err);
        }
      else
        _CS_is.setstate(ios_base::failbit);
    }
  catch (...)
    {
      _CS_is._CS_process_exception();
    }

  return _CS_is;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (bool& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (short& _CS_val)
{
  long _CS_tmp;
  _CS_get_numeric(*this, _CS_tmp);
  if (_CS_tmp < numeric_limits<short>::min())
    _CS_val = numeric_limits<short>::min();
  else if (_CS_tmp > numeric_limits<short>::max())
    _CS_val = numeric_limits<short>::max();
  else
    _CS_val = static_cast<short>(_CS_tmp);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (unsigned short& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (int& _CS_val)
{
  long _CS_tmp;
  _CS_get_numeric(*this, _CS_tmp);
  if (_CS_tmp < numeric_limits<int>::min())
    _CS_val = numeric_limits<int>::min();
  else if (_CS_tmp > numeric_limits<int>::max())
    _CS_val = numeric_limits<int>::max();
  else
    _CS_val = static_cast<int>(_CS_tmp);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (unsigned int& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (long& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (unsigned long& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (float& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (double& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (long double& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (void*& _CS_val)
{
  return _CS_get_numeric(*this, _CS_val);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
inline basic_istream<_CS_cT, _CS_traits>&
_CS_setstate(basic_istream<_CS_cT, _CS_traits>& _CS_strm, ios_base::iostate _CS_state)
{
  _CS_strm.setstate(_CS_state);
  return _CS_strm;
}

template <class _CS_cT, class _CS_traits>
inline _CS_cT
_CS_sgetc(basic_streambuf<_CS_cT, _CS_traits>* _CS_sb, bool& _CS_extracted)
{
  try { return _CS_sb->sgetc(); }
  catch (...) { _CS_extracted = true; throw; }
}

template <class _CS_cT, class _CS_traits>
inline _CS_cT
_CS_snextc(basic_streambuf<_CS_cT, _CS_traits>* _CS_sb, bool& _CS_extracted)
{
  try { return _CS_sb->sbumpc(); }
  catch (...) { _CS_extracted = true; throw; }
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::operator>> (basic_streambuf<_CS_cT, _CS_traits>* _CS_to)
{
  if (sentry(*this))
    {
      if (_CS_to == 0)
        return _CS_setstate(*this, this->failbit);
      bool _CS_extracted = false;
        
      try
        {
          basic_streambuf<_CS_cT, _CS_traits>* _CS_from = this->rdbuf();
          typename _CS_traits::int_type _CS_c = _CS_sgetc(_CS_from, _CS_extracted);

          // special processing of the first character
          if (_CS_traits::eq_int_type(_CS_c, _CS_traits::eof())
              || _CS_traits::eq_int_type(_CS_to->sputc(_CS_c), _CS_traits::eof()))
            return _CS_setstate(*this, ios_base::failbit);
          
          for (_CS_c = _CS_snextc(_CS_from, _CS_extracted);
               !_CS_traits::eq_int_type(_CS_c, _CS_traits::eof());
               _CS_c = _CS_snextc(_CS_from, _CS_extracted))
            {
              if (_CS_traits::eq_int_type(_CS_to->sputc(_CS_traits::to_char_type(_CS_c)),_CS_traits::eof()))
                return *this;
            }

          setstate(this->eofbit);
        }
      catch (...)
        {
          if (_CS_extracted)
            {
              try { this->setstate(ios_base::failbit); } catch (...) {}
              if (this->exceptions() & this->failbit)
                throw;
            }
          else
            this->_CS_process_exception();
        }
    }
  else
    this->setstate(ios_base::failbit);
  
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
streamsize
basic_istream<_CS_cT, _CS_traits>::gcount() const
{
  return _CS_m_gcount;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
typename basic_istream<_CS_cT, _CS_traits>::int_type
basic_istream<_CS_cT, _CS_traits>::_CS_get()
{
  if (sentry(*this, true, true))
    try
      {
        typename _CS_traits::int_type _CS_rc(this->rdbuf()->sbumpc());
        if (_CS_traits::eq_int_type(_CS_rc, _CS_traits::eof()))
          {
            setstate(this->failbit | this->eofbit);
            _CS_m_gcount = 0;
          }
        else
          {
            _CS_m_gcount = 1;
            return _CS_rc;
          }
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  else
    this->setstate(ios_base::failbit);
  
  return _CS_traits::eof();
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::get(char_type& _CS_c)
{
  if (sentry(*this, true))
    try
      {
        typename _CS_traits::int_type _CS_rc(this->rdbuf()->sbumpc());
        if (_CS_traits::eq_int_type(_CS_rc, _CS_traits::eof()))
          {
            setstate(this->failbit | this->eofbit);
            _CS_m_gcount = 0;
          }
        else
          {
            _CS_m_gcount = 1;
            _CS_c = _CS_traits::to_char_type(_CS_rc);
          }
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
    else
      this->setstate(ios_base::failbit);

  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::get(char_type* _CS_str, streamsize _CS_n)
{
  return get(_CS_str, _CS_n, this->_CS_newline());
}

// --------------------------------------------------------------------------

template <typename _CS_cT>
inline streamsize _CS_ptrdist(_CS_cT const* _CS_beg, _CS_cT const* _CS_end)
{
#if 0
  return _CS_end - _CS_beg;
#else
  streamsize _CS_rc = 0;
  for (; _CS_beg != _CS_end; ++_CS_beg)
    ++_CS_rc;
  return _CS_rc;
#endif
}

template <class _CS_cT>
struct _CS_finish
{
  _CS_finish(streamsize& _CS_gcount, _CS_cT*& _CS_str):
    _CS_m_gcount(_CS_gcount),
    _CS_m_str(_CS_str),
    _CS_m_cur(_CS_str)
    {
    }
  ~_CS_finish()
    {
      this->_CS_m_gcount = _CS_ptrdist(this->_CS_m_str, this->_CS_m_cur);
      *this->_CS_m_cur = _CS_cT();
    }
  bool identical() const { return this->_CS_m_cur == this->_CS_m_str; }
 private:
  streamsize&    _CS_m_gcount;
  _CS_cT*        _CS_m_str;
  _CS_cT*&       _CS_m_cur;
  _CS_finish(_CS_finish const&);
  void operator= (_CS_finish const&);
};

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::get(char_type* _CS_str, streamsize _CS_n, char_type _CS_d)
{
  sentry _CS_kerberos(*this, true);
  if (_CS_kerberos)
    try
      {
        _CS_finish<char_type> _CS_fo(this->_CS_m_gcount, _CS_str);
        char_type* _CS_bufend = _CS_str + _CS_n - 1;

        basic_streambuf<_CS_cT, _CS_traits>* _CS_sb = this->rdbuf();

        if (_CS_str == _CS_bufend)
          basic_ios<_CS_cT, _CS_traits>::setstate(ios_base::failbit);
        else if (traits_type::eq_int_type(_CS_sb->sgetc(), traits_type::eof()))
          basic_ios<_CS_cT, _CS_traits>::setstate(ios_base::failbit | ios_base::eofbit);
        else
          {
            while (_CS_str != _CS_bufend)
              {
                char_type* _CS_b = _CS_sb->_CS_begin();
                streamsize _CS_max = _CS_sb->_CS_end() - _CS_b;
                if (_CS_max > streamsize(_CS_bufend - _CS_str))
                  _CS_max = streamsize(_CS_bufend - _CS_str);

                if (_CS_max == 0)
                  {
                    int_type _CS_i = _CS_sb->_CS_underflow();
                    if (traits_type::eq_int_type(_CS_i, traits_type::eof()))
                      {
                        setstate(this->eofbit);
                        return *this;
                      }
                    else
                      {
                        *_CS_str = traits_type::to_char_type(_CS_i);
                        if (traits_type::eq(*_CS_str, _CS_d))
                          {
                            if (_CS_fo.identical())
                              this->setstate(ios_base::failbit);
                            return *this;
                          }
                        else
                          {
                            ++_CS_str;
                            _CS_sb->sbumpc();
                          }
                      }
                  }
                else
                  {
                    char_type const* _CS_f = traits_type::find(_CS_b, _CS_max, _CS_d);
                    if (_CS_f == 0)
                      _CS_f = _CS_b + _CS_max;
                    streamsize _CS_count(_CS_f - _CS_b);
                    _CS_sb->_CS_commit(const_cast<char_type*>(_CS_f));
                    char_traits<char_type>::copy(_CS_str, _CS_b, _CS_count);
                    _CS_str += _CS_count;

                    if (_CS_count < _CS_max)
                      {
                        if (_CS_fo.identical())
                          this->setstate(ios_base::failbit);
                        return *this;
                      }
                    else
                      _CS_sb->_CS_underflow();
                  }
              }
          }
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  else
    this->setstate(ios_base::failbit);

  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::get(basic_streambuf<_CS_cT, _CS_traits>& _CS_sb)
{
  return get(_CS_sb, this->_CS_newline());
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::get(basic_streambuf<_CS_cT, _CS_traits>& _CS_to, char_type _CS_delim)
{
  if (sentry(*this, true))
    {
      try
        {
          basic_streambuf<_CS_cT, _CS_traits>* _CS_from = this->rdbuf();
          typename _CS_traits::int_type _CS_c;
          typename _CS_traits::int_type _CS_d(_CS_traits::to_int_type(_CS_delim));
          bool _CS_inserted = false;
          bool _CS_done = false;
          
          while (!_CS_done
                 && !_CS_traits::eq_int_type(_CS_c = _CS_from->sgetc(), _CS_traits::eof())
                 && !_CS_traits::eq_int_type(_CS_c, _CS_d))
            {
#if 1
              if (_CS_traits::eq_int_type(_CS_to.sputc(_CS_c), _CS_traits::eof()))
                break;
              else
                _CS_inserted = true;
              _CS_from->sbumpc();
#else
              streamsize _CS_n = _CS_from->_CS_end() - _CS_from->_CS_begin();
              if (_CS_n != 0)
                {
                  if (_CS_cT const* _CS_f = _CS_traits::find(_CS_from->_CS_begin(), _CS_n, _CS_delim))
                    {
                      _CS_done = true;
                      _CS_n = _CS_f - _CS_from->_CS_begin();
                    }
                  
                  streamsize _CS_m = _CS_to.sputn(_CS_from->_CS_begin(), _CS_n);
                  if (_CS_m != _CS_n)
                    {
                      if (_CS_m != 0)
                        {
                          _CS_inserted = true;
                          _CS_from->_CS_commit(_CS_from->_CS_begin() + _CS_m);
                        }
                      break;
                    }
                  else
                    _CS_from->_CS_commit(_CS_from->_CS_begin() + _CS_n);
                  _CS_inserted = true;
                }
              else
                if (_CS_traits::eq_int_type(_CS_to.sputc(_CS_traits::to_char_type(_CS_c)), _CS_traits::eof()))
                  break;
                else
                  _CS_inserted = true;
#endif
            }
          if (_CS_traits::eq_int_type(_CS_c, _CS_traits::eof()))
            setstate(_CS_inserted? (this->eofbit): (this->eofbit | this->failbit));
          else if (!_CS_inserted)
            setstate(this->failbit);
        }
      catch (...)
        {
          this->_CS_process_exception();
        }
    }
  else
    this->setstate(ios_base::failbit);
  
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT>
struct _CS_getline_finish
{
  _CS_getline_finish(streamsize& _CS_gcount, _CS_cT*& _CS_str):
    _CS_m_gcount(_CS_gcount),
    _CS_m_str(_CS_str),
    _CS_m_cur(_CS_str)
    {
      this->_CS_m_gcount = 0;
    }
  ~_CS_getline_finish()
    {
      this->_CS_m_gcount += _CS_ptrdist(this->_CS_m_str, this->_CS_m_cur);
      *this->_CS_m_cur = _CS_cT();
    }
  streamsize& _CS_m_gcount;
  _CS_cT*         _CS_m_str;
  _CS_cT*&        _CS_m_cur;

private:
  _CS_getline_finish(_CS_getline_finish const&);
  void operator= (_CS_getline_finish const&);
};


template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::getline(char_type* _CS_str, streamsize _CS_n)
{
  return getline(_CS_str, _CS_n, this->_CS_newline());
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::getline(char_type* _CS_str, streamsize _CS_n, char_type _CS_d)
{
  sentry _CS_kerberos(*this, true);
  if (_CS_kerberos)
    try
      {
        _CS_getline_finish<char_type> _CS_fo(_CS_m_gcount, _CS_str);
        char_type* _CS_bufend = _CS_str + _CS_n - 1;

        basic_streambuf<_CS_cT, _CS_traits>* _CS_sb = this->rdbuf();

        if (_CS_str == _CS_bufend)
          basic_ios<_CS_cT, _CS_traits>::setstate(ios_base::failbit);
        else if (traits_type::eq_int_type(_CS_sb->sgetc(), traits_type::eof()))
          basic_ios<_CS_cT, _CS_traits>::setstate(ios_base::failbit | ios_base::eofbit);
        else
          {
            while (_CS_str != _CS_bufend)
              {
                char_type* _CS_b = _CS_sb->_CS_begin();
                streamsize _CS_max = _CS_sb->_CS_end() - _CS_b;
                if (_CS_max > streamsize(_CS_bufend - _CS_str))
                  _CS_max = streamsize(_CS_bufend - _CS_str);

                if (_CS_max == 0)
                  {
                    int_type _CS_i = _CS_sb->sbumpc();
                    if (traits_type::eq_int_type(_CS_i, traits_type::eof()))
                      {
                        this->setstate(ios_base::eofbit);
                        return *this;
                      }
                    else
                      {
                        *_CS_str = traits_type::to_char_type(_CS_i);
                        if (traits_type::eq(*_CS_str, _CS_d))
                          {
                            this->_CS_m_gcount = 1; //-dk:TODO check if this has to be += 1
                            return *this;
                          }
                        else
                          ++_CS_str;
                      }
                  }
                else
                  {
                    char_type const* _CS_f = traits_type::find(_CS_b, _CS_max, _CS_d);
                    if (_CS_f == 0)
                      _CS_f = _CS_b + _CS_max;
                    streamsize _CS_count(_CS_f - _CS_b);
                    char_traits<char_type>::copy(_CS_str, _CS_b, _CS_count);
                    _CS_str += _CS_count;

                    if (_CS_count < _CS_max)
                      {
                        if (traits_type::eq(_CS_b[_CS_count], _CS_d))
                          {
                            _CS_sb->_CS_commit(const_cast<char_type*>(_CS_f + 1));
                            _CS_m_gcount = 1;
                          }
                        else
                          {
                            _CS_sb->_CS_commit(const_cast<char_type*>(_CS_f));
                          }
                        return *this;
                      }
                    else
                      {
                        _CS_sb->_CS_commit(const_cast<char_type*>(_CS_f));
                        _CS_sb->_CS_underflow();
                      }
                  }
              }
            int_type _CS_i = _CS_sb->sgetc();
            if (!traits_type::eq_int_type(_CS_i, traits_type::eof())
                && traits_type::eq(traits_type::to_char_type(_CS_i), _CS_d))
              {
                _CS_sb->sbumpc();
                _CS_m_gcount = 1;
              }
            else
              this->setstate(ios_base::failbit);
          }
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  else
    this->setstate(ios_base::failbit);

  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::ignore()
{
  if (sentry(*this, true))
    try
      {
        if (_CS_traits::eq_int_type(this->rdbuf()->sbumpc(), _CS_traits::eof()))
          {
            basic_ios<_CS_cT, _CS_traits>::setstate(ios_base::eofbit);
            _CS_m_gcount = 0;
          }
        else
          _CS_m_gcount = 1;
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::ignore(streamsize _CS_n)
{
  return this->ignore(_CS_n, _CS_traits::eof());
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
struct _CS_ignore_finish
{
  _CS_ignore_finish(streamsize& _CS_gcount):
    _CS_m_gcount(_CS_gcount)
  {
    _CS_m_gcount = 0;
  }
  ~_CS_ignore_finish()
  {
    //-dk:TODO fix count! _CS_m_gcount += _CS_m_cur - _CS_m_str;
  }
  streamsize&    _CS_m_gcount;
  
private:
  _CS_ignore_finish(_CS_ignore_finish const&);
  void operator= (_CS_ignore_finish const&);
};


template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::ignore(streamsize _CS_n, int_type _CS_c)
{
  if (sentry(*this, true))
    try
      {
        _CS_m_gcount = 0;
        basic_streambuf<_CS_cT, _CS_traits>* _CS_sb = this->rdbuf();

        if (traits_type::eq_int_type(_CS_sb->sgetc(), traits_type::eof()))
          basic_ios<_CS_cT, _CS_traits>::setstate(ios_base::eofbit);
        else
          {
            char_type _CS_d = traits_type::to_char_type(_CS_c);

            while (_CS_m_gcount < _CS_n || _CS_n == numeric_limits<streamsize>::max())
              {
                char_type* _CS_b = _CS_sb->_CS_begin();
                streamsize _CS_max = _CS_sb->_CS_end() - _CS_b;

                if (_CS_max == 0)
                  {
                    int_type _CS_i = _CS_sb->sbumpc();
                    if (traits_type::eq_int_type(_CS_i, traits_type::eof()))
                      {
                        this->setstate(ios_base::eofbit);
                        return *this;
                      }
                    else
                      {
                        _CS_m_gcount += 1;
                        if (traits_type::eq_int_type(_CS_i, _CS_c))
                          return *this;
                      }
                  }
                else
                  {
                    if (_CS_n - _CS_m_gcount < _CS_max)
                      _CS_max = _CS_n - _CS_m_gcount;
                    
                    char_type const* _CS_f = traits_type::find(_CS_b, _CS_max, _CS_d);
                    if (_CS_f == 0)
                      {
                        _CS_m_gcount += _CS_max;
                        _CS_sb->_CS_commit(_CS_b + _CS_max);
                      }
                    else
                      {
                        _CS_m_gcount +=  ++_CS_f - _CS_b;
                        _CS_sb->_CS_commit(const_cast<char_type*>(_CS_f));
                        return *this;
                      }
                  }
              }
          }
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
typename basic_istream<_CS_cT, _CS_traits>::int_type
basic_istream<_CS_cT, _CS_traits>::peek()
{
  try
    {
      _CS_m_gcount = 0;
      return sentry(*this, true)? this->rdbuf()->sgetc(): traits_type::eof();
    }
  catch (...)
    {
      this->_CS_process_exception();
      return traits_type::eof();
    }
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::read(char_type* _CS_str, streamsize _CS_n)
{
  if (sentry(*this, true))
    try
      {
        _CS_m_gcount = this->rdbuf()->sgetn(_CS_str, _CS_n);
        if (_CS_m_gcount < _CS_n)
          this->setstate(ios_base::eofbit | ios_base::failbit);
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  else
    {
      _CS_m_gcount = 0;
      this->setstate(ios_base::failbit);
    }
  
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
streamsize
basic_istream<_CS_cT, _CS_traits>::readsome(char_type* _CS_str,
                                                        streamsize _CS_n)
{
  if (sentry(*this, true))
    try
      {
        streamsize _CS_avail = this->rdbuf()->in_avail();
        switch (_CS_avail)
          {
          case -1:
            this->setstate(ios_base::eofbit);
          case 0:
            _CS_m_gcount = 0;
            break;
          default:
            _CS_n = _CS_avail < _CS_n? _CS_avail: _CS_n;
            _CS_m_gcount = this->rdbuf()->sgetn(_CS_str, _CS_n);
            break;
          }
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
    else
      {
        _CS_m_gcount = 0;
        this->setstate(ios_base::failbit);
      }

  return _CS_m_gcount;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::putback(char_type _CS_c)
{
  if (sentry(*this, true))
    try
      {
        if (traits_type::eq_int_type(this->rdbuf()->sputbackc(_CS_c), traits_type::eof()))
          setstate(this->badbit);
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  else
    this->setstate(ios_base::failbit);

  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::unget()
{
  if (sentry(*this, true))
    try
      {
        if (traits_type::eq_int_type(this->rdbuf()->sungetc(), traits_type::eof()))
          setstate(this->badbit);
      }
    catch (...)
      {
        this->_CS_process_exception();
      }
  else
    this->setstate(ios_base::failbit);

  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
int
basic_istream<_CS_cT, _CS_traits>::sync()
{
  if (this->rdbuf() == 0)
    return -1;
  else if (traits_type::eq_int_type(this->rdbuf()->pubsync(), traits_type::eof()))
    {
      setstate(this->badbit);
      return traits_type::eof();
    }

  return 0;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
typename basic_istream<_CS_cT, _CS_traits>::pos_type
basic_istream<_CS_cT, _CS_traits>::tellg()
{
  return this->fail()? static_cast<typename traits_type::pos_type>(-1): this->rdbuf()->pubseekoff(0, this->cur, this->in);
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::seekg(pos_type _CS_pos)
{
  // TODO set failbit
  if (!this->fail())
    this->rdbuf()->pubseekpos(_CS_pos, this->in);
  return *this;
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
basic_istream<_CS_cT, _CS_traits>::seekg(off_type _CS_off, ios_base::seekdir _CS_dir)
{
  // TODO set failbit
  if (!this->fail())
    this->rdbuf()->pubseekoff(_CS_off, _CS_dir, this->in);
  return *this;
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_scT>
inline basic_istream<_CS_cT, _CS_traits>&
_CS_get_char(basic_istream<_CS_cT, _CS_traits>& _CS_in, _CS_scT& _CS_c)
{
  typename basic_istream<_CS_cT, _CS_traits>::sentry _CS_kerberos(_CS_in);
  if (_CS_kerberos)
    try
      {
        typename _CS_traits::int_type _CS_i = _CS_in.rdbuf()->sbumpc();
        if (!_CS_traits::eq_int_type(_CS_i, _CS_traits::eof()))
          _CS_c = _CS_traits::to_char_type(_CS_i);
        else
          _CS_in.setstate(ios_base::failbit | ios_base::eofbit);
      }
    catch (...)
      {
        _CS_in._CS_process_exception();
      }
  else
    _CS_in.setstate(ios_base::failbit);
  return _CS_in;
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
operator>> (basic_istream<_CS_cT, _CS_traits>& _CS_in, _CS_cT& _CS_c)
{
  return _CS_get_char(_CS_in, _CS_c);
}

template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator>> (basic_istream<char, _CS_traits>& _CS_in, signed char& _CS_c)
{
  return _CS_get_char(_CS_in, _CS_c);
}

template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator>> (basic_istream<char, _CS_traits>& _CS_in, unsigned char& _CS_c)
{
  return _CS_get_char(_CS_in, _CS_c);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits, class _CS_scT>
inline basic_istream<_CS_cT, _CS_traits>&
_CS_get_word(basic_istream<_CS_cT, _CS_traits>& _CS_in,
                   _CS_scT* _CS_str)
{
  typename basic_istream<_CS_cT, _CS_traits>::sentry _CS_kerberos(_CS_in);
  if (_CS_kerberos)
    try
      {
        ptrdiff_t _CS_n = _CS_in.width() > 0? _CS_in.width() - 1
                                            : numeric_limits<ptrdiff_t>::max() - 1;

        basic_streambuf<_CS_cT, _CS_traits>* _CS_sbuf = _CS_in.rdbuf();
        _CS_scT* _CS_cur = _CS_str;
        ctype<_CS_cT> const& _CS_ctype = use_facet<ctype<_CS_cT> >(_CS_in.getloc());
        typename _CS_traits::int_type _CS_i;
        typename _CS_traits::int_type _CS_eof = _CS_traits::eof();
  
        for (_CS_i = _CS_sbuf->sgetc();
             _CS_cur - _CS_str < _CS_n && !_CS_traits::eq_int_type(_CS_i, _CS_eof);
             _CS_i = _CS_sbuf->snextc())
          {
            typename _CS_traits::char_type _CS_c(_CS_traits::to_char_type(_CS_i));
            if (_CS_ctype.is(ctype_base::space, _CS_c))
              break;
            *_CS_cur = _CS_c;
            if (_CS_traits::eq(_CS_c, _CS_scT()))
              break;
            ++_CS_cur;
          }

        *_CS_cur = _CS_scT();
        _CS_in.width(0);
        if (_CS_traits::eq_int_type(_CS_i, _CS_traits::eof()))
          _CS_in.setstate(ios_base::eofbit);
        if (_CS_cur == _CS_str)
          _CS_in.setstate(ios_base::failbit);
      }
    catch (...)
      {
        _CS_in._CS_process_exception();
      }
  else
    {
      _CS_in.setstate(ios_base::failbit);
      *_CS_str = _CS_scT();
    }
  return _CS_in;
}

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
operator>> (basic_istream<_CS_cT, _CS_traits>& _CS_in, _CS_cT* _CS_str)
{
  return _CS_get_word(_CS_in, _CS_str);
}

template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator>> (basic_istream<char, _CS_traits>& _CS_in, signed char* _CS_str)
{
  return _CS_get_word(_CS_in, _CS_str);
}

template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator>> (basic_istream<char, _CS_traits>& _CS_in, unsigned char* _CS_str)
{
  return _CS_get_word(_CS_in, _CS_str);
}

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
ws(basic_istream<_CS_cT, _CS_traits>& _CS_in)
{
  if (_CS_skip_ws(_CS_in))
    _CS_in.setstate(ios_base::eofbit);
  return _CS_in;
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_ISTREAM_CC__ */
