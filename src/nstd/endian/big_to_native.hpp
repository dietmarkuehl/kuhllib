// nstd/endian/big_to_native.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_NSTD_ENDIAN_BIG_TO_NATIVE
#define INCLUDED_NSTD_ENDIAN_BIG_TO_NATIVE

#include "nstd/endian/type.hpp"
#include <limits>
#include <stdexcept>
#include <utility>
#include <string.h>
#include <stdint.h>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace endian {
        namespace detail {
            template <bool> struct big_to_native;
            template <> struct big_to_native<true>;
            template <> struct big_to_native<false>;
        }
        template < ::nstd::endian::type T, typename InIt>
        auto big_to_native(InIt, InIt)
            -> std::pair<::nstd::endian::type_map_t<T>, InIt>;
        template < ::nstd::endian::type... T, typename InIt>
        auto big_to_native(InIt, InIt, ::nstd::endian::type_map_t<T>&...)
            -> InIt;
    }
}

// ----------------------------------------------------------------------------

template <>
struct nstd::endian::detail::big_to_native<true> {
    template < ::nstd::endian::type T, typename InIt>
    static auto convert(InIt it, InIt end)
        -> std::pair<::nstd::endian::type_map_t<T>, InIt> {
        //-dk:TODO this is a rather naive implementation
        using utype = typename std::make_unsigned<::nstd::endian::type_map_t<T> >::type;
        utype value{};
        for (std::size_t i{0}; i != sizeof(utype); ++i, ++it) {
            if (it == end) {
                throw std::range_error("incomplete big to native");
            }
            value = (value << 8u) | uint8_t(*it);
        }
        return std::make_pair(::nstd::endian::type_map_t<T>(value), it);
    }
};

template <>
struct nstd::endian::detail::big_to_native<false> {
    template < ::nstd::endian::type T, typename InIt>
    static auto convert(InIt it, InIt end)
        -> std::pair<::nstd::endian::type_map_t<T>, InIt> {
        //-dk:TODO this implementation is extremely naive
        using type = ::nstd::endian::type_map_t<T>;
        char buffer[sizeof(type)];
        for (std::size_t i{sizeof(type)}; i-- != 0u; ++it) {
            if (it == end) {
                throw std::range_error("incomplete big to native");
            }
            buffer[i] = *it;
        }
        type rc;
        memcpy(&rc, buffer, sizeof(type));
        return std::make_pair(rc, it);
    }
};

// ----------------------------------------------------------------------------

template < ::nstd::endian::type T, typename InIt>
auto nstd::endian::big_to_native(InIt it, InIt end)
    -> std::pair<::nstd::endian::type_map_t<T>, InIt> {
        using type = ::nstd::endian::type_map_t<T>;
        using detail = ::nstd::endian::detail::big_to_native<std::numeric_limits<type>::is_integer>;
        return detail::template convert<T>(it, end);
}

// ----------------------------------------------------------------------------

#endif
