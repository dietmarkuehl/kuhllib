// nstd/execution/receiver_of.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_RECEIVER_OF
#define INCLUDED_NSTD_EXECUTION_RECEIVER_OF

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/hidden_names/valid_completion_for.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    namespace hidden {
        template <typename Receiver>
        struct receiver_of_helper {
            template <::nstd::hidden_names::valid_completion_for<Receiver>... Signatures>
            void operator()(::nstd::execution::completion_signatures<Signatures...>*) {}
        };
    }
    template <typename Receiver, typename Completions>
    concept receiver_of
        =  ::nstd::execution::receiver<Receiver>
        && requires(Completions* completions, ::nstd::execution::hidden::receiver_of_helper<Receiver> helper) {
            helper(completions);
            //-dk:TODO remove []<::nstd::hidden_names::valid_completion_for<Receiver>... Signatures>(
            //    ::nstd::execution::completion_signatures<Signatures...>*)
            //{}(completions);
        }
        ;

    namespace hidden_names {
        template <typename> struct test_receiver_of_base;
        template <typename Tag, typename... Args> struct test_receiver_of_base<Tag(Args...)>;
        template <typename...> struct test_receiver_of;
    }
}

// ----------------------------------------------------------------------------

template <typename Tag, typename... Args>
struct nstd::execution::hidden_names::test_receiver_of_base<Tag(Args...)> {
    friend auto tag_invoke(Tag, test_receiver_of_base&, Args...) noexcept -> void {}
    friend auto tag_invoke(Tag, test_receiver_of_base &&, Args...) noexcept -> void {}
    friend auto tag_invoke(Tag, test_receiver_of_base const&, Args...) noexcept -> void {}
};

template <typename... Signatures>
struct nstd::execution::hidden_names::test_receiver_of
    : ::nstd::execution::hidden_names::test_receiver_of_base<Signatures>...
{
    test_receiver_of(test_receiver_of&&) = default;
    friend auto tag_invoke(::nstd::execution::get_env_t, test_receiver_of const&) -> int { return {}; }
};

static_assert(::nstd::execution::receiver_of<
    ::nstd::execution::hidden_names::test_receiver_of<::nstd::execution::set_value_t(int, bool),
                                        ::nstd::execution::set_error_t(void*),
                                        ::nstd::execution::set_stopped_t()
                                       >,
    ::nstd::execution::completion_signatures<::nstd::execution::set_value_t(int, bool),
                                             ::nstd::execution::set_error_t(void*),
                                             ::nstd::execution::set_stopped_t()
                                             >
    >);

// ----------------------------------------------------------------------------

#endif
