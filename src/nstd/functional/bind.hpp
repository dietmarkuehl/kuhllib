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

#include "nstd/functional/invoke.hpp"
#include "nstd/functional/is_bind_expression.hpp"
#include "nstd/functional/placeholders.hpp"
#include "nstd/functional/reference_wrapper.hpp"
#include "nstd/functional/tuple_invoke.hpp"
#include "nstd/type_traits/decay.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/remove_reference.hpp"
#include "nstd/type_traits/result_of.hpp"
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
            template <int, typename T, typename Args>
            auto bind_get_arg(nstd::functional::reference_wrapper<T> bound, Args const&) 
                -> T& {
                return bound.get();
            }
            template <int, typename Bound, typename... Args>
            auto bind_get_arg(Bound& bound, std::tuple<Args...> const& args)
                -> typename nstd::type_traits::enable_if<nstd::functional::is_bind_expression<Bound>::value,
                                                         nstd::type_traits::result_of_t<Bound(Args...)> >::type {
                return nstd::functional::tuple_invoke(bound, args);
            }
            template <int Index, typename Bound, typename Args>
            auto bind_get_arg(Bound const&, Args const& args)
                -> typename nstd::type_traits::enable_if<0 != nstd::functional::is_placeholder<Bound>::value,
                                                         typename std::tuple_element<Bound::value - 1u, Args>::type>::type {
                return std::get<Bound::value - 1u>(args);
            }
            template <int Index, typename Bound, typename Args>
            auto bind_get_arg(Bound& bound, Args const& args)
                -> typename nstd::type_traits::enable_if<0 == nstd::functional::is_placeholder<Bound>::value,
                                                         Bound>::type {
                return bound;
            }

            template <typename Indices, typename Fun, typename... Bound>
            struct binder;
            template <std::size_t... Indices, typename Fun, typename... Bound>
            struct binder<nstd::utility::index_sequence<Indices...>, Fun, Bound...>;
        }

        template <typename Indices, typename Fun, typename... Bound>
        struct is_bind_expression<nstd::functional::detail::binder<Indices, Fun, Bound...> >
            : nstd::type_traits::true_type {
        };

        template <typename Fun, typename... Args>
        auto bind(Fun&&, Args&&...)
            -> nstd::functional::detail::binder<nstd::utility::index_sequence_for<Args...>, Fun, Args...>;
    }

}

// ----------------------------------------------------------------------------

template <std::size_t... Indices, typename Fun, typename... Bound>
struct nstd::functional::detail::binder<nstd::utility::index_sequence<Indices...>, Fun, Bound...>
{
private:
    nstd::type_traits::decay_t<Fun> fun;
    std::tuple<Bound...>            bound;

    template <typename Args>
    auto invoke(Args&& args)
        -> decltype(nstd::functional::invoke(this->fun,
                                             nstd::functional::detail::bind_get_arg<Indices>(std::get<Indices>(this->bound), args)...)) {
        return nstd::functional::invoke(this->fun,
                                        nstd::functional::detail::bind_get_arg<Indices>(std::get<Indices>(this->bound), args)...);
    }
public:
    template <typename F>
    binder(F&& fun, Bound&&...  bound)
        : fun(nstd::utility::forward<F>(fun))
        , bound(nstd::utility::forward<Bound>(bound)...) {
    }

    template <typename... Args>
    auto operator()(Args&&... args)
        -> decltype(this->invoke(std::tuple<Args...>(nstd::utility::forward<Args>(args)...))) {
        return this->invoke(std::tuple<Args...>(nstd::utility::forward<Args>(args)...));
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
