// nstd/functional/placeholders.hpp                                   -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_PLACEHOLDERS
#define INCLUDED_NSTD_FUNCTIONAL_PLACEHOLDERS

#include "nstd/type_traits/integral_constant.hpp"
#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        namespace detail {
            template <std::size_t N>
            struct placeholder
                : nstd::type_traits::integral_constant<std::size_t, N> {
            };
        }
        namespace placeholders {
            constexpr nstd::functional::detail::placeholder<1> _1{};
            constexpr nstd::functional::detail::placeholder<2> _2{};
            constexpr nstd::functional::detail::placeholder<3> _3{};
            constexpr nstd::functional::detail::placeholder<4> _4{};
            constexpr nstd::functional::detail::placeholder<5> _5{};
            constexpr nstd::functional::detail::placeholder<6> _6{};
            constexpr nstd::functional::detail::placeholder<7> _7{};
            constexpr nstd::functional::detail::placeholder<8> _8{};
            constexpr nstd::functional::detail::placeholder<9> _9{};
        }
        template <typename T> struct is_placeholder;
        template <std::size_t N> struct is_placeholder<nstd::functional::detail::placeholder<N>>;
        template <std::size_t N> struct is_placeholder<nstd::functional::detail::placeholder<N> const>;
    }

}

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::functional::is_placeholder
    : nstd::type_traits::integral_constant<int, 0> {
};

template <std::size_t N>
struct nstd::functional::is_placeholder<nstd::functional::detail::placeholder<N>>
    : nstd::type_traits::integral_constant<int, N> {
};

template <std::size_t N>
struct nstd::functional::is_placeholder<nstd::functional::detail::placeholder<N> const>
    : nstd::type_traits::integral_constant<int, N> {
};

// ----------------------------------------------------------------------------

#endif
