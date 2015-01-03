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

// ----------------------------------------------------------------------------

namespace nstd {
    class ios_base;
}

// ----------------------------------------------------------------------------

class nstd::ios_base {
public:
    class failure;
    using fmtflags = unsigned long;
    static constexpr fmtflags left       {0x0001};
    static constexpr fmtflags right      {0x0002};
    static constexpr fmtflags internal   {0x0003};
    static constexpr fmtflags adjustfield{0x0003};

    static constexpr fmtflags dec        {0x0004};
    static constexpr fmtflags hex        {0x0008};
    static constexpr fmtflags oct        {0x000C};
    static constexpr fmtflags basefield  {0x000C};

    static constexpr fmtflags fixed      {0x0010};
    static constexpr fmtflags scientific {0x0020};
    static constexpr fmtflags floatfield {0x0030};

    static constexpr fmtflags boolalpha  {0x0040};
    static constexpr fmtflags showbase   {0x0080};
    static constexpr fmtflags showpoint  {0x0100};
    static constexpr fmtflags showpos    {0x0200};
    static constexpr fmtflags skipws     {0x0400};
    static constexpr fmtflags unitbuf    {0x0800};
    static constexpr fmtflags uppercase  {0x1000};

    using iostate = unsigned long;
    static constexpr iostate goodbit  {0x0000};
    static constexpr iostate failbit  {0x2000};
    static constexpr iostate badbit   {0x4000};
    static constexpr iostate eofbit   {0x8000};

    using openmode = unsigned int;
    static constexpr openmode app{0x01};
    static constexpr openmode ate{0x02};
    static constexpr openmode binary{0x04};
    static constexpr openmode in{0x08};
    static constexpr openmode out{0x10};
    static constexpr openmode trunc{0x20};

    using seekdir = int;
    static constexpr seekdir beg{1};
    static constexpr seekdir cur{2};
    static constexpr seekdir end{3};

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
    failure(::nstd::string const&, ::nstd::error_code const& = ::nstd::io_errc::stream);
};

// ----------------------------------------------------------------------------

#endif
