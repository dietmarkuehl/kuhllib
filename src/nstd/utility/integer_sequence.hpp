// nstd/utility/integer_sequence.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_UTILITY_INTEGER_SEQUENCE
#define INCLUDED_NSTD_UTILITY_INTEGER_SEQUENCE

#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace utility {
        template <typename T, T...> struct integer_sequence;

        namespace detail {
            template <typename, std::size_t> struct combine_integer_sequence;
            template <typename T, T... Values, std::size_t Value>
            struct combine_integer_sequence<nstd::utility::integer_sequence<T, Values...>, Value>;

            template <typename T, std::size_t> struct make_integer_sequence;
            template <typename T> struct make_integer_sequence<T, 0u>;
        }
        template <std::size_t... Values>
        using index_sequence = nstd::utility::integer_sequence<std::size_t, Values...>;

        template <typename T, T N>
        using make_integer_sequence = typename nstd::utility::detail::make_integer_sequence<T, N>::type;
        template <std::size_t N>
        using make_index_sequence = nstd::utility::make_integer_sequence<std::size_t, N>;
        template <typename... T>
        using index_sequence_for = nstd::utility::make_integer_sequence<std::size_t, sizeof...(T)>;
    }

}

// ----------------------------------------------------------------------------

template <typename T, T... Values, std::size_t Value>
struct nstd::utility::detail::combine_integer_sequence<nstd::utility::integer_sequence<T, Values...>, Value> {
    using type = nstd::utility::integer_sequence<T, Values..., Value>;
};

template <typename T, std::size_t Size>
struct nstd::utility::detail::make_integer_sequence {
    using type = typename nstd::utility::detail::combine_integer_sequence<nstd::utility::make_integer_sequence<T, Size - 1u>,
                                                                          Size - 1u>::type;
};

template <typename T>
struct nstd::utility::detail::make_integer_sequence<T, 0u> {
    using type = nstd::utility::integer_sequence<T>;
};

// ----------------------------------------------------------------------------

template <typename T, T... Values>
struct nstd::utility::integer_sequence {
    typedef T value_type;
    static constexpr std::size_t size() { return sizeof...(Values); }
};

// ----------------------------------------------------------------------------

#endif
