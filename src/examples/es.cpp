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

template <std::uint64_t Size>
struct ring_buffer {
    struct state_base {
        virtual void complete() = 0;
        virtual void stop() = 0;
    };

    static constexpr int producer{0};
    static constexpr int consumer{1};

    static constexpr std::uint64_t mask{Size - 1u};
    char buffer[Size];
    std::uint64_t   position[2]{ 0, 0 };
    state_base*     completion[2]{ nullptr, nullptr };

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
            self.complete();
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
    using producer_state = state<Receiver, NN::mutable_buffer, producer, decltype([](ring_buffer* ring){
                return Size - (ring->position[producer] - ring->position[consumer]);
            })>;
    template <typename Receiver>
    using consumer_state = state<Receiver, NN::const_buffer, consumer, decltype([](ring_buffer* ring){
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

public:
    template <typename Fun>
    auto produce(Fun fun) {
        return sender<NN::mutable_buffer, producer_state>{this}
            |  EX::let_value(fun)
            |  EX::then([self = this](int size){
                std::cout << "produced size=" << size << "\n";
                self->position[producer] += size;
                if (self->completion[consumer]) {
                    std::exchange(self->completion[consumer], nullptr)->complete();
                }
                return size;
            })
            ;
    }
    template <typename Fun>
    auto consume(Fun fun) {
        return sender<NN::const_buffer, consumer_state>{this}
            |  EX::let_value(fun)
            |  EX::then([self = this](int size){
                std::cout << "consumed size=" << size << "\n";
                self->position[consumer] += size;
                if (self->completion[producer]) {
                    std::exchange(self->completion[producer], nullptr)->complete();
                }
                return size;
            })
            ;
    }
    void stop() {
        for (auto&& completion: this->completion) {
            if (completion) {
                completion->stop();
            }
        }
    }
};

// ----------------------------------------------------------------------------

struct client {
    stream_socket   stream;
    ring_buffer<32> buffer;
    bool            done{false};

    client(stream_socket&& stream): stream(std::move(stream)) {}
    client(client&& other): stream(std::move(other.stream)) {}
    ~client() { std::cout << "destroyed\n"; }

    void stop() { this->done = true; this->buffer.stop(); }
    template <typename Fun>
    auto produce(Fun&& fun) { return this->buffer.produce(std::forward<Fun>(fun)); }
    template <typename Fun>
    auto consume(Fun&& fun) { return this->buffer.consume(std::forward<Fun>(fun)); }
};

// ----------------------------------------------------------------------------

auto make_reader(NN::io_context& context, client& c)
{
    return EX::repeat_effect_until(
        c.produce([&](NN::mutable_buffer buffer){
            return EX::schedule(context.scheduler())
                 | NN::async_read_some(c.stream, buffer)
                 | EX::then([&c](int n) {
                       if (n <= 0) {
                           c.stop();
                           return 0;
                       }
                       return n;
                   });
		}) | EX::then([](auto&&...){}),
        [&c]{ return c.done; }
        )
	    ;
}

auto make_writer(NN::io_context& context, client& c)
{
    return EX::repeat_effect_until(
        c.consume([&](NN::const_buffer buffer){
            return EX::schedule(context.scheduler())
                 | NN::async_write_some(c.stream, buffer)
                 | EX::then([&c](int n) {
                       if (n <= 0) {
                           c.stop();
                           return 0;
                       }
                       return n;
                   });
		}) | EX::then([](auto&&...){}),
        [&c]{ return c.done; }
        )
	    ;
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
