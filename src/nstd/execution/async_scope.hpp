// nstd/execution/async_scope.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_ASYNC_SCOPE
#define INCLUDED_NSTD_EXECUTION_ASYNC_SCOPE

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/container/intrusive_list.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_to.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <atomic>
#include <mutex>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    namespace async_scope {
        struct spawn_receiver;
        template <::nstd::execution::sender> struct nest_sender_cloak;
        template <::nstd::execution::sender> struct spawn_future_sender;
        struct upon_empty_sender;
    }
}

namespace nstd::execution {
    struct async_scope;
}

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender>
struct nstd::hidden_names::async_scope::nest_sender_cloak {
    struct nest_sender 
    {
        template <::nstd::execution::receiver Receiver>
        struct receiver_cloak
        {
            struct receiver {
                ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
                template <typename Tag, typename... A>
                friend auto tag_invoke(Tag&& tag, receiver&& r, A... a) noexcept(noexcept(tag(::nstd::utility::move(r.d_receiver), nstd::utility::forward<A>(a)...)))
                {
                    return tag(::nstd::utility::move(r.d_receiver), nstd::utility::forward<A>(a)...);
                }
            };
        };

        ::nstd::type_traits::remove_cvref_t<Sender> d_sender;

        template <::nstd::execution::receiver R>
        friend auto tag_invoke(::nstd::execution::connect_t, nest_sender&& self, R&& r)
        {
            return ::nstd::execution::connect(::nstd::utility::move(self.d_sender),
                                              receiver<R>(::nstd::utility::forward<R>(r))
                                              );
        }
    };
};

// ----------------------------------------------------------------------------

struct nstd::hidden_names::async_scope::upon_empty_sender
{
    using completion_signatures = ::nstd::execution::completion_signatures<
        ::nstd::execution::set_value_t()
        >;

    struct state_base {
        nstd::container::intrusive_list_node<state_base> link;
        ::nstd::execution::async_scope*                  scope;

        auto insert() -> void;
        auto complete() -> void { this->do_complete(); }
        virtual auto do_complete() -> void {}
    };
    template <::nstd::execution::receiver Receiver>
    struct state_cloak {
        struct state
            : state_base
        {
            ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
            friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept -> void {
                self.insert();
            }
            auto do_complete() -> void override {
                ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver));
            }
        };
    };

    ::nstd::execution::async_scope* scope;

    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, upon_empty_sender&& self, Receiver&& r)
        -> typename state_cloak<Receiver>::receiver {
            return { self.scope, ::nstd::utility::forward<Receiver>(r) };
        };
};

// ----------------------------------------------------------------------------

struct nstd::execution::async_scope {
private:
    using upon_empty_state_base = nstd::hidden_names::async_scope::upon_empty_sender::state_base;

    ::std::mutex                                             d_mutex;
    ::nstd::container::intrusive_list<upon_empty_state_base> d_on_empty;
    ::std::atomic<::std::size_t>                             d_outstanding{0u};

public:
    async_scope() = default;
    ~async_scope();
    async_scope(async_scope&&) = delete;

    template <::nstd::execution::sender_to<::nstd::hidden_names::async_scope::spawn_receiver> S>
    void spawn(S&&);

    template <nstd::execution::sender S>
    ::nstd::hidden_names::async_scope::spawn_future_sender<S> spawn_future(S&& snd);

    template <nstd::execution::sender Sender>
    auto nest(Sender&& sender)
        -> typename ::nstd::hidden_names::async_scope::nest_sender_cloak<Sender>::nest_sender;

    [[nodiscard]]
    ::nstd::hidden_names::async_scope::upon_empty_sender upon_empty() const noexcept { return {}; }
    
    ::nstd::stop_token_ns::in_place_stop_source& get_stop_source() noexcept;
    ::nstd::stop_token_ns::in_place_stop_token get_stop_token() const noexcept;
    void request_stop() noexcept;

    // should be private:
    auto insert(::nstd::hidden_names::async_scope::upon_empty_sender::state_base* s) -> bool;
    auto erase(::nstd::hidden_names::async_scope::upon_empty_sender::state_base* s) -> void;
private:
    auto complete() -> void;
};

// ----------------------------------------------------------------------------

template <nstd::execution::sender Sender>
auto nstd::execution::async_scope::nest(Sender&& s)
    -> typename ::nstd::hidden_names::async_scope::nest_sender_cloak<Sender>::nest_sender
{
    ++this->d_outstanding;
    return { ::nstd::utility::forward<Sender>(s) };
}

// ----------------------------------------------------------------------------

#endif
