// src/examples/chat_server.cpp                                       -*-C++-*-
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

#include "nstd/file/observer_context.hpp"
#include "nstd/file/ring_context.hpp"
#include "nstd/net.hpp"
#include "nstd/execution.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/remove_cvref.hpp"

#include <iostream>
#include <list>
#include <deque>
#include <vector>
#include <utility>

namespace EX = ::nstd::execution;
namespace NF = ::nstd::file;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;
namespace TT = ::nstd::type_traits;

using socket_acceptor = NN::basic_socket_acceptor<NI::tcp>;
using stream_socket = NN::basic_stream_socket<NI::tcp>;
using endpoint = NI::basic_endpoint<NI::tcp>;

// ----------------------------------------------------------------------------

namespace {
    constexpr inline struct when_each_t {
        template <typename Receiver>
        struct state_base {
            TT::remove_cvref_t<Receiver> d_receiver;
            ::std::size_t                d_count;
            ::std::function<void()>      d_result;
        };
        template <typename Receiver>
        struct receiver {
            state_base<Receiver>* d_state;
            friend auto tag_invoke(EX::get_env_t, receiver const& self) noexcept { return EX::get_env(self.d_state->d_receiver); }
            friend auto tag_invoke(EX::set_value_t, receiver&& self, auto&&...) noexcept {
                if (0u == --self.d_state->d_count) {
                    if (self.d_state->d_result) {
                        self.d_state->d_result();
                    }
                    else {
                        EX::set_value(::std::move(self.d_state->d_receiver));
                    }
                }
            }
            friend auto tag_invoke(EX::set_error_t, receiver&& self, auto&& error) noexcept {
                if (!self.d_state->d_result) {
                    self.d_state->d_result = [&receiver = self.d_state->d_receiver, error]() mutable { EX::set_error(::std::move(receiver), ::std::move(error)); };
                }
                if (0u == --self.d_state->d_count) {
                    self.d_state->d_result();
                }
            }
            friend auto tag_invoke(EX::set_stopped_t, receiver&& self) noexcept {
                if (!self.d_state->d_result) {
                    self.d_state->d_result = [&receiver = self.d_state->d_receiver]() mutable { EX::set_stopped(::std::move(receiver)); };
                }
                if (0u == --self.d_state->d_count) {
                    self.d_state->d_result();
                }
            }
        };

        template <typename Range, typename Fun>
        using inner_sender = decltype(TT::declval<Fun>()(::std::begin(TT::declval<Range>())));
        template <typename Range, typename Fun, EX::receiver Receiver>
        struct inner_state {
            decltype(EX::connect(TT::declval<inner_sender<Range, Fun>>(), TT::declval<receiver<Receiver>>())) d_state;
            template <typename S, typename R>
            inner_state(S&& s, R&& r)
                : d_state(EX::connect(::std::forward<S>(s), ::std::forward<R>(r)))
            {
            }
        };

        template <typename Range, typename Fun, typename Receiver>
        struct state
            : state_base<Receiver>
        {
            ::std::deque<inner_state<Range, Fun, Receiver>> d_states;
            template <typename R>
            state(auto&& range, auto&& fun, R&& r) 
                : state_base<Receiver>{::std::forward<R>(r), ::std::size(range), {}}
            {
                auto end(::std::end(range));
                for (auto it(::std::begin(range)); it != end; ++it) {
                    this->d_states.emplace_back(fun(it), receiver<Receiver>{this});
                }
            }
            friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                for (auto&& inner: self.d_states) {
                    EX::start(inner.d_state);
                }
            }
        };

        template <typename Range, typename Fun>
        struct sender {
            using completion_signatures = EX::completion_signatures<
                    EX::set_value_t()
                >;

            Range d_range;
            Fun   d_fun;

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender&& self, Receiver&& receiver) {
                return state<Range, Fun, Receiver>(self.d_range, self.d_fun, ::std::forward<Receiver>(receiver));
            }
            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender& self, Receiver&& receiver) {
                return state<Range, Fun, Receiver>(self.d_range, self.d_fun, ::std::forward<Receiver>(receiver));
            }
        };

        template <typename SizedRange, typename Fun>
        auto operator()(SizedRange&& range, Fun fun) const -> sender<SizedRange, Fun> {
            return { ::std::forward<SizedRange>(range), ::std::move(fun) };
        }
    } when_each;
}

// ----------------------------------------------------------------------------

namespace {
    class clients
    {
    private:
        struct client {
            client(stream_socket&& socket, bool done, ::std::size_t users)
                : first(::std::move(socket))
                , second(done)
                , users(users)
            {
            }
            stream_socket first;
            bool          second;
            ::std::size_t users;
        };
        using container = ::std::list<client>;
        container d_clients;

        auto remove_client(container::iterator it) -> void {
            if (0u == --it->users) {
                this->d_clients.erase(it);
            }
        }
        auto distribute(auto scheduler, ::std::vector<char> buffer) {
            (void)scheduler;
            (void)buffer;
            static_assert(EX::sender<decltype(EX::schedule(scheduler))>);
            EX::start_detached(
                EX::let_value(EX::schedule(scheduler), [this, scheduler, buffer]{
                    return when_each(this->d_clients, [this, scheduler, &buffer](auto it){
                        ++(it->users);
                        return EX::on(scheduler,
                                NN::async_write(it->first, NN::buffer(buffer))
                             |  EX::then([this, it](auto&&...){ this->remove_client(it); })
                        );
                    });
                })
            );
        }
    public:
        clients() = default;
        clients(clients&&) = delete;
        auto add_client(stream_socket cl, auto scheduler) -> void {
            this->d_clients.emplace_back(::std::move(cl), false, 1u);
            EX::start_detached(
                EX::repeat_effect_until(
                    EX::let_value(EX::just(),
                        [this,
                        &cl = this->d_clients.back().first,
                        &done = this->d_clients.back().second,
                        buffer = ::std::vector<char>(1024),
                        scheduler]() mutable {
                            return EX::on(scheduler,
                                  NN::async_read_some(cl, NN::buffer(buffer))
                                | EX::then([this, &buffer, &done, scheduler](::std::size_t n){
                                    if (n != 0u) {
                                        buffer.resize(n);
                                        this->distribute(scheduler, ::std::move(buffer));
                                    }
                                    else {
                                        done = true;
                                    }
                                  })
                            );
                        }
                    )
                    , [&done = this->d_clients.back().second]{ return done; }
                )
                | EX::then([this, it = --this->d_clients.end()]{
                    this->remove_client(it);
                })
            );
        }
    };
}

// ----------------------------------------------------------------------------

namespace {
    auto run_server(clients& cs, NN::io_context::scheduler_type scheduler, socket_acceptor& server) {
        return EX::repeat_effect(
                  EX::on(scheduler,
                    NN::async_accept(server)
                  | EX::then([&cs, scheduler](auto&& client, auto&& endpoint) {
                       ::std::cout << "accepted a new client: " << endpoint << "\n";
                        cs.add_client(::std::move(client), scheduler);
                    })
                  )
               );
    }
}

// ----------------------------------------------------------------------------

int main()
{
    ::std::cout << ::std::unitbuf;

    int const port(12345);
    socket_acceptor server(endpoint(NI::address_v4::any(), port));
    clients         cs;
    ::std::cout << "using port=" << port << "\n";

#ifndef __APPLE__
    NF::ring_context     ring(nstd::file::ring_context::queue_size{64});
    NN::io_context       context(::std::make_unique<NF::observer_context>(ring));
#else
    NN::io_context       context;
#endif
    EX::run(context, run_server(cs, context.scheduler(), server));

    ::std::cout << "done\n";
}
