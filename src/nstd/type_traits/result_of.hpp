// nstd/type_traits/result_of.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_RESULT_OF
#define INCLUDED_NSTD_TYPE_TRAITS_RESULT_OF

#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/is_member_pointer.hpp"
#include "nstd/type_traits/declval.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace type_traits {
        namespace detail {
            template <typename... Args>
            struct result_of_test {
                template <typename F,
                          typename = decltype(::nstd::type_traits::declval<F>()(::nstd::type_traits::declval<Args>()...), 0)>
                static ::nstd::type_traits::true_type test(int);
                template <typename F,
                          typename = typename ::nstd::type_traits::enable_if< ::nstd::type_traits::is_member_pointer<F>::value>::type>
                static ::nstd::type_traits::true_type test(short);
                template <typename F>
                static ::nstd::type_traits::false_type test(...);
            };
            template <bool, typename> struct result_of;
            template <typename Fun, typename... Args> struct result_of<true, Fun(Args...)>;
            template <typename T, typename S, typename Object, typename... Args> struct result_of<true, auto(Object, Args...) -> T S::*>;
            template <typename T> struct result_of<false, T>;
        }
        template <typename> class result_of;
        template <typename Fun, typename... Args> class result_of<Fun(Args...)>;
        template <typename T>
        using result_of_t = typename ::nstd::type_traits::result_of<T>::type;
    }

}

// ----------------------------------------------------------------------------

template <typename T>
struct ::nstd::type_traits::detail::result_of<false, T> {
};
template <typename Fun, typename... Args>
struct ::nstd::type_traits::detail::result_of<true, Fun(Args...)> {
    using type = decltype(::nstd::type_traits::declval<Fun>()(::nstd::type_traits::declval<Args>()...));
};
template <typename T, typename S, typename Object, typename... Args>
struct ::nstd::type_traits::detail::result_of<true, auto(Object, Args...) -> T S::*> {
    using type = decltype((::nstd::type_traits::declval<Object>().*::nstd::type_traits::declval<T S::*>())(::nstd::type_traits::declval<Args>()...));
};

// ----------------------------------------------------------------------------

template <typename Fun, typename... Args>
class ::nstd::type_traits::result_of<Fun(Args...)>
    : public ::nstd::type_traits::detail::result_of<
            decltype(::nstd::type_traits::detail::result_of_test<Args...>::template test<Fun>(0))::value,
        Fun(Args...)> {
};

// ----------------------------------------------------------------------------

#endif
