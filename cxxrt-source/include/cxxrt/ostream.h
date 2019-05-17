// -*-C++-*- cxxrt/ostream.h
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
// Title:   basic_ostream - writing using IOStreams 
// Version: $Id: ostream.h,v 1.1.1.1 2002/06/05 01:01:56 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_OSTREAM_H__)
#define _CXXRT_OSTREAM_H__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_IOS_H__)
#  include <cxxrt/ios.h>
#endif

#include <cxxrt/ctraits.h>

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
class basic_ostream: public virtual basic_ios<_CS_cT, _CS_traits>
{
public:
  typedef basic_ios<_CS_cT, _CS_traits> _CS_basic_ios;
  typedef typename _CS_basic_ios::char_type   char_type;
  typedef typename _CS_basic_ios::int_type    int_type;
  typedef typename _CS_basic_ios::pos_type    pos_type;
  typedef typename _CS_basic_ios::off_type    off_type;
  typedef typename _CS_basic_ios::traits_type traits_type;

  class sentry
  {
  public:
    _CXXRT_EXPLICIT sentry(basic_ostream<_CS_cT, _CS_traits>& _CS_os);
    ~sentry();
    operator bool() const;

  private:
    sentry(sentry const&);         // not defined
    void operator=(sentry const&); // not defined

    bool                                   _CS_m_ok;
    basic_ostream<char_type, traits_type>& _CS_m_os;
  };

  explicit basic_ostream(basic_streambuf<_CS_cT, _CS_traits>*);
  // the following constructor is for internal use only:
  basic_ostream(basic_streambuf<_CS_cT, _CS_traits>*, bool);
  ~basic_ostream();

  basic_ostream& operator<< (basic_ostream& (*)(basic_ostream&));
  basic_ostream& operator<< (_CS_basic_ios& (*)(_CS_basic_ios&));
  basic_ostream& operator<< (ios_base& (*)(ios_base&));
  basic_ostream& operator<< (bool);
  basic_ostream& operator<< (short);
  basic_ostream& operator<< (unsigned short);
  basic_ostream& operator<< (int);
  basic_ostream& operator<< (unsigned int);
  basic_ostream& operator<< (long);
  basic_ostream& operator<< (unsigned long);
  basic_ostream& operator<< (float);
  basic_ostream& operator<< (double);
  basic_ostream& operator<< (long double);
  basic_ostream& operator<< (void const*);
  basic_ostream& operator<< (basic_streambuf<_CS_cT, _CS_traits>*);

  basic_ostream& put(char_type);
  basic_ostream& write(char_type const*, streamsize);
  basic_ostream& flush();

  pos_type tellp();
  basic_ostream& seekp(pos_type);
  basic_ostream& seekp(off_type, ios_base::seekdir);

private:
  basic_ostream(basic_ostream const&);   // deliberatly not accessible
  void operator= (basic_ostream const&); // deliberatly not accessible

#if !defined(_MSC_VER)
  template <class _CS_T> void _CS_print(_CS_T);
#else
#if 1
  template <class _CS_T>
  void _CS_print(_CS_T _CS_t)
    {
      if (typename basic_ostream<_CS_cT, _CS_traits>::sentry(*this))
        {
          try
            {
              if (use_facet<num_put<_CS_cT> >(getloc()).put(*this, *this, fill(), _CS_t).failed())
                setstate(badbit);
            }
          catch (...)
            {
              _CS_process_exception();
            }
        }
    }
#endif
#endif
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>&, _CS_cT);

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>&, char);

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>&, char);

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>&, signed char);

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>&, unsigned char);

// --------------------------------------------------------------------------

#if !defined(_MSC_VER)
template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>&, _CS_cT const*);

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>&
operator<< (basic_ostream<_CS_cT, _CS_traits>&, char const*);
#endif

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>&, char const*);

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>&, signed char const*);

template <class _CS_traits>
basic_ostream<char, _CS_traits>&
operator<< (basic_ostream<char, _CS_traits>&, unsigned char const*);

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>& endl(basic_ostream<_CS_cT, _CS_traits>&);
template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>& ends(basic_ostream<_CS_cT, _CS_traits>&);
template <class _CS_cT, class _CS_traits>
basic_ostream<_CS_cT, _CS_traits>& flush(basic_ostream<_CS_cT, _CS_traits>&);

// --------------------------------------------------------------------------

#if defined(_CXXRT_MIN_IO)
extern template class basic_ostream<char>;
extern template class basic_ostream<wchar_t>;
#endif

#if !defined(_CXXRT_MIN_IO) || !defined(_CXXRT_OSTREAM_VTBL)

template <class _CS_cT, class _CS_traits>
inline
basic_ostream<_CS_cT, _CS_traits>::sentry::sentry(basic_ostream<_CS_cT, _CS_traits>& _CS_os):
  _CS_m_ok(true),
  _CS_m_os(_CS_os)
{
  if (_CS_m_os.tie() != 0)
    _CS_m_os.tie()->flush();
  _CS_m_ok = _CS_m_os.good();
}

template <class _CS_cT, class _CS_traits>
inline
basic_ostream<_CS_cT, _CS_traits>::sentry::~sentry()
{
  if (((_CS_m_os.flags() & ios_base::unitbuf) != 0))// && !std::uncaught_exception())
    _CS_m_os.flush();
}

template <class _CS_cT, class _CS_traits>
inline
basic_ostream<_CS_cT, _CS_traits>::sentry::operator bool() const
{
  return _CS_m_ok;
}

#endif

// --------------------------------------------------------------------------

#if defined(_CXXRT_MIN_IO) || defined(_CXXRT_USE_MIN_IO)
extern template class basic_ostream<char>;
extern template class basic_ostream<wchar_t>;
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_OSTREAM_CC__)
#  include <srccxx/ostream.cc>
#endif

#endif /* _CXXRT_OSTREAM_H__ */
