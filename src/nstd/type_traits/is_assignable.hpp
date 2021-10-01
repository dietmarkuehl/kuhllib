// nstd/type_traits/is_assignable.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_IS_ASSIGNABLE
#define INCLUDED_NSTD_TYPE_TRAITS_IS_ASSIGNABLE

#include "nstd/type_traits/conditional.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/is_void.hpp"
#include "nstd/type_traits/remove_cv.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace type_traits {
        namespace detail {
            template <typename S, typename T,
                      typename = decltype(nstd::type_traits::declval<
                                              nstd::type_traits::conditional_t<
                                                  nstd::type_traits::is_void<
                                                      nstd::type_traits::remove_cv_t<S>
                                                      >::value, int const, S>
                                              >() = nstd::type_traits::declval<
                                                  nstd::type_traits::conditional_t<
                                                      nstd::type_traits::is_void<
                                                          nstd::type_traits::remove_cv_t<T>
                                                          >::value, int const, T>
                                           >())>
            auto test(int) -> nstd::type_traits::true_type;
            template <typename S, typename T>
            auto test(...) -> nstd::type_traits::false_type;
        }
        template <typename S, typename T> struct is_assignable;
    }

}

// ----------------------------------------------------------------------------

template <typename S, typename T>
struct nstd::type_traits::is_assignable
    : nstd::type_traits::integral_constant<bool, decltype(nstd::type_traits::detail::test<S, T>(0))::value>
{
};

// ----------------------------------------------------------------------------

#endif
