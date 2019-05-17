// -*-C++-*- cxxrt/ios_base.h
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
// Title:   Declarations for ios_base, the absolute IOStreams base 
// Version: $Id: ios_base.h,v 1.3 2003/04/13 19:35:59 dietmar_kuehl Exp $

// TODO: replace _CS_map by a better approach...
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_IOS_BASE_H__)
#define _CXXRT_IOS_BASE_H__ 1

#if !defined(_CXXRTCF_H__)
#  include <cxxrtcf.h>
#endif
#if !defined(__EXCEPTION__)
// #  include _CXXRT_EXCEPTION
#  include <exception>
#endif
#if !defined(_CXXRT_BSTR_FWD_H__)
#  include <cxxrt/bstr_fwd.h>
#endif
#if !defined(_CXXRT_DKMAP_H__)
#  include <cxxrt/dkmap.h>
#endif
#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif
#if !defined(__CWCHAR__)
#  include <cwchar>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

#if defined(_CRT_STD)
  using _CRT_STD mbstate_t;
  using _CRT_STD wint_t;
#endif

// --------------------------------------------------------------------------

template <class _CS_state> class fpos;

// --------------------------------------------------------------------------

class ios_base
{
public:
  typedef streamsize      streamoff;
  typedef fpos<mbstate_t> streampos;

  enum _CS_stdstream
  {
    _CS_stdin  = 0,
    _CS_stdout = 1,
    _CS_stderr = 2,
    _CS_stdlog = 3
  };

  class failure: public std::exception
  {
  public:
    explicit failure(char const*);
    explicit failure(string const&);
    failure(failure const&);
    failure& operator= (failure const&);
    virtual ~failure() throw();
    virtual char const* what() const throw();

    private:
      char const* _CS_message;
  };

  enum fmtflags
  {
    _CS_oct        = 0x0007, // base - 1 == 7  == 0x7
    _CS_dec        = 0x0009, // base - 1 == 9  == 0x9
    _CS_hex        = 0x000f, // base - 1 == 15 == 0xf
    _CS_internal   = 0x0010,
    _CS_left       = 0x0020,
    _CS_right      = 0x0040,
    _CS_fixed      = 0x0080,
    _CS_scientific = 0x0100,
    _CS_showbase   = 0x0200,
    _CS_showpoint  = 0x0400,
    _CS_showpos    = 0x0800,
    _CS_skipws     = 0x1000,
    _CS_unitbuf    = 0x2000,
    _CS_uppercase  = 0x4000,
    _CS_boolalpha  = 0x8000,

    _CS_padzero    = 0x10000,
    _CS_showspace  = 0x20000,
    _CS_hexscience = 0x40000,

    _CS_adjustfield = 0x0070,
    _CS_basefield   = 0x000f,
    _CS_bfloatfield = 0x00180,
    _CS_floatfield  = 0x40180,
    _CS_os_good     = 0x100000,
    _CS_os_tie      = 0x200000,
    _CS_os_check    = 0x302000,
    _CS_is_check    = 0x300000,
    _CS_fmtmask     = 0x00ffff,
    _CS_fmtall      = 0xffffff

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 0
    ,
    oct         = _CS_oct,
    dec         = _CS_dec,
    hex         = _CS_hex,
    internal    = _CS_internal,
    left        = _CS_left,
    right       = _CS_right,
    fixed       = _CS_fixed,
    scientific  = _CS_scientific,
    showbase    = _CS_showbase,
    showpoint   = _CS_showpoint,
    showpos     = _CS_showpos,
    skipws      = _CS_skipws,
    unitbuf     = _CS_unitbuf,
    uppercase   = _CS_uppercase,
    boolalpha   = _CS_boolalpha,
    adjustfield = _CS_adjustfield,
    basefield   = _CS_basefield,
    floatfield  = _CS_bfloatfield
#endif
  };

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
  static fmtflags const oct         = _CS_oct;
  static fmtflags const dec         = _CS_dec;
  static fmtflags const hex         = _CS_hex;
  static fmtflags const internal    = _CS_internal;
  static fmtflags const left        = _CS_left;
  static fmtflags const right       = _CS_right;
  static fmtflags const fixed       = _CS_fixed;
  static fmtflags const scientific  = _CS_scientific;
  static fmtflags const showbase    = _CS_showbase;
  static fmtflags const showpoint   = _CS_showpoint;
  static fmtflags const showpos     = _CS_showpos;
  static fmtflags const skipws      = _CS_skipws;
  static fmtflags const unitbuf     = _CS_unitbuf;
  static fmtflags const uppercase   = _CS_uppercase;
  static fmtflags const boolalpha   = _CS_boolalpha;
  static fmtflags const adjustfield = _CS_adjustfield;
  static fmtflags const basefield   = _CS_basefield;
  static fmtflags const floatfield  = _CS_bfloatfield;
#endif

  enum iostate
  {
    _CS_goodbit = 0x00,
    _CS_badbit  = 0x01,
    _CS_eofbit  = 0x02,
    _CS_failbit = 0x04,
    _CS_iosall = 0x07

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 0
    ,
    goodbit = _CS_goodbit,
    badbit  = _CS_badbit,
    eofbit  = _CS_eofbit,
    failbit = _CS_failbit
#endif
  };
  typedef unsigned int io_state;

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
  static iostate const goodbit = _CS_goodbit;
  static iostate const badbit  = _CS_badbit;
  static iostate const eofbit  = _CS_eofbit;
  static iostate const failbit = _CS_failbit;
#endif

  enum  openmode
  {
    _CS_in              = 0x01,
    _CS_out             = 0x02,
    _CS_inout = 0x03,
    _CS_app             = 0x04,
    _CS_ate             = 0x08,
    _CS_trunc           = 0x10,
    _CS_binary          = 0x20,
    // bin          = 0x20, // backward compatibility?! standard C++: binary
    _CS_omall = 0x3f
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 0
    ,
    in     = _CS_in,
    out    = _CS_out,
    app    = _CS_app,
    ate    = _CS_ate,
    trunc  = _CS_trunc,
    binary = _CS_binary
#endif
  };
  typedef unsigned int open_mode;

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
  static openmode const in     = _CS_in;
  static openmode const out    = _CS_out;
  static openmode const app    = _CS_app;
  static openmode const ate    = _CS_ate;
  static openmode const trunc  = _CS_trunc;
  static openmode const binary = _CS_binary;
#endif

  enum seekdir
  {
    _CS_beg,
    _CS_cur,
    _CS_end
#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 0
    ,
    beg = _CS_beg,
    cur = _CS_cur,
    end = _CS_end
#endif
  };
  typedef unsigned int seek_dir;

#if _CXXRT_HAS_CONST_EXPRESSION_MEMBER == 1
  static seekdir const beg = _CS_beg;
  static seekdir const cur = _CS_cur;
  static seekdir const end = _CS_end;
#endif

  enum event
  {
    erase_event,
    imbue_event,
    copyfmt_event
  };

  typedef void (*event_callback)(event, ios_base&, int);

  class Init
  {
  public:
    Init();
    ~Init();

  private:
    Init(Init const&);
    void operator=(Init const&);
    static unsigned int _CS_count;
  };


  fmtflags flags() const;
  fmtflags flags(fmtflags);
  fmtflags setf(fmtflags);
  fmtflags setf(fmtflags, fmtflags);
  fmtflags unsetf(fmtflags);

  streamsize precision() const;
  streamsize precision(streamsize);
  
  streamsize width() const;
  streamsize width(streamsize);
  void _CS_width(streamsize _CS_w) { _CS_m_width = _CS_w; }

  static int xalloc();
  long&  iword(int);
  void*& pword(int);

  virtual ~ios_base();

  locale imbue(locale const&);
  locale getloc() const;
  locale const& _CS_getloc() const; // { return _CS_loc; }

  void register_callback(event_callback, int);
  static bool sync_with_stdio(bool = true);

  bool _CS_need_osentry() const { return (_CS_flags & _CS_os_check) != 0; }
  bool _CS_need_isentry() const { return (_CS_flags & _CS_is_check) != 0; }
protected:
  ios_base();

  ios_base& _CS_copyfmt(ios_base const&);
  void _CS_call_callbacks(event);

private:
  ios_base(ios_base const&);        // deny access and do not implement
  void operator= (ios_base const&); // deny access and do not implement

  fmtflags    _CS_flags;
  // fmtflags    _CS_need_osentry;
private:
  streamsize  _CS_precision;
  streamsize  _CS_m_width;
  locale      _CS_loc;

public:
  struct _CS_callback;
private:
  _CS_callback* _CS_list;

  struct _CS_word_rec
  {
    long  _CS_iword;
    void* _CS_pword;

    _CS_word_rec():
      _CS_iword(0),
      _CS_pword(0)
    {
    }
    _CS_word_rec(_CS_word_rec const& _CS_wr):
      _CS_iword(_CS_wr._CS_iword),
      _CS_pword(_CS_wr._CS_pword)
    {
    }
    _CS_word_rec& operator= (_CS_word_rec const& _CS_wr)
    {
      _CS_iword = _CS_wr._CS_iword;
      _CS_pword = _CS_wr._CS_pword;
      return *this;
    }
    ~_CS_word_rec() {}
  };
  _CS_map<int, _CS_word_rec> _CS_words; //-dk:TODO
};

// ---------------------------------------------------------------------------
// Turn the enums 'ios_base::fmtflags', 'ios_base::iostate', and
// 'ios::openmode' into bitmask types:

_CXXRT_BITMASK(ios_base::fmtflags, ios_base::_CS_fmtall)
_CXXRT_BITMASK(ios_base::iostate, ios_base::_CS_iosall)
_CXXRT_BITMASK(ios_base::openmode, ios_base::_CS_omall)

// ---------------------------------------------------------------------------

#if !defined(_CXXRT_MIN_IO)

inline ios_base::fmtflags
ios_base::flags() const
{
  return _CS_flags & _CS_fmtmask;
}

inline streamsize
ios_base::precision() const
{
  return _CS_precision;
}

inline streamsize
ios_base::width() const
{
  return _CS_m_width;
}

#endif

// --------------------------------------------------------------------------

#define _CXXRT_MANIPULATOR(_CXXRT_flag)                 \
  inline ios_base &_CXXRT_flag(ios_base &_CS_fmt)       \
  {                                                     \
    _CS_fmt.setf(ios_base::_CXXRT_flag);                \
    return _CS_fmt;                                     \
  }                                                     \
  inline ios_base &no ## _CXXRT_flag(ios_base &_CS_fmt) \
  {                                                     \
    _CS_fmt.unsetf(ios_base::_CXXRT_flag);              \
    return _CS_fmt;                                     \
  }

_CXXRT_MANIPULATOR(boolalpha)
_CXXRT_MANIPULATOR(showbase)
_CXXRT_MANIPULATOR(showpoint)
_CXXRT_MANIPULATOR(showpos)
_CXXRT_MANIPULATOR(skipws)
_CXXRT_MANIPULATOR(unitbuf)
_CXXRT_MANIPULATOR(uppercase)

#undef _CXXRT_MANIPULATOR

#define _CXXRT_MANIPULATOR(_CXXRT_flag,_CXXRT_field)                  \
  inline ios_base &_CXXRT_flag(ios_base &_CS_fmt)                   \
  {                                                                         \
    _CS_fmt.setf(ios_base::_CXXRT_flag, ios_base::_CXXRT_field);  \
    return _CS_fmt;                                                   \
  }

_CXXRT_MANIPULATOR(internal, adjustfield)
_CXXRT_MANIPULATOR(left, adjustfield)
_CXXRT_MANIPULATOR(right, adjustfield)
_CXXRT_MANIPULATOR(oct, basefield)
_CXXRT_MANIPULATOR(dec, basefield)
_CXXRT_MANIPULATOR(hex, basefield)
_CXXRT_MANIPULATOR(fixed, floatfield)
_CXXRT_MANIPULATOR(scientific, floatfield)

#undef _CXXRT_MANIPULATOR

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_IOS_BASE_H__ */
