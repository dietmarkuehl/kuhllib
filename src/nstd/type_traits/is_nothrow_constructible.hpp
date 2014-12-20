// nstd/type_traits/is_nothrow_constructible.hpp                      -*-C++-*-
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE
#define INCLUDED_NSTD_TYPE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE

#include "nstd/type_traits/add_rvalue_reference.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/is_constructible.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace type_traits {
        namespace detail {
            template <bool, typename, typename...> struct is_nothrow_constructible;
            template <typename T, typename... Args> struct is_nothrow_constructible<true, T, Args...>;
            template <typename T, typename... Args> struct is_nothrow_constructible<false, T, Args...>;
        }
        template <typename, typename...> struct is_nothrow_constructible;
    }

}

// ----------------------------------------------------------------------------

template <typename T, typename... Args>
struct nstd::type_traits::detail::is_nothrow_constructible<true, T, Args...>
    : nstd::type_traits::integral_constant<bool,
        noexcept(T(nstd::type_traits::declval<nstd::type_traits::add_rvalue_reference_t<Args> >()...))> {
};

template <typename T, typename... Args>
struct nstd::type_traits::detail::is_nothrow_constructible<false, T, Args...>
    : nstd::type_traits::false_type {
};

// ----------------------------------------------------------------------------

template <typename T, typename... Args>
struct nstd::type_traits::is_nothrow_constructible
    : nstd::type_traits::integral_constant<bool,
        nstd::type_traits::detail::is_nothrow_constructible<
                     nstd::type_traits::is_constructible<T, Args...>::value, T, Args...>::value>
{
};

// ----------------------------------------------------------------------------

#endif
