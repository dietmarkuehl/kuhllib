// toy-buffer.hpp                                                     -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_TOY_BUFFER
#define INCLUDED_TOY_BUFFER

#include <array>
#include <concepts>
#include <type_traits>
#include <sys/socket.h>

// ----------------------------------------------------------------------------

namespace toy {
    template <typename B>
    concept byte_type
        =   std::same_as<std::remove_cv_t<B>, char>
        ||  std::same_as<std::remove_cv_t<B>, signed char>
        ||  std::same_as<std::remove_cv_t<B>, unsigned char>
        ||  std::same_as<std::remove_cv_t<B>, std::byte>
        ;
    template <typename B>
    concept byte_array
        =   std::is_bounded_array_v<std::remove_cvref_t<B>>
        &&  toy::byte_type<std::remove_extent_t<std::remove_cvref_t<B>>>
        ;

    template <toy::byte_array... B>
    std::array<::iovec, sizeof...(B)> buffer(B&&... b) {
        return std::array<::iovec, sizeof...(B)>{
            ::iovec{
                .iov_base = const_cast<char*>(reinterpret_cast<char const*>(b)),
                .iov_len = ::std::extent_v<std::remove_cvref_t<B>>
            }...
        };
    }
    template <toy::byte_type B>
    std::array<::iovec, 1> buffer(B* b, std::size_t n) {
        return std::array<::iovec, 1>{
            ::iovec{
                .iov_base = const_cast<char*>(reinterpret_cast<char const*>(b)),
                .iov_len  = n
            }
        };
    }
}

// ----------------------------------------------------------------------------

#endif
