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
#include "nstd/hidden_names/merge_completion_signatures.hpp"
#include "nstd/hidden_names/unique_completion_signatures.hpp"
#include "nstd/type_traits/conditional.hpp"
#include <concepts>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    namespace hidden_names {
        template <typename... A>
        using default_set_value = ::nstd::execution::completion_signatures<::nstd::execution::set_value_t(A...)>;
        template <typename E>
        using default_set_error = ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(E)>;

        template <typename E>
        struct bound_merge_completion_signatures {
            template <typename... T>
            using alias = ::nstd::hidden_names::merge_completion_signatures_t<E, T...>;
        };

        template <typename Env, template <typename> class SetError>
        struct bound_transform_error_signatures {
            template <typename... E>
            using alias = ::nstd::hidden_names::merge_completion_signatures_t<Env, SetError<E>...>;
        };
    }

    template <::nstd::execution::sender Sender,
               typename Env = ::nstd::hidden_names::exec_envs::no_env,
               ::nstd::hidden_names::valid_completion_signatures<Env> Add = ::nstd::execution::completion_signatures<>,
               template <typename...> class SetValue = ::nstd::execution::hidden_names::default_set_value,
               template <typename> class SetError = ::nstd::execution::hidden_names::default_set_error,
               ::nstd::hidden_names::valid_completion_signatures<Env> SetStopped = ::nstd::execution::completion_signatures<::nstd::execution::set_stopped_t()>
               >
            requires ::nstd::execution::sender<Sender, Env>
        using make_completion_signatures = ::nstd::hidden_names::merge_completion_signatures_t<
            Env,
            Add,
            ::nstd::execution::value_types_of_t<Sender, Env, SetValue, ::nstd::execution::hidden_names::bound_merge_completion_signatures<Env>::template alias>,
            ::nstd::execution::error_types_of_t<Sender, Env, ::nstd::execution::hidden_names::bound_transform_error_signatures<Env, SetError>::template alias>,
            ::nstd::type_traits::conditional_t<::nstd::execution::sends_stopped<Sender, Env>,
                                               ::nstd::execution::completion_signatures<::nstd::execution::set_stopped_t()>,
                                               ::nstd::execution::completion_signatures<>
                                               >
            >;
}

// ----------------------------------------------------------------------------

#endif
