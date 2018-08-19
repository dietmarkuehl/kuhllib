// -*-C++-*- cxxrt/streambuf.h
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
// Title:   Declaration for the abstraction of external representations
// Version: $Id: streambuf.h,v 1.1.1.1 2002/06/05 01:01:52 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_STREAMBUF_H__)
#define _CXXRT_STREAMBUF_H__ 1

#if !defined(_CXXRT_IOS_BASE_H__)
#  include <cxxrt/ios_base.h>
#endif
#if !defined(_CXXRT_SBUF_FWD_H__)
#  include <cxxrt/sbuf_fwd.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(_CXXRT_CTRAITS_H__)
#  include <cxxrt/ctraits.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

struct input_iterator_tag;
template <class _CS_cT, class _CS_It, class _CS_tag> class _CS_in_it;
template <class _CS_cT, class _CS_It, class _CS_tag> class _CS_input_it;

template <class _CS_cT, class _CS_traits> class istreambuf_iterator;

template <class _CS_cT, class _CS_traits>
class basic_streambuf
{
  friend class _CS_in_it<_CS_cT, istreambuf_iterator<_CS_cT, _CS_traits>,
    input_iterator_tag>;
  friend class _CS_input_it<_CS_cT, istreambuf_iterator<_CS_cT, _CS_traits>,
    input_iterator_tag>;
  basic_streambuf(basic_streambuf const &); // without implementation
  void operator=(basic_streambuf const &);  // without implementation

public:
  typedef _CS_cT                     char_type;
  typedef _CS_traits                    traits_type;
  typedef typename _CS_traits::int_type int_type;
  typedef typename _CS_traits::pos_type pos_type;
  typedef typename _CS_traits::off_type off_type;

  virtual ~basic_streambuf();

  locale pubimbue(locale const &);
  locale getloc() const;

  basic_streambuf<_CS_cT, _CS_traits>* pubsetbuf(char_type*, streamsize);

  pos_type pubseekoff(off_type, ios_base::seekdir, ios_base::openmode = ios_base::out | ios_base::in);
  pos_type pubseekoff(off_type _CS_o, ios_base::seek_dir _CS_s,
                      ios_base::open_mode _CS_w = ios_base::out | ios_base::in)
    {
      _CXXRT_DEPRECATED(seek_dir__and__open_mode__are__deprecated,use__seekdir__and__openmode__instead)
      return pubseekoff(_CS_o, ios_base::seekdir(_CS_s), ios_base::openmode(_CS_w));
    }
  pos_type pubseekpos(pos_type, ios_base::openmode = ios_base::out | ios_base::in);
  pos_type pubseekpos(pos_type _CS_p, ios_base::open_mode _CS_w)
    {
      _CXXRT_DEPRECATED(seek_dir__and__open_mode__are__deprecated,use__seekdir__and__openmode__instead)
      return pubseekpos(_CS_p, ios_base::openmode(_CS_w));
    }
  int pubsync();

  streamsize in_avail();
  int_type   snextc();
  int_type   sbumpc() { return _CS_gcur != _CS_gend? _CS_traits::to_int_type(*_CS_gcur++): uflow(); }
  void       stossc()
    {
      _CXXRT_DEPRECATED(stossc__is__deprecated,use__sbumpc__instead)
      sbumpc();
    }
    
  int_type   sgetc() { return _CS_gend == _CS_gcur? underflow(): _CS_traits::to_int_type(*_CS_gcur); }
  streamsize sgetn(char_type*, streamsize);

  int_type sputbackc(char_type);
  int_type sungetc();

  int_type   sputc(char_type _CS_c)
    {
      if (_CS_pcur == _CS_pend)
        return overflow(_CS_traits::to_int_type(_CS_c));
      else
        {
          _CS_traits::assign(*_CS_pcur++, _CS_c);
          return _CS_traits::to_int_type(_CS_c);
        }
    }

  streamsize sputn(char_type const*, streamsize);

  // The following functions are an extension specific to this implementation.
  // They are used to improve the performance of some functions.
  char_type* _CS_begin() const { return _CS_gcur; }
  char_type* _CS_end() const { return _CS_gend; }
  char_type* _CS_gnext() { return ++_CS_gcur; }
  int_type   _CS_gbump() { return _CS_traits::to_int_type(*_CS_gcur++); }
  void       _CS_commit(char_type* _CS_c) { _CS_gcur = _CS_c; }
  int_type   _CS_uflow() { return uflow(); }
  int_type   _CS_underflow() { return underflow(); }

  char_type* _CS_pptr()                  { return _CS_pcur; }
  char_type* _CS_epptr()                 { return _CS_pend; }
  int_type   _CS_sputc(char_type _CS_c)  { return sputc(_CS_traits::to_int_type(_CS_c)); }
  void       _CS_pbump(streamsize _CS_n) { pbump(_CS_n); }

protected:
  basic_streambuf();

  char_type* eback() const;
  char_type* gptr() const;
  char_type* egptr() const;
  void       gbump(int);
  void       setg(char_type*, char_type*, char_type*);

  char_type *pbase() const;
  char_type *pptr() const;
  char_type *epptr() const;
  void      pbump(int _CS_n)             { _CS_pcur += _CS_n; }
  void      setp(char_type*, char_type*);

  virtual void imbue(locale const&);

  virtual basic_streambuf<_CS_cT, _CS_traits>* setbuf(char_type*, streamsize);

  virtual pos_type seekoff(off_type,
         ios_base::seekdir,
         ios_base::openmode = ios_base::out | ios_base::in);
  virtual pos_type seekpos(pos_type,
         ios_base::openmode = ios_base::out | ios_base::in);
  virtual int sync();

  virtual streamsize showmanyc();
  virtual streamsize xsgetn(char_type*, streamsize );
  virtual int_type   underflow();
  virtual int_type   uflow();

  virtual int_type pbackfail(int_type = _CS_traits::eof());

  virtual streamsize xsputn(char_type const*, streamsize );
  virtual int_type   overflow(int_type = _CS_traits::eof());

private:
  char_type* _CS_gbeg;
  char_type* _CS_gcur;
  char_type* _CS_gend;
  char_type* _CS_pbeg;
  char_type* _CS_pcur;
  char_type* _CS_pend;
  locale*    _CS_loc;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_IOSTREAM_IMPL) && !defined(__SRCCXX_STREAMBUF_CC__)
#  include <srccxx/streambuf.cc>
#endif

// --------------------------------------------------------------------------

#endif /* _CXXRT_STREAMBUF_H__ */
