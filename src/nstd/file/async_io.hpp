// nstd/file/async_io.hpp                                             -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_ASYNC_IO
#define INCLUDED_NSTD_FILE_ASYNC_IO

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
#include "nstd/execution/start.hpp"
#include "nstd/file/context.hpp"
#include "nstd/file/io_object.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/exchange.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

#include "nstd/execution/add_value.hpp"
#include "nstd/execution/just.hpp"
#include <atomic>
#include <functional>
#include <mutex>
#include <optional>
#include <system_error>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::async_io {
    template <::nstd::execution::receiver Receiver, ::nstd::file::io_object Stream, typename InnerState>
    struct state_base
        : ::nstd::file::io_base
    {
        using env_t = decltype(::nstd::execution::get_env(::nstd::type_traits::declval<Receiver>()));
        using stop_token_t = decltype(::nstd::execution::get_stop_token(::nstd::type_traits::declval<env_t>()));

        struct cancel
            : ::nstd::file::io_base {
            state_base* d_state;
            cancel(state_base* state)
                : ::nstd::file::io_base()
                , d_state(state) {
            }  
            cancel(cancel&& other)
                : ::nstd::file::io_base()
                , d_state(::nstd::utility::exchange(other.d_state, nullptr)) {
            }
            cancel(cancel const&) = delete;
            auto operator()() noexcept -> void {
                if (++this->d_state->d_outstanding == 2u) {
                    auto const& env{::nstd::execution::get_env(this->d_state->d_receiver)};
                    auto        scheduler{::nstd::execution::get_scheduler(env)};
                    scheduler.cancel(this->d_state, this);
                }
            }
            virtual auto do_result(::std::int32_t, ::std::uint32_t) -> void override {
                this->d_state->d_cancelled = true;
                if (--this->d_state->d_outstanding == 0u) {
                    if (this->d_state->d_result < 0) {
                        ::nstd::execution::set_stopped(::nstd::utility::move(this->d_state->d_receiver));
                    }
                    else {
                        this->d_state->complete(this->d_state->d_result, this->d_state->d_flags);
                    }
                }
            }
        };
        using callback_t = typename stop_token_t::template callback_type<cancel>;

        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        Stream&                                       d_stream;
        InnerState                                    d_inner_state{};
        ::std::optional<callback_t>                   d_callback;
        ::std::atomic<::std::size_t>                  d_outstanding{};
        ::std::atomic<bool>                           d_cancelled{false};
        ::std::int32_t                                d_result{};
        ::std::uint32_t                               d_flags{};

        template <::nstd::execution::receiver R>
        state_base(R&& receiver, Stream& stream) 
            : d_receiver(::nstd::utility::forward<R>(receiver))
            , d_stream(stream) {
        }

        auto do_result(::std::int32_t result, ::std::uint32_t flags) -> void override {
            this->d_result = result;
            this->d_flags  = flags;
            if (0 == --this->d_outstanding) {
                if (result < 0) {
                    if (this->d_cancelled) {
                        ::nstd::execution::set_stopped(::nstd::utility::move(this->d_receiver));
                    }
                    else {
                        ::nstd::execution::set_error(
                            ::nstd::utility::move(this->d_receiver),
                            ::std::error_code(result, ::std::system_category())
                            );
                    }
                }
                else {
                    this->complete(result, flags);
                }
            }
        }
        auto engage_callback() ->  void {
            auto env{::nstd::execution::get_env(this->d_receiver)};
            this->d_callback.emplace(::nstd::execution::get_stop_token(env), state_base::cancel(this));
        }
        auto complete(::std::int32_t result, ::std::uint32_t flags) -> void {
            ::std::visit(
                [&](auto& inner){ inner.d_state.complete(result, flags, this->d_receiver) ;},
                this->d_inner_state
            );
        }
    };

    template <typename Env, typename Operation>
    struct inner_state_t {
        template <typename... Args>
        struct inner_holder {
            using inner_type = decltype(Operation::connect(::nstd::type_traits::declval<Env>(), ::nstd::type_traits::declval<Args>()...));
            inner_type d_state;
            template <typename... A>
            inner_holder(Env const& env, A&&... args)
                : d_state(Operation::connect(env, ::nstd::utility::forward<A>(args)...)) {

            }
        };
    };
    struct empty_state {
        struct nested {
            auto complete(::std::int32_t, ::std::uint32_t, auto&) -> void {}
        } d_state;
    };
    template <typename... T>
    using variant = ::std::variant<empty_state, T...>;

    template <::nstd::execution::receiver Receiver, ::nstd::file::io_object Stream, typename Completions, typename Env, typename Operation>
    struct receiver {
        state_base<Receiver, Stream, Completions>* d_state;

        friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) noexcept {
            return ::nstd::execution::get_env(self.d_state->d_receiver);
        }
        template <typename... Args>
        friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, Args&&... args) noexcept {

            ++self.d_state->d_outstanding;
            self.d_state->engage_callback();

            using state_type = typename inner_state_t<Env, Operation>::template inner_holder<Args...>;
            self.d_state->d_inner_state.template emplace<state_type>(
                ::nstd::execution::get_env(self.d_state->d_receiver),
                ::nstd::utility::forward<Args>(args)...
                ).d_state.start(
                    self.d_state->d_stream,
                    ::nstd::execution::get_scheduler(::nstd::execution::get_env(self.d_state->d_receiver)),
                    self.d_state
                    );
        }
        template <typename Error>
        friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, Error&& error) noexcept {
            ::nstd::execution::set_error(
                ::nstd::utility::move(self.d_state->d_receiver),
                ::nstd::utility::forward<Error>(error)
            );
        }
        template <typename Error>
        friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self) noexcept {
            ::nstd::execution::set_stopped(::nstd::utility::move(self.d_state->d_receiver));
        }
    };

    template <template <typename, typename> class Operation,
              ::nstd::execution::sender   UpstreamSender,
              ::nstd::execution::receiver Receiver,
              ::nstd::file::io_object     Stream,
              typename Env = decltype(::nstd::execution::get_env(::nstd::type_traits::declval<Receiver>())),
              typename Completions =
                ::nstd::execution::value_types_of_t<
                    UpstreamSender,
                    Env,
                    ::nstd::hidden_names::async_io::inner_state_t<Env, Operation<Stream, Env>>::template inner_holder,
                    variant
                >
             >
    struct state
        : ::nstd::hidden_names::async_io::state_base<Receiver, Stream, Completions>
    {
        using receiver_t = ::nstd::hidden_names::async_io::receiver<Receiver, Stream, Completions, Env, Operation<Stream, Env>>;
        using upstream_state_t = decltype(::nstd::execution::connect(::nstd::type_traits::declval<UpstreamSender>(), ::nstd::type_traits::declval<receiver_t>()));

        upstream_state_t d_state;

        template <::nstd::execution::sender US, ::nstd::execution::receiver R>
        state(US&& upstream_sender, R&& receiver, Stream& stream)
            : ::nstd::hidden_names::async_io::state_base<Receiver, Stream, Completions>(::nstd::utility::forward<R>(receiver), stream)
            , d_state(::nstd::execution::connect(::nstd::utility::forward<US>(upstream_sender), receiver_t{this}))
        {
        }

        friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept -> void {
            ::nstd::execution::start(self.d_state);
        }
    };

    template <template <typename, typename> class Operation,
              ::nstd::execution::sender Sender,
              ::nstd::file::io_object Stream>
    struct sender
    {
        template <typename Env>
        friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, sender const&, Env const&) noexcept {
            return ::nstd::execution::completion_signatures<
                typename Operation<Stream, Env>::completion_signature,
                ::nstd::execution::set_error_t(::std::error_code), //-dk:TODO add upstream errors
                ::nstd::execution::set_stopped_t()
            >{};
        }

        ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
        Stream&                                     d_stream;

        friend auto tag_invoke(::nstd::execution::get_attrs_t, sender const& self) noexcept {
            return ::nstd::execution::get_attrs(self.d_sender);
        }
        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Receiver&& receiver) noexcept {
            return ::nstd::hidden_names::async_io::state<Operation, Sender, Receiver, Stream>{
                self.d_sender,
                ::nstd::utility::forward<Receiver>(receiver),
                self.d_stream
            };
        }
    };

    template <template <typename, typename> class Operation>
    struct cpo
        : ::nstd::execution::sender_adaptor_closure<cpo<Operation>>
    {
        template <::nstd::execution::sender Sender, ::nstd::file::io_object Stream>
        auto operator()(Sender&& sender, Stream& stream) const {
            return ::nstd::hidden_names::async_io::sender<Operation, Sender, Stream>{
                ::nstd::utility::forward<Sender>(sender),
                stream
            };
        } 
        template <::nstd::file::io_object Stream>
        auto operator()(Stream& stream) const {
            return ::nstd::execution::sender_adaptor_closure<cpo<Operation>>::operator()(::std::ref(stream));
        } 
    };
}

// ----------------------------------------------------------------------------

#endif
