// include/concepts.hpp                                               -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CONCEPTS
#define INCLUDED_CONCEPTS

namespace cxxrt::dummy{}
#  if !defined(__clang__)
#    include <concepts>
#  else

#include <functional>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace std
{
    template <typename F, typename T>
    concept convertible_to
        =  std::is_convertible_v<F, T>
        && requires(std::add_rvalue_reference_t<F>(&f)())
           {
               static_cast<T>(f());
           }
        ;

    template <typename D, typename B>
    concept derived_from
        =  std::is_base_of_v<B, D>
        && is_convertible_v<D const volatile*, B const volatile*>
        ;

    template <typename F, typename... A>
    concept invocable
        =  requires(F&& f, A&&... a)
           {
               std::invoke(std::forward<F>(f), std::forward<A>(a)...);
           }
        ;

    template <typename T>
    concept destructible
        =  std::is_nothrow_destructible_v<T>
        ;
    template <typename T, typename... A>
    concept constructible_from
        =  destructible<T>
        && std::is_constructible_v<T, A...>;
        ;

    template <typename T>
    concept move_constructible
        =  constructible_from<T, T>
        && convertible_to<T, T>
        ;

    template <typename T>
    concept copy_constructible
        =  move_constructible<T>
        && constructible_from<T, T&>
        && convertible_to<T&, T>
        && constructible_from<T, T const&>
        && convertible_to<T const&, T>
        && constructible_from<T, T const>
        && convertible_to<T const, T>
        ;

    template <typename T>
    concept boolean_testable_impl
        = convertible_to<T, bool>
        ;

    template <typename T>
    concept boolean_testable
        =  boolean_testable_impl<T>
        && requires(T&& t)
           {
               { !std::forward<T>(t) } -> boolean_testable_impl;
           }
        ;

    template <typename S, typename T>
    concept weakly_equality_comparable_with
        =  requires(std::remove_reference<S> const& s,
                    std::remove_reference<T> const& t)
           {
               { s == t } -> boolean_testable;
               { s != t } -> boolean_testable;
               { t == s } -> boolean_testable;
               { t != s } -> boolean_testable;
           }
        ;
    template <typename T>
    concept equality_comparable
        =  weakly_equality_comparable_with<T, T>
        ;
}

// ----------------------------------------------------------------------------

#  endif
#endif
