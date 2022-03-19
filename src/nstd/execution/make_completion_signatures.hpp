// nstd/execution/make_completion_signatures.hpp                      -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_EXECUTION_MAKE_COMPLETION_SIGNATURES
#define INCLUDED_NSTD_EXECUTION_MAKE_COMPLETION_SIGNATURES

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/dependent_completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sends_stopped.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/value_types_of_t.hpp"
#include "nstd/execution/error_types_of_t.hpp"
#include "nstd/type_traits/conditional.hpp"
#include <concepts>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    namespace hidden_names {
        template <typename... A>
	using default_set_value = ::nstd::execution::completion_signatures<::nstd::execution::set_value_t(A...)>;
        template <typename... A>
	using dk_default_set_value = ::nstd::execution::set_value_t(A...);

        template <typename E>
	using default_set_error = ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(E)>;
        template <typename E>
	using dk_default_set_error = ::nstd::execution::set_error_t(E);

        template <typename E, typename... L>
        struct combine_signatures_impl {
            using type = ::nstd::execution::dependent_completion_signatures<E>;
        };
        template <typename E, typename... T>
        struct combine_signatures_impl<E,
                                       ::nstd::execution::completion_signatures<T...>
                                       > {
            using type = ::nstd::execution::completion_signatures<T...>;
        };
        template <typename E, typename... T0, typename... T1, typename... L>
        struct combine_signatures_impl<E,
                                       ::nstd::execution::completion_signatures<T0...>,
                                       ::nstd::execution::completion_signatures<T1...>,
                                       L...
                                       > {
            using type = typename combine_signatures_impl<E,
                                                          ::nstd::execution::completion_signatures<T0..., T1...>,
                                                          L...>::type;
                ;
        };

        template <bool, typename, typename... T>
        struct unique_aux {
            using type = ::nstd::execution::completion_signatures<T...>;
        };
        template <typename S0, typename... T>
        struct unique_aux<true, S0, T...> {
            using type = ::nstd::execution::completion_signatures<T..., S0>;
        };

        template <typename, typename> struct unique;
        template <typename T>
        struct unique<T, ::nstd::execution::completion_signatures<>> {
            using type = T;
        };
        template <typename... T, typename S0, typename... S1>
        struct unique<::nstd::execution::completion_signatures<T...>,
                       ::nstd::execution::completion_signatures<S0, S1...>> {
            using type = typename unique<typename unique_aux<(true && ... && not ::std::same_as<S0, T>), S0, T...>::type,
                                         ::nstd::execution::completion_signatures<S1...>
                                         >::type; 
        };

        template <typename E, typename... L>
        using combine_signatures = typename unique<::nstd::execution::completion_signatures<>,
                                                    typename combine_signatures_impl<E, L...>::type>::type;
    }

    template <::nstd::execution::sender Sender,
               typename Env = ::nstd::hidden_names::exec_envs::no_env,
               ::nstd::hidden_names::valid_completion_signatures<Env> Add = ::nstd::execution::completion_signatures<>,
               template <typename...> class SetValue = ::nstd::execution::hidden_names::dk_default_set_value,
               template <typename...> class SetError = ::nstd::execution::hidden_names::default_set_error,
               ::nstd::hidden_names::valid_completion_signatures<Env> SetStopped = ::nstd::execution::completion_signatures<::nstd::execution::set_stopped_t()>
               >
            requires ::nstd::execution::sender<Sender, Env>
        using make_completion_signatures = ::nstd::execution::hidden_names::combine_signatures<
            Env,
            Add,
            ::nstd::execution::value_types_of_t<Sender, Env, SetValue, ::nstd::execution::completion_signatures>,
        ::nstd::execution::error_types_of_t<Sender, Env, SetError>,
            ::nstd::type_traits::conditional_t<::nstd::execution::sends_stopped<Sender, Env>,
                                               ::nstd::execution::completion_signatures<::nstd::execution::set_stopped_t()>,
                                               ::nstd::execution::completion_signatures<>
                                               >
            >;
}

// ----------------------------------------------------------------------------

#endif
