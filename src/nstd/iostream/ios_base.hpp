// nstd/iostream/ios_base.hpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
//                                                                       
//  Permission is hereby granted, free of charge, to any person          
//  obtaining a copy of this software and associated documentation       
//  files (the "Software"), to deal in the Software without restriction, 
//  including without limitation the rights to use, copy, modify,        
//  merge, publish, distribute, sublicense, and/or sell copies of        
//  the Software, and to permit persons to whom the Software is          
//  furnished to do so, subject to the following conditions:             
//                                                                       
//  The above copyright notice and this permission notice shall be       
//  included in all copies or substantial portions of the Software.      
//                                                                       
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING         
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR        
//  OTHER DEALINGS IN THE SOFTWARE. 
// ----------------------------------------------------------------------------

#ifndef INCLUDED_NSTD_IOSTREAM_IOS_BASE
#define INCLUDED_NSTD_IOSTREAM_IOS_BASE

#include "nstd/iostream/io_types.hpp"
#include "nstd/locale/locale_fwd.hpp"
#include "nstd/string/string_fwd.hpp"
#include "nstd/system_error/system_error.hpp"
#include "nstd/utility/bitmask.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    enum class fmtflags: unsigned short;
    enum class iostate: unsigned short;
    enum class openmode: unsigned short;
    template <> struct bitmask<fmtflags> { using type = unsigned short; };
    template <> struct bitmask<iostate> { using type = unsigned short; };
    template <> struct bitmask<openmode> { using type = unsigned short; };
    class ios_base;
}

// ----------------------------------------------------------------------------

enum class nstd::fmtflags: unsigned short
{
    left        = 0x0001,
    right       = 0x0002,
    internal    = 0x0003,
    adjustfield = 0x0003,

    dec         = 0x0004,
    hex         = 0x0008,
    oct         = 0x000C,
    basefield   = 0x000C,

    fixed       = 0x0010,
    scientific  = 0x0020,
    floatfield  = 0x0030,

    boolalpha   = 0x0040,
    showbase    = 0x0080,
    showpoint   = 0x0100,
    showpos     = 0x0200,
    skipws      = 0x0400,
    unitbuf     = 0x0800,
    uppercase   = 0x1000
};

enum class nstd::iostate: unsigned short {
    goodbit = 0x0000,
    failbit = 0x2000,
    badbit  = 0x4000,
    eofbit  = 0x8000
};

enum class nstd::openmode: unsigned short {
    app    = 0x01,
    ate    = 0x02,
    binary = 0x04,
    in     = 0x08,
    out    = 0x10,
    trunc  = 0x20
};

// ----------------------------------------------------------------------------

class nstd::ios_base {
public:
    class failure;

    using fmtflags = ::nstd::fmtflags;
    static constexpr fmtflags left{::nstd::fmtflags::left};
    static constexpr fmtflags right{::nstd::fmtflags::right};
    static constexpr fmtflags internal{::nstd::fmtflags::internal};
    static constexpr fmtflags adjustfield{::nstd::fmtflags::adjustfield};
    static constexpr fmtflags dec{::nstd::fmtflags::dec};
    static constexpr fmtflags hex{::nstd::fmtflags::hex};
    static constexpr fmtflags oct{::nstd::fmtflags::oct};
    static constexpr fmtflags basefield{::nstd::fmtflags::basefield};
    static constexpr fmtflags fixed{::nstd::fmtflags::fixed};
    static constexpr fmtflags scientific{::nstd::fmtflags::scientific};
    static constexpr fmtflags floatfield{::nstd::fmtflags::floatfield};
    static constexpr fmtflags boolalpha{::nstd::fmtflags::boolalpha};
    static constexpr fmtflags showbase{::nstd::fmtflags::showbase};
    static constexpr fmtflags showpoint{::nstd::fmtflags::showpoint};
    static constexpr fmtflags showpos{::nstd::fmtflags::showpos};
    static constexpr fmtflags skipws{::nstd::fmtflags::skipws};
    static constexpr fmtflags unitbuf{::nstd::fmtflags::unitbuf};
    static constexpr fmtflags uppercase{::nstd::fmtflags::uppercase};

    using iostate = ::nstd::iostate;
    static constexpr iostate goodbit{::nstd::iostate::goodbit};
    static constexpr iostate failbit{::nstd::iostate::failbit};
    static constexpr iostate badbit{::nstd::iostate::badbit};
    static constexpr iostate eofbit{::nstd::iostate::eofbit};

    using openmode = ::nstd::openmode;
    static constexpr openmode app{::nstd::openmode::app};
    static constexpr openmode ate{::nstd::openmode::ate};
    static constexpr openmode binary{::nstd::openmode::binary};
    static constexpr openmode in{::nstd::openmode::in};
    static constexpr openmode out {::nstd::openmode::out };
    static constexpr openmode trunc {::nstd::openmode::trunc };

    enum seekdir {
        beg,
        cur,
        end
    };

    class Init;

protected:
    ios_base();

public:
    ios_base(ios_base const&) = delete;
    ios_base& operator=(ios_base const&) = delete;
    virtual ~ios_base();

    auto flags() const -> fmtflags;
    auto flags(fmtflags) -> fmtflags;
    auto setf(fmtflags) -> fmtflags;
    auto setf(fmtflags, fmtflags) -> fmtflags;
    auto unsetf(fmtflags) -> fmtflags;

    auto precision() const -> ::nstd::streamsize;
    auto precision(::nstd::streamsize) -> ::nstd::streamsize;
    auto width() const -> ::nstd::streamsize;
    auto width(::nstd::streamsize) -> ::nstd::streamsize;

    auto imbue(::nstd::locale const&) -> ::nstd::locale;
    auto getloc() const -> ::nstd::locale;

    static auto xalloc() -> int;
    auto iword(int) -> long&;
    auto pword(int) -> void*&;

    enum event { erase_event, imbue_event, copyfmt_event };
    using event_callback = void(*)(event, ::nstd::ios_base&, int);
    auto register_callback(event_callback, int) -> void;

    static auto sync_with_stdio(bool) -> bool;
};

// ----------------------------------------------------------------------------

class nstd::ios_base::failure
    : public ::nstd::system_error {
public:
    explicit failure(::nstd::string const&, ::nstd::error_code const& = ::nstd::io_errc::stream);
    explicit failure(char const*, ::nstd::error_code const& = ::nstd::io_errc::stream);
};

class nstd::ios_base::Init {
public:
    Init() = default;
    ~Init() = default;
};

// ----------------------------------------------------------------------------

#endif
