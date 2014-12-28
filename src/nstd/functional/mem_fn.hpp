// nstd/functional/mem_fn.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_MEM_FN
#define INCLUDED_NSTD_FUNCTIONAL_MEM_FN

#include "nstd/functional/invoke.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/is_function.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        namespace detail {
            template <typename>        struct mem_fn_types;
            template <typename R, typename T> struct mem_fn_types<R (T::*)()>;
            template <typename R, typename T> struct mem_fn_types<R (T::*)() const>;
            template <typename R, typename T> struct mem_fn_types<R (T::*)() volatile>;
            template <typename R, typename T> struct mem_fn_types<R (T::*)() const volatile>;
            template <typename R, typename T, typename A> struct mem_fn_types<R (T::*)(A)>;
            template <typename R, typename T, typename A> struct mem_fn_types<R (T::*)(A) const>;
            template <typename R, typename T, typename A> struct mem_fn_types<R (T::*)(A) volatile>;
            template <typename R, typename T, typename A> struct mem_fn_types<R (T::*)(A) const volatile>;
            template <typename R, typename T, typename... A> struct mem_fn_types<R (T::*)(A...)>;

            template <typename R, typename T>
            struct mem_fn;
        }
        template <typename R, typename T>
        auto mem_fn(R T::*mem_fun)
            -> nstd::functional::detail::mem_fn<R, T>;
    }

}

// ----------------------------------------------------------------------------

template <typename R>
struct nstd::functional::detail::mem_fn_types {
};
template <typename R, typename T>
struct nstd::functional::detail:: mem_fn_types<R (T::*)()> {
    using result_type = R;
    using argument_type = T*;
};
template <typename R, typename T>
struct nstd::functional::detail:: mem_fn_types<R (T::*)() const> {
    using result_type = R;
    using argument_type = T const*;
};
template <typename R, typename T>
struct nstd::functional::detail:: mem_fn_types<R (T::*)() volatile> {
    using result_type = R;
    using argument_type = T volatile*;
};
template <typename R, typename T>
struct nstd::functional::detail:: mem_fn_types<R (T::*)() const volatile> {
    using result_type = R;
    using argument_type = T const volatile*;
};
template <typename R, typename T, typename A>
struct nstd::functional::detail:: mem_fn_types<R (T::*)(A)> {
    using result_type          = R;
    using first_argument_type  = T*;
    using second_argument_type = A;
};
template <typename R, typename T, typename A>
struct nstd::functional::detail:: mem_fn_types<R (T::*)(A) const> {
    using result_type          = R;
    using first_argument_type  = T const*;
    using second_argument_type = A;
};
template <typename R, typename T, typename A>
struct nstd::functional::detail:: mem_fn_types<R (T::*)(A) volatile> {
    using result_type          = R;
    using first_argument_type  = T volatile*;
    using second_argument_type = A;
};
template <typename R, typename T, typename A>
struct nstd::functional::detail:: mem_fn_types<R (T::*)(A) const volatile> {
    using result_type          = R;
    using first_argument_type  = T const volatile*;
    using second_argument_type = A;
};
template <typename R, typename T, typename... A>
struct nstd::functional::detail:: mem_fn_types<R (T::*)(A...)> {
    using result_type = R;
};

// ----------------------------------------------------------------------------

template <typename R, typename T>
struct nstd::functional::detail::mem_fn
    : nstd::functional::detail::mem_fn_types<R T::*> {
private:
    R T::*mem_fun;
public:
    explicit mem_fn(R T::*mem_fun): mem_fun(mem_fun) {}
    template <typename... Args>
    auto operator()(Args&&... args) const
        -> decltype(nstd::functional::invoke(nstd::type_traits::declval<R T::*>(), nstd::utility::forward<Args>(args)...)) {
        return nstd::functional::invoke(this->mem_fun, nstd::utility::forward<Args>(args)...);
    }
    bool operator==(mem_fn const& other) const {
        return this->mem_fun == other.mem_fun;
    }
    bool operator!=(mem_fn const& other) const {
        return !(*this == other);
    }
};

// ----------------------------------------------------------------------------

template <typename R, typename T>
auto nstd::functional::mem_fn(R T::*mem_fun)
    -> nstd::functional::detail::mem_fn<R, T> {
    return nstd::functional::detail::mem_fn<R, T>(mem_fun);
}

// ----------------------------------------------------------------------------

#endif
