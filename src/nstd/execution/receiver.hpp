// nstd/execution/receiver.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_EXECUTION_RECEIVER
#define INCLUDED_NSTD_EXECUTION_RECEIVER

#include "nstd/concepts/move_constructible.hpp"
#include "nstd/concepts/constructible_from.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <concepts>
#include <exception>

// ----------------------------------------------------------------------------
// [exec.recv]

namespace nstd::execution {
    template <typename Receiver>
    concept receiver
        =  ::nstd::concepts::move_constructible<::nstd::type_traits::remove_cvref_t<Receiver>>
        && ::nstd::concepts::constructible_from<::nstd::type_traits::remove_cvref_t<Receiver>, Receiver>
        && requires(const ::nstd::type_traits::remove_cvref_t<Receiver>& rec) {
            ::nstd::execution::get_env(rec);
        }
        ;

    namespace hidden_names {
        struct test_env;
        struct test_receiver;
    }
}

// ----------------------------------------------------------------------------

struct nstd::execution::hidden_names::test_env {
};

struct nstd::execution::hidden_names::test_receiver {
    friend auto tag_invoke(::nstd::execution::get_env_t, test_receiver)
        -> ::nstd::execution::hidden_names::test_env {
        return {};
    }
    friend auto tag_invoke(::nstd::execution::set_value_t, test_receiver&&, auto&&...) noexcept
    -> void {
    }
    friend auto tag_invoke(::nstd::execution::set_error_t, test_receiver&&, auto&&...) noexcept
    -> void {
    }
    friend auto tag_invoke(::nstd::execution::set_stopped_t, test_receiver&&, auto&&...) noexcept
    -> void {
    }
};

static_assert(::nstd::execution::receiver<::nstd::execution::hidden_names::test_receiver>);

// ----------------------------------------------------------------------------

#endif
