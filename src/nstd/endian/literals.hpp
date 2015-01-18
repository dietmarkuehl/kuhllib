// nstd/endian/literals.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_ENDIAN_LITERALS
#define INCLUDED_NSTD_ENDIAN_LITERALS

#include "nstd/endian/type.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace endian {
        inline namespace literals {
            auto constexpr operator"" _eu(unsigned long long)
                -> ::nstd::endian::type;
            auto constexpr operator"" _es(unsigned long long)
                -> ::nstd::endian::type;
            auto constexpr operator"" _ef(unsigned long long)
                -> ::nstd::endian::type;
        }
    }
}

// ----------------------------------------------------------------------------

auto constexpr nstd::endian::literals::operator"" _eu(unsigned long long value)
    -> ::nstd::endian::type {
    return value == 8u?  ::nstd::endian::type::uint8
        :  value == 16u? ::nstd::endian::type::uint16
        :  value == 32u? ::nstd::endian::type::uint32
        :  value == 64u? ::nstd::endian::type::uint64
        : ::nstd::endian::type{};
}

auto constexpr nstd::endian::literals::operator"" _es(unsigned long long value)
    -> ::nstd::endian::type {
    return value == 8u?  ::nstd::endian::type::int8
        :  value == 16u? ::nstd::endian::type::int16
        :  value == 32u? ::nstd::endian::type::int32
        :  value == 64u? ::nstd::endian::type::int64
        : ::nstd::endian::type{};
}

auto constexpr nstd::endian::literals::operator"" _ef(unsigned long long value)
    -> ::nstd::endian::type {
    return value == 32u? ::nstd::endian::type::float32
        :  value == 64u? ::nstd::endian::type::float64
        : ::nstd::endian::type{};
}

// ----------------------------------------------------------------------------

#endif
