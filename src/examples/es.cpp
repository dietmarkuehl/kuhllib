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

#include <iostream>

#include "nstd/concepts/same_as.hpp"
#include "nstd/execution.hpp"
#include "nstd/execution/upon_done.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/net/async_read_some.hpp"
#include "nstd/net/basic_socket_acceptor.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/ip/basic_endpoint.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/thread/sync_wait.hpp"
#include <deque>
#include <list>
#include <memory>
#include <mutex>
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

        template <typename Tag, typename... T>
        friend auto tag_invoke(Tag tag, receiver&& self, T&&... args) noexcept -> void {
            std::cout << self.message << ": " << debug_name<Tag>::value << "\n";
            tag(std::move(self.upstream), std::forward<T>(args)...);
        }
    };
    template <EX::sender Sender>
    struct sender {
        template <template <typename...> class T, template <typename...> class V>
        using value_types = typename Sender::template value_types<T, V>;
        template <template <typename...> class V>
        using error_types = typename Sender::template error_types<V>;
        static constexpr bool sends_done = Sender::sends_done;

        using completion_signatures = typename Sender::completion_signatures;

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
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<Type>>;
        template <template <typename...> class V>
        using error_types = V<>;
        static constexpr bool sends_done = true;

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
        return sender<Type, State>{this}
            |  EX::let_value(fun)
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
            return EX::schedule(context.scheduler())
                 | c.async_read_some(buffer)
                 | EX::then([&c](int n) {
                       if (n <= 0) {
                           c.stop();
                           return 0;
                       }
                       return n;
                   });
		})
    );
}

auto make_writer(NN::io_context& context, client& c)
{
    return EX::repeat_effect(
        c.consume([&](NN::const_buffer buffer){
            return EX::schedule(context.scheduler())
                 | c.async_write_some(buffer)
                 | EX::then([&c](int n) {
                       if (n <= 0) {
                           c.stop();
                           return 0;
                       }
                       return n;
                   });
		})
    );
}

void run_client(NN::io_context& context, stream_socket&& stream) {
    EX::start_detached(
        EX::just()
        | EX::let_value([&, c = client(std::move(stream))]() mutable {
	    return EX::when_all(
	            make_reader(context, c),
		        make_writer(context, c)
		    );
        })
    );
}

// ----------------------------------------------------------------------------

int main()
{
    ::std::cout << ::std::unitbuf;
    NN::io_context  c;
    socket_acceptor server(endpoint(NI::address_v4::any(), 12345));

    EX::run(c,
        EX::repeat_effect(
        EX::schedule(c.scheduler())
        | NN::async_accept(server)
        | EX::then([&c](std::error_code ec, stream_socket stream) {
             if (!ec) run_client(c, std::move(stream));
          })
        ));

}
