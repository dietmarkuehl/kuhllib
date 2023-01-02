// examples/echo_server.cpp                                           -*-C++-*-
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

#include "nstd/execution.hpp"
#include "nstd/net.hpp"

#include <iostream>
#include <coroutine>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace EX = ::nstd::execution;
namespace NC = ::nstd::concepts;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;
namespace TT = ::nstd::type_traits;

using socket_acceptor = NN::basic_socket_acceptor<NI::tcp>;
using stream_socket = NN::basic_stream_socket<NI::tcp>;
using endpoint = NI::basic_endpoint<NI::tcp>;

// ----------------------------------------------------------------------------

template <typename> struct debug_name { static constexpr char value[] = "unknown-tag"; };
template <> struct debug_name<EX::set_value_t> { static constexpr char value[] = "set_value"; };
template <> struct debug_name<EX::set_error_t> { static constexpr char value[] = "set_error"; };
template <> struct debug_name<EX::set_stopped_t> { static constexpr char value[] = "set_stopped"; };

inline constexpr struct debug_t {

    template <EX::receiver Receiver>
    struct receiver {
        std::remove_cvref_t<Receiver> upstream;
        std::string                   message;

        friend auto tag_invoke(EX::get_env_t, receiver const& self) noexcept { return EX::get_env(self.upstream); }
        template <typename Tag, typename... T>
        friend auto tag_invoke(Tag tag, receiver&& self, T&&... args) noexcept -> void {
            std::cout << self.message << ": " << debug_name<Tag>::value << "\n";
            tag(std::move(self.upstream), std::forward<T>(args)...);
        }
    };
    template <EX::sender Sender>
    struct sender {
        using completion_signatures = typename std::remove_cvref_t<Sender>::completion_signatures;

        std::remove_cvref_t<Sender> upstream;
        std::string                 message;

        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, sender&& self, Receiver&& r) {
            return EX::connect(std::move(self.upstream),
                               receiver<Receiver>{std::forward<Receiver>(r), std::move(self.message)});
        }
        template <EX::receiver Receiver>
        friend auto tag_invoke(EX::connect_t, sender const& self, Receiver&& r) {
            return EX::connect(self.upstream,
                               receiver<Receiver>{std::forward<Receiver>(r), self.message});
        }
    };

    template <EX::sender Sender>
    auto operator()(Sender&& s, std::string const& message) const -> sender<Sender>  {
        return { std::forward<Sender>(s), message };
    }
    auto operator()(std::string const& message) const {
        return [this, message](EX::sender auto&& s){ return (*this)(std::forward<decltype(s)>(s), message); };
    }
} debug{};

// ----------------------------------------------------------------------------

template <std::uint64_t Size>
struct ring_buffer_base {
    std::uint64_t   position[2]{ 0, 0 };
};

template <std::uint64_t Size>
struct ring_buffer
    : ring_buffer_base<Size>
{
    struct state_base {
        virtual void complete() = 0;
        virtual void stop() = 0;
    };

    static constexpr int producer{0};
    static constexpr int consumer{1};

    static constexpr std::uint64_t mask{Size - 1u};
    bool        done{false};
    char        buffer[Size];
    state_base* completion[2]{ nullptr, nullptr };

    template <EX::receiver Receiver, typename Type, int Side, typename BufferSize>
    struct state
        : state_base
    {
        ring_buffer*                  ring;
        std::remove_cvref_t<Receiver> receiver;

        template <EX::receiver R>
        state(ring_buffer* ring, R&& receiver)
            : ring(ring)
            , receiver(std::forward<R>(receiver)) {
        }

        friend void tag_invoke(EX::start_t, state& self) noexcept {
            if (self.ring->done) {
                EX::set_stopped(std::move(self.receiver));
            }
            else {
                self.complete();
            }
        }
        void complete() override {
            std::uint64_t size(BufferSize()(this->ring));
            if(0 < size) {
                std::uint64_t begin = this->ring->position[Side] & mask;
                EX::set_value(std::move(this->receiver),
                                        Type(this->ring->buffer + begin,
                                             std::min(Size - begin, size)));
            }
            else {
                this->ring->completion[Side] = this;
            }
        }
        void stop() override {
            EX::set_stopped(std::move(this->receiver));
        }
    };
    template <typename Receiver>
    using producer_state = state<Receiver, NN::mutable_buffer, producer, decltype([](ring_buffer_base<Size>* ring){
                return Size - (ring->position[producer] - ring->position[consumer]);
            })>;
    template <typename Receiver>
    using consumer_state = state<Receiver, NN::const_buffer, consumer, decltype([](ring_buffer_base<Size>* ring){
                return ring->position[producer] - ring->position[consumer];
            })>;

    template <typename Type, template <typename> class State>
    struct sender {
        using completion_signatures = EX::completion_signatures<
            EX::set_value_t(Type),
            EX::set_stopped_t()
            >;

        ring_buffer* ring;

        template <EX::receiver Receiver>
        friend State<Receiver> tag_invoke(EX::connect_t, sender const& self, Receiver&& r) noexcept {
	        return State<Receiver>(self.ring, std::forward<Receiver>(r));
	    }
    };

    template <typename Type, template <typename> class State, int Side, typename Fun>
    auto make_sender(Fun fun) {
        return EX::let_value(sender<Type, State>{this}, fun)
            |  EX::then([self = this](int size){
                self->position[Side] += size;
                if (self->completion[Side == producer? consumer: producer]) {
                    std::exchange(self->completion[Side == producer? consumer: producer], nullptr)->complete();
                }
                return size;
            })
            ;
    }
public:
    template <typename Fun>
    auto produce(Fun fun) {
        return make_sender<NN::mutable_buffer, producer_state, producer>(fun);
    }
    template <typename Fun>
    auto consume(Fun fun) {
        return make_sender<NN::const_buffer, consumer_state, consumer>(fun);
    }
    void stop() {
        this->done = true;
        for (auto&& completion: this->completion) {
            if (completion) {
                completion->stop();
            }
        }
    }
};

// ----------------------------------------------------------------------------

class client {
    stream_socket   stream;
    ring_buffer< 2> buffer;

public:
    client(stream_socket&& stream): stream(std::move(stream)) {}
    client(client&& other): stream(std::move(other.stream)) {}

    void stop() { this->buffer.stop(); }
    template <typename Fun>
    auto produce(Fun&& fun) { return this->buffer.produce(std::forward<Fun>(fun)); }
    template <typename Fun>
    auto consume(Fun&& fun) { return this->buffer.consume(std::forward<Fun>(fun)); }

    auto async_read_some(NN::mutable_buffer buffer) {
        return NN::async_read_some(this->stream, buffer);
    }
    auto async_write_some(NN::const_buffer buffer) {
        return NN::async_write_some(this->stream, buffer);
    }
};

// ----------------------------------------------------------------------------

auto make_reader(NN::io_context& context, client& c)
{
    return EX::repeat_effect(
        c.produce([&](NN::mutable_buffer buffer){
            return EX::on(context.scheduler(),
                   c.async_read_some(buffer)
                 | EX::then([&c](int n) {
                       if (n <= 0) {
                           c.stop();
                           return 0;
                       }
                       return n;
                   })
            );
		})
    );
}

auto make_writer(NN::io_context& context, client& c)
{
    return EX::repeat_effect(
        c.consume([&](NN::const_buffer buffer){
            return EX::on(context.scheduler(),
                   c.async_write_some(buffer)
                 | EX::then([&c](int n) {
                       if (n <= 0) {
                           c.stop();
                           return 0;
                       }
                       return n;
                   })
            );
		})
    );
}

void run_client(NN::io_context& context, stream_socket&& stream) {
    EX::start_detached(
        EX::just()
        | EX::let_value([&, c = client(std::move(stream))]() mutable {
	    return EX::when_all(
	            make_reader(context, c),
		        make_writer(context, c),
                EX::just()
		    );
        })
    );
}

// ----------------------------------------------------------------------------

struct task {
    struct state_base {
        virtual void complete() = 0;
    };
    struct promise_type {
        state_base* completion{nullptr};

        auto initial_suspend() { return std::suspend_always(); }
        auto final_suspend() noexcept { return std::suspend_always(); }
        void return_void() {
            std::cout << "return_void\n";
            this->completion->complete();
        }
        auto get_return_object() {
            std::cout << "get_return_object()\n";
            return task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        void unhandled_exception() {
            std::cout << "unhandled_exception()\n";
            std::terminate();
        }

        template <EX::sender Sender>
        struct awaitable {
            //using result_type = typename Sender::template value_types<std::tuple, std::type_identity_t>;
            using result_type = EX::value_types_of_t<
                Sender,
                ::nstd::hidden_names::exec_envs::no_env,
                ::nstd::hidden_names::decayed_tuple,
                ::std::type_identity_t
                >;

            //awaitable(awaitable&&) = delete;
            //awaitable(awaitable const&) = delete;
            struct receiver {
                std::optional<result_type>*  result;
                std::coroutine_handle<void>* handle;

                friend auto tag_invoke(EX::get_env_t, receiver const&) noexcept { return 0; }
                template <typename... Args>
                friend void tag_invoke(EX::set_value_t, receiver&& self, Args&&... args) noexcept {
                    *self.result = std::make_tuple(std::forward<Args>(args)...);
                    self.handle->resume();
                }
                template <typename Error>
                friend void tag_invoke(EX::set_error_t, receiver&&, Error&& ) noexcept {
                    std::cout << "awaitable::receiver::set_error_t\n";
                }
                friend void tag_invoke(EX::set_stopped_t, receiver&&) noexcept {
                    std::cout << "awaitable::receiver::set_stopped_t\n";
                }
            };

            struct state {
                using op_state = decltype(EX::connect(std::declval<Sender>(), std::declval<receiver>()));

                std::coroutine_handle<void> handle;
                std::optional<result_type>  result;
                op_state                    d_op_state;
                state(std::coroutine_handle<void> handle,
                      Sender&&                    sender)
                    : handle(std::move(handle))
                    , d_op_state(EX::connect(std::move(sender), receiver{&this->result, &this->handle}))
                {
                    std::cout << "state::state()\n";
                }
                ~state() {
                    std::cout << "state::~state()\n";
                }
                state(state&&) = delete;
                state(state const&) = delete;
            };

            std::remove_cvref_t<Sender> d_sender;
            std::shared_ptr<state>      d_state;

            template <EX::sender S>
            awaitable(S&& s): d_sender(std::forward<S>(s)) {}

            bool await_ready() { std::cout << "awaitable::await_ready()\n"; return false; }
            void await_suspend(std::coroutine_handle<void> handle){
                std::cout << "awaitable::await_suspend\n";
                this->d_state = std::make_shared<state>(std::move(handle), std::move(this->d_sender));
                EX::start(this->d_state->d_op_state);
            }
            auto await_resume() {
                std::cout << "await_resume()\n";
                return *this->d_state->result;
            }
        };

        template <EX::sender Sender>
        auto await_transform(Sender&& s) {
            std::cout << "await_transform\n";
            return awaitable<Sender>(std::forward<Sender>(s));
        }
    };

    template <EX::receiver Receiver>
    struct state
        : state_base
    {
        std::coroutine_handle<promise_type> handle;
        std::remove_cvref_t<Receiver>       receiver;
        template <EX::receiver R>
        state(std::coroutine_handle<promise_type>&& handle, R&& r)
            : handle(std::move(handle))
            , receiver(std::forward<R>(r)) {
        }

        friend auto tag_invoke(EX::start_t, state& self) noexcept {
            std::cout << "task::start\n";
            self.handle.promise().completion = &self;
            self.handle.resume();
        }
        void complete() override {
            std::cout << "task::complete\n";
            EX::set_value(std::move(this->receiver));
        }
    };

    using completion_signatures = EX::completion_signatures<
    EX::set_value_t(),
    EX::set_error_t(int),
    EX::set_stopped_t()
    >;

    std::coroutine_handle<promise_type> handle;

    template <EX::receiver Receiver>
    friend auto tag_invoke(EX::connect_t, task&& self, Receiver&& receiver) {
        std::cout << "task::connect\n";
        return state<Receiver>(std::move(self.handle), std::forward<Receiver>(receiver));
    }
};

int main()
{
    ::std::cout << ::std::unitbuf;
    NN::io_context  c;
    socket_acceptor server(endpoint(NI::address_v4::any(), 12345));

    EX::run(c,
        EX::repeat_effect(
        EX::on(c.scheduler(),
              NN::async_accept(server)
            | EX::then([&c](stream_socket stream, auto ) {
                run_client(c, std::move(stream));
            })
        )
    ));
}
