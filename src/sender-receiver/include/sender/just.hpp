// include/sender/just.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_SENDER_JUST
#define INCLUDED_SENDER_JUST

#include <execution/set_value.hpp>
#include <execution/set_error.hpp>
#include <execution/sender_base.hpp>

#include <exception>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    template<typename> class just;
}

// ----------------------------------------------------------------------------

template<typename V>
class cxxrt::execution::just
{
private:
    std::remove_cvref_t<V> d_v;

public:
    template<template<typename...> class T, template<typename...> class Var>
    using value_types = Var<T<V>>;
    template<template<typename...> class Var>
    using error_types = Var<std::exception_ptr>;
    static constexpr bool sends_done = false;

    template<typename R>
    class state
    {
    private:
        V d_v;
        R d_r;

    public:
        template<typename T>
        state(T&& v, R&& r)
            : d_v(std::forward<T>(v))
            , d_r(std::forward<R>(r))
        {
        }
        void start() noexcept try
        {
            cxxrt::execution::set_value(std::move(this->d_r),
                                        std::move(this->d_v));
        }
        catch (...)
        {
            cxxrt::execution::set_error(std::move(this->d_r),
                                        std::current_exception());
        }
    };

    explicit just(V v): d_v(std::forward<V>(v)) {}

    template<typename R>
    state<R> connect(R&& r)
    {
        return state<R>{std::move(this->d_v), std::forward<R>(r)};
    }
};

// ----------------------------------------------------------------------------

#endif
