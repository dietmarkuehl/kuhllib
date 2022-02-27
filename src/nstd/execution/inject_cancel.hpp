// nstd/execution/inject_cancel.hpp                                   -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_INJECT_CANCEL
#define INCLUDED_NSTD_EXECUTION_INJECT_CANCEL

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/hidden_names/add_signatures.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/stop_token/stoppable_token.hpp"
#include "nstd/stop_token/never_stop_token.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct inject_cancel_t
    {
        template <::nstd::stop_token::stoppable_token Token, ::nstd::execution::receiver Receiver>
        struct receiver
        {
            ::nstd::type_traits::remove_cvref_t<Token>    d_token;
            ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;

            friend auto tag_invoke(::nstd::execution::get_stop_token_t, receiver const& self) noexcept
                -> ::nstd::type_traits::remove_cvref_t<Token>
            {
                static_assert(::nstd::stop_token::stoppable_token<decltype(self.d_token)>);
                return self.d_token;
            }
            template <typename Tag, typename... Args>
            friend auto tag_invoke(Tag tag, receiver&& self, Args&&... args) noexcept
            {
                return nstd::tag_invoke(tag, ::nstd::utility::move(self.d_receiver), ::nstd::utility::forward<Args>(args)...);
            }
        };
        template <::nstd::execution::sender Sender, ::nstd::stop_token::stoppable_token Token>
        struct sender
        {
            using completion_signatures
                = ::nstd::hidden_names::add_signatures_t<typename Sender::completion_signatures, ::nstd::execution::set_stopped_t()>;
            template <template <typename...> class T, template <typename...> class V>
            using value_types = typename Sender::template value_types<T, V>;
            template < template <typename...> class V>
            using error_types = typename Sender::template error_types< V>;
            static constexpr bool sends_done{true};

            ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
            ::nstd::type_traits::remove_cvref_t<Token>  d_token;

            template <nstd::execution::receiver Receiver>
            friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& rceiver) noexcept
            {
                return ::nstd::execution::connect(::nstd::utility::move(self.d_sender),
                                                  receiver<Token, Receiver>{
                                                      ::nstd::utility::move(self.d_token),
                                                      ::nstd::utility::forward<Receiver>(rceiver)
                                                  });
            }
        };
        template <::nstd::execution::sender Sender, ::nstd::stop_token::stoppable_token Token>
        auto operator()(Sender&& sndr, Token token) const -> sender<Sender, Token>
        {
            return { ::nstd::utility::forward<Sender>(sndr), ::nstd::utility::forward<Token>(token) };
        }
        template <::nstd::stop_token::stoppable_token Token>
        auto operator()(Token token) const {
            return [this, token](::nstd::execution::sender auto&& sndr) mutable {
                return (*this)(::nstd::utility::forward<decltype(sndr)>(sndr), token);
            };
        }
    } inject_cancel;
}

static_assert(::nstd::execution::receiver<::nstd::execution::inject_cancel_t::receiver<::nstd::stop_token::never_stop_token, ::nstd::execution::test_receiver>>);
static_assert(::nstd::execution::receiver<::nstd::execution::inject_cancel_t::receiver<::nstd::stop_token::in_place_stop_token, ::nstd::execution::test_receiver>>);

// ----------------------------------------------------------------------------

#endif
