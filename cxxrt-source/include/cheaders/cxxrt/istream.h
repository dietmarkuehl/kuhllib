// -*-C++-*- cxxrt/istream.h
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
// Title:   Declarations for the class basic_istream
// Version: $Id: istream.h,v 1.3 2003/04/13 19:35:59 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_ISTREAM_H__)
#define _CXXRT_ISTREAM_H__ 1

// --------------------------------------------------------------------------

#if !defined(_CXXRT_IOS_H__)
#  include <cxxrt/ios.h>
#endif
#if !defined(_CXXRT_STREAMBUF_H__)
#  include <cxxrt/streambuf.h>
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
class basic_istream: virtual public basic_ios<_CS_cT, _CS_traits>
{
public:
  typedef basic_ios<_CS_cT, _CS_traits> _CS_basic_ios;
  typedef typename _CS_basic_ios::char_type   char_type;
  typedef typename _CS_basic_ios::int_type    int_type;
  typedef typename _CS_basic_ios::pos_type    pos_type;
  typedef typename _CS_basic_ios::off_type    off_type;
  typedef typename _CS_basic_ios::traits_type traits_type;

  _CXXRT_EXPLICIT basic_istream(basic_streambuf<_CS_cT, _CS_traits>*);
  // the following constructor is for internal use only:
  _CXXRT_EXPLICIT basic_istream(basic_streambuf<_CS_cT, _CS_traits>*,
                                  basic_ostream<_CS_cT, _CS_traits>*); 
  ~basic_istream();

  class sentry
  {
  public:
#if !defined(_MSC_VER)
    _CXXRT_EXPLICIT sentry(basic_istream<_CS_cT, _CS_traits>&, bool = false);
#else
    _CXXRT_EXPLICIT sentry(basic_istream<_CS_cT, _CS_traits>&, bool = false)
      {
        //-dk:TODO
      }
#endif
    sentry(basic_istream<_CS_cT, _CS_traits>& _CS_is, bool, bool):
      _CS_m_ok(true)
    {
      if (_CS_is.flags() & (ios_base::_CS_os_good | ios_base::_CS_os_tie))
        _CS_init(_CS_is, true);
    }
    ~sentry() {}
    operator bool() const { return _CS_m_ok; }

  private:
    void _CS_init(basic_istream<_CS_cT, _CS_traits>&, bool);
    sentry(sentry const&);         // not defined
    void operator=(sentry const&); // not defined

    bool                         _CS_m_ok; // has to be remembered to fit the standard exactly!
  };

  basic_istream& operator>> (basic_istream& (*)(basic_istream&));
  basic_istream& operator>> (_CS_basic_ios& (*)(_CS_basic_ios&));
  basic_istream& operator>> (ios_base& (*)(ios_base&));
  basic_istream& operator>> (bool&);
  basic_istream& operator>> (short&);
  basic_istream& operator>> (unsigned short&);
  basic_istream& operator>> (int&);
  basic_istream& operator>> (unsigned int&);
  basic_istream& operator>> (long&);
  basic_istream& operator>> (unsigned long&);
  basic_istream& operator>> (float&);
  basic_istream& operator>> (double&);
  basic_istream& operator>> (long double&);
  basic_istream& operator>> (void*&);
  basic_istream& operator>> (basic_streambuf<_CS_cT, _CS_traits>*);

  streamsize     gcount() const;
  int_type       get() {
    //-dk:TODO
    _CS_m_gcount = 1;
    basic_streambuf<_CS_cT, _CS_traits>* _CS_sb = basic_ios<_CS_cT, _CS_traits>::rdbuf();
    if (_CS_sb == 0)
      return traits_type::eof();
    else
      return (this->_CS_need_isentry() || _CS_sb->_CS_begin() == _CS_sb->_CS_end())
	? _CS_get() : _CS_sb->_CS_gbump();
  }
  basic_istream& get(char_type&);
  basic_istream& get(char_type*, streamsize);
  basic_istream& get(char_type*, streamsize, char_type);
  basic_istream& get(basic_streambuf<_CS_cT, _CS_traits>&);
  basic_istream& get(basic_streambuf<_CS_cT, _CS_traits>&, char_type);
  basic_istream& getline(char_type*, streamsize);
  basic_istream& getline(char_type*, streamsize, char_type);
  basic_istream& ignore();
  basic_istream& ignore(streamsize);
  basic_istream& ignore(streamsize, int_type);
  int_type       peek();
  basic_istream& read(char_type*, streamsize);
  streamsize     readsome(char_type*, streamsize);
  basic_istream& putback(char_type);
  basic_istream& unget();
  int            sync();
  pos_type       tellg();
  basic_istream& seekg(pos_type);
  basic_istream& seekg(off_type, ios_base::seekdir);

private:
  int_type       _CS_get();
  basic_istream(basic_istream const&);   // deliberatly not accessible
  void operator= (basic_istream const&); // deliberatly not accessible

  streamsize _CS_m_gcount;
};

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
operator >> (basic_istream<_CS_cT, _CS_traits>&, _CS_cT&);

template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator >> (basic_istream<char, _CS_traits>&, signed char&);
template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator >> (basic_istream<char, _CS_traits>&, unsigned char&);

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
operator >> (basic_istream<_CS_cT, _CS_traits>&, _CS_cT*);

template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator >> (basic_istream<char, _CS_traits>&, signed char*);
template <class _CS_traits>
basic_istream<char, _CS_traits>&
operator >> (basic_istream<char, _CS_traits>&, unsigned char*);

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_traits>
basic_istream<_CS_cT, _CS_traits>&
ws(basic_istream<_CS_cT, _CS_traits>&);

// --------------------------------------------------------------------------

#if defined(_CXXRT_MIN_IO) || defined(_CXXRT_USE_MIN_IO)
extern template class basic_istream<char>;
extern template class basic_istream<wchar_t>;
#endif

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_ISTREAM_CC__)
#  include <srccxx/istream.cc>
#endif

#endif /* _CXXRT_ISTREAM_H__ */
