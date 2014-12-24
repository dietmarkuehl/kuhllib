// nstd/functional/bind.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_BIND
#define INCLUDED_NSTD_FUNCTIONAL_BIND

#include "nstd/functional/placeholders.hpp"
#include "nstd/functional/mem_fn.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/remove_reference.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/integer_sequence.hpp"
#include "nstd/utility/move.hpp"
#include <tuple>
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        namespace detail {
            template <typename Fun> struct binder_fun;
            template <typename RC, typename... Args> struct binder_fun<RC(Args...)>;
            template <typename R, typename T> struct binder_fun<R T::*>;
            template <typename Fun>
            using binder_fun_t = typename nstd::functional::detail::binder_fun<Fun>::type;

            template <int Index, typename Bound, typename Args>
            auto bind_get_arg(Bound& bound, Args const&) 
            -> typename nstd::type_traits::enable_if<0 == nstd::functional::is_placeholder<Bound>::value, Bound>::type {
                return bound;
            }
            template <int Index, typename Bound, typename Args>
            auto bind_get_arg(Bound const&, Args const& args)
            -> typename nstd::type_traits::enable_if<0 != nstd::functional::is_placeholder<Bound>::value,
                                                     typename std::tuple_element<Bound::value - 1u, Args>::type>::type {
                return std::get<Bound::value - 1u>(args);
            }

            template <typename Indices, typename Fun, typename... Bound>
            struct binder;
            template <std::size_t... Indices, typename Fun, typename... Bound>
            struct binder<nstd::utility::index_sequence<Indices...>, Fun, Bound...>;
        }

        template <typename Fun, typename... Args>
        auto bind(Fun&&, Args&&...)
            -> nstd::functional::detail::binder<nstd::utility::index_sequence_for<Args...>, Fun, Args...>;
    }

}

// ----------------------------------------------------------------------------

template <typename Fun>
struct nstd::functional::detail::binder_fun {
    using type = Fun;
};

template <typename RC, typename... Args>
struct nstd::functional::detail::binder_fun<RC(Args...)> {
    using type = RC(*)(Args...);
};
            
template <typename R, typename T>
struct nstd::functional::detail::binder_fun<R T::*> {
    using type = nstd::functional::detail::mem_fn<R, T>;
};

// ----------------------------------------------------------------------------

template <std::size_t... Indices, typename Fun, typename... Bound>
struct nstd::functional::detail::binder<nstd::utility::index_sequence<Indices...>, Fun, Bound...>
{
    nstd::functional::detail::binder_fun_t<Fun> fun;
    std::tuple<Bound...>                        bound;
    binder(Fun&& fun, Bound&&...  bound)
        : fun(nstd::utility::move(fun))
        , bound(std::forward<Bound>(bound)...) {
    }

    template <typename... Args>
    auto operator()(Args&&... args)
        -> decltype(nstd::type_traits::declval<nstd::functional::detail::binder_fun_t<Fun>&>()
                    (nstd::functional::detail::bind_get_arg<Indices>(std::get<Indices>(this->bound), std::make_tuple(nstd::utility::forward<Args>(args)...))...)) {
        namespace D = nstd::functional::detail;
        return (this->fun)(D::bind_get_arg<Indices>(std::get<Indices>(this->bound), std::make_tuple(nstd::utility::forward<Args>(args)...))...);
    }

    auto operator== (binder const& other) const -> bool {
        return this->fun == other.fun && this->bound == other.bound;
    }
    auto operator!= (binder const& other) const -> bool {
        return !(*this == other);
    }
};

// ----------------------------------------------------------------------------

template <typename Fun, typename... Args>
auto nstd::functional::bind(Fun&& fun, Args&&... args)
            -> nstd::functional::detail::binder<nstd::utility::index_sequence_for<Args...>, Fun, Args...>
{
    using type = nstd::functional::detail::binder<nstd::utility::index_sequence_for<Args...>, Fun, Args...>;
    return type(nstd::utility::forward<Fun>(fun), nstd::utility::forward<Args>(args)...);
}

// ----------------------------------------------------------------------------

#endif
