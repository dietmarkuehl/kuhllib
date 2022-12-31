// nstd/net/async_io.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_IO
#define INCLUDED_NSTD_NET_ASYNC_IO

#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/file/context.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

#include <atomic>
#include <functional>
#include <mutex>
#include <optional>
#include <system_error>

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names {
    template <typename Operation, ::nstd::execution::receiver Receiver>
    struct async_io_state
        : ::nstd::file::context::io_base
    {
        using env_t = decltype(::nstd::execution::get_env(::nstd::type_traits::declval<::nstd::type_traits::remove_cvref_t<Receiver> const>()));
        using scheduler_t  = decltype(::nstd::execution::get_scheduler(::nstd::type_traits::declval<env_t>()));
        using stop_token_t = decltype(::nstd::execution::get_stop_token(::nstd::type_traits::declval<env_t>()));

        struct cancel
            : ::nstd::file::context::io_base {
            async_io_state* d_state;
            cancel(async_io_state* state)
                : ::nstd::file::context::io_base()
                , d_state(state) {
            }  
            auto operator()() noexcept -> void {
                if (++this->d_state->d_outstanding == 2u) {
                    ::nstd::execution::get_scheduler(::nstd::execution::get_env(this->d_state->d_receiver)).cancel(this->d_state, this);
                }
            }
            virtual auto do_result(::std::int32_t, ::std::uint32_t) -> void override {
                this->d_state->d_cancelled = true;
                if (--this->d_state->d_outstanding == 0u) {
                    this->d_state->d_operation.complete(0, 0u, true, this->d_state->d_state, this->d_state->d_receiver);
                }
            }
        };
        using callback_t = typename stop_token_t::template callback_type<cancel>;

        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        Operation                                     d_operation;
        typename Operation::state                     d_state;
        ::std::atomic<std::size_t>                    d_outstanding{0u};
        ::std::atomic<bool>                           d_cancelled{false};
        ::std::optional<callback_t>                   d_callback;

        template <typename O, ::nstd::execution::receiver R>
        async_io_state(O&& o, R&& r)
            : d_receiver(::nstd::utility::forward<R>(r))
            , d_operation(::nstd::utility::forward<O>(o))
            , d_state{}
            , d_callback{} {
        }
        friend void tag_invoke(::nstd::execution::start_t, async_io_state& self) noexcept {
            auto env = nstd::execution::get_env(self.d_receiver);
            auto scheduler = ::nstd::execution::get_scheduler(env);
            self.d_callback.emplace(::nstd::execution::get_stop_token(env), cancel(&self));
            ++self.d_outstanding;
            self.d_operation.start(scheduler, self.d_state, &self);
        }

        auto do_result(int32_t rc, uint32_t flags) -> void override {
            if (--this->d_outstanding == 0u) {
                this->d_callback.reset();
                this->d_operation.complete(rc, flags, this->d_cancelled, this->d_state, this->d_receiver);
            }
        }
    };

    template <typename Operation>
    struct async_io_sender
    {
        using completion_signatures
            = ::nstd::execution::completion_signatures<
                typename Operation::completion_signature,
                ::nstd::execution::set_error_t(::std::error_code), //-dk:TODO
                ::nstd::execution::set_stopped_t()
            >;

        Operation d_operation;
        template <typename... P>
        async_io_sender(P&&... p)
            : d_operation{::nstd::utility::forward<P>(p)...} {
        }

        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, async_io_sender const& self, Receiver&& receiver)
            -> ::nstd::net::hidden_names::async_io_state<Operation, Receiver>
        {
            return { self.d_operation, ::nstd::utility::forward<Receiver>(receiver) };
        }
    };
}

// ----------------------------------------------------------------------------

#endif
