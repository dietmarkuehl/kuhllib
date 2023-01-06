// nstd/net/scope.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_SCOPE
#define INCLUDED_NSTD_NET_SCOPE

#include "nstd/execution/connect.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include <atomic>
#include <iostream>
#include <exception>

// ----------------------------------------------------------------------------

namespace nstd::net {
    class scope;
}

// ----------------------------------------------------------------------------

class nstd::net::scope {
    struct env {
        ::nstd::net::scope* d_scope;

        friend auto tag_invoke(::nstd::execution::get_scheduler_t, env const& self) noexcept
        {
            return self.d_scope->scheduler();
        }
        friend auto tag_invoke(::nstd::execution::get_stop_token_t, env const& self) noexcept
            -> ::nstd::stop_token_ns::in_place_stop_token
        {
            return self.d_scope->stop_token();
        }
    };
    struct job_base {
        virtual ~job_base() = default;
    };
    struct receiver {
        job_base*           d_job;
        ::nstd::net::scope* d_scope;

        friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) noexcept
            -> ::nstd::net::scope::env
        {
            return { self.d_scope };
        }

        friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, auto&&...) noexcept
            -> void
        {
            delete self.d_job;
        }
        friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, std::exception_ptr error) noexcept
            -> void
        {
            try { ::std::rethrow_exception(error); }
            catch (std::exception const& ex) {
                ::std::cout << "ERROR: " << ex.what() << "\n";
            }
            delete self.d_job;
        }
        friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, auto&& error) noexcept
            -> void
        {
            ::std::cout << "error: " << error << "\n";
            delete self.d_job;
        }
        friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self) noexcept
            -> void
        {
            delete self.d_job;
        }
    };
    template <typename Sender>
    struct job
        : ::nstd::net::scope::job_base
    {
        decltype(::nstd::execution::connect(::nstd::type_traits::declval<Sender>(),
                                            ::nstd::net::scope::receiver{nullptr, nullptr})) d_state;
        job(Sender&& sender, ::nstd::net::scope* scope)
            : d_state(::nstd::execution::connect(::nstd::utility::forward<Sender>(sender),
                                                 ::nstd::net::scope::receiver{this, scope}))
        {
            ::nstd::execution::start(this->d_state);
        }
    };

    ::nstd::net::io_context                     d_context;
    ::nstd::stop_token_ns::in_place_stop_source d_stop_source;
    ::std::atomic<bool>                         d_stopped{false};

public:
    template <::nstd::execution::sender Sender>
    auto start(Sender&&) -> void;
    auto stop() -> void;
    auto run() -> void;
    auto stopped() const -> bool { return this->d_stopped; }

    auto scheduler() -> typename ::nstd::net::io_context::scheduler_type { return this->d_context.scheduler(); }
    auto stop_token() -> typename ::nstd::stop_token_ns::in_place_stop_token { return this->d_stop_source.token(); }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender>
auto nstd::net::scope::start(Sender&& sender) -> void
{
    new ::nstd::net::scope::job<Sender>(::nstd::utility::forward<Sender>(sender), this);
}

// ----------------------------------------------------------------------------

#endif
