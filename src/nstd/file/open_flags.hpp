// nstd/file/open_flags.hpp                                           -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_FILE_OPEN_FLAGS
#define INCLUDED_NSTD_FILE_OPEN_FLAGS

#include <iosfwd>

// ----------------------------------------------------------------------------

namespace nstd::file {
    enum class open_flags: unsigned int {
        // note: these are mapped to the real values.
        none       = 0x00000,
        in         = 0x00001,
        rdonly     = in,
        out        = 0x00002,
        wronly     = out,
        inout      = 0x00003,
        rdwr       = inout,

        append     = 0x00004,
        trunc      = 0x00008,
        create     = 0x00010,

        async      = 0x00020,
        cloexec    = 0x00040,
        close_exec = cloexec,
        direct     = 0x00080,
        directory  = 0x00100,
        dsync      = 0x00200,
        excl       = 0x00400,
        largefile  = 0x00800,
        noattime   = 0x01000,
        noctty     = 0x02000,
        nofollow   = 0x04000,
        nonblock   = 0x08000,
        ndelay     = nonblock,
        path       = 0x10000,
        sync       = 0x20000,
        tmpfile    = 0x40000,

        all        = 0x7FFFF
    };

    // open_flags is a bitmask type
    template <typename Op>
    constexpr auto apply_binary_op(::nstd::file::open_flags, ::nstd::file::open_flags, Op)
        -> ::nstd::file::open_flags;
    constexpr auto operator~  (::nstd::file::open_flags) -> ::nstd::file::open_flags;
    constexpr auto operator|= (::nstd::file::open_flags&, ::nstd::file::open_flags) -> ::nstd::file::open_flags&;
    constexpr auto operator^= (::nstd::file::open_flags&, ::nstd::file::open_flags) -> ::nstd::file::open_flags&;
    constexpr auto operator&= (::nstd::file::open_flags&, ::nstd::file::open_flags) -> ::nstd::file::open_flags&;
    constexpr auto operator|  (::nstd::file::open_flags,  ::nstd::file::open_flags) -> ::nstd::file::open_flags;
    constexpr auto operator^  (::nstd::file::open_flags,  ::nstd::file::open_flags) -> ::nstd::file::open_flags;
    constexpr auto operator&  (::nstd::file::open_flags,  ::nstd::file::open_flags) -> ::nstd::file::open_flags;

    int to_system(::nstd::file::open_flags);
    ::std::ostream& operator<< (::std::ostream&, ::nstd::file::open_flags);
}

// ----------------------------------------------------------------------------

template <typename Op>
inline constexpr auto
nstd::file::apply_binary_op(::nstd::file::open_flags f0,
                            ::nstd::file::open_flags f1,
                            Op                       op)
    -> ::nstd::file::open_flags
{
    return ::nstd::file::open_flags(op(static_cast<unsigned int>(f0),
                                       static_cast<unsigned int>(f1)));
}

inline constexpr auto
nstd::file::operator~ (::nstd::file::open_flags flags)
    -> nstd::file::open_flags
{
    return ::nstd::file::open_flags(~static_cast<unsigned int>(flags));
}

inline constexpr auto
nstd::file::operator|= (::nstd::file::open_flags& f0, ::nstd::file::open_flags f1)
    -> ::nstd::file::open_flags&
{
    return f0 = ::nstd::file::apply_binary_op(f0, f1,
                                              [](auto x, auto y){ return x | y; });
}

inline constexpr auto
nstd::file::operator^= (::nstd::file::open_flags& f0, ::nstd::file::open_flags f1)
    -> ::nstd::file::open_flags&
{
    return f0 = ::nstd::file::apply_binary_op(f0, f1,
                                              [](auto x, auto y){ return x ^ y; });
}

inline constexpr auto
nstd::file::operator&= (::nstd::file::open_flags& f0, ::nstd::file::open_flags f1)
    -> ::nstd::file::open_flags&
{
    return f0 = ::nstd::file::apply_binary_op(f0, f1,
                                              [](auto x, auto y){ return x & y; });
}

inline constexpr auto
nstd::file::operator| (::nstd::file::open_flags f0, ::nstd::file::open_flags f1)
    -> ::nstd::file::open_flags
{
    return ::nstd::file::apply_binary_op(f0, f1,
                                         [](auto x, auto y){ return x | y; });
}

inline constexpr auto
nstd::file::operator^ (::nstd::file::open_flags f0, ::nstd::file::open_flags f1)
    -> ::nstd::file::open_flags
{
    return ::nstd::file::apply_binary_op(f0, f1,
                                         [](auto x, auto y){ return x ^ y; });
}

inline constexpr auto
nstd::file::operator& (::nstd::file::open_flags f0, ::nstd::file::open_flags f1)
    -> ::nstd::file::open_flags
{
    return ::nstd::file::apply_binary_op(f0, f1,
                                         [](auto x, auto y){ return x & y; });
}

// ----------------------------------------------------------------------------

#endif
