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

struct client {
    struct state_base {
        virtual void complete() = 0;
	virtual void stop() = 0;
    };
    stream_socket stream;
    static constexpr std::uint64_t mask = 0x3ff;
    char          buffer[mask + 1];
    std::uint64_t readpos = 0;
    std::uint64_t writepos = 0;;
    state_base*   completion = nullptr;

    bool          done{false};
    client(stream_socket&& stream): stream(std::move(stream)) {}
    client(client&& other): stream(std::move(other.stream)) {}
    ~client() { std::cout << "destroyed\n"; }

    template <EX::receiver Receiver>
    struct state: state_base {
        client*                      c;
        TT::remove_cvref_t<Receiver> receiver;
        template <EX::receiver R>
        state(client* c, R&& r): c(c), receiver(std::forward<R>(r)) {}
        friend void tag_invoke(EX::start_t, state& self) noexcept {
	    if (self.c->readpos != self.c->writepos) {
	        self.complete();
	    }
	    else {
	        self.c->completion = &self;
	    }
	}
	void complete() {
	    std::size_t begin = c->writepos & mask;
	    std::size_t size = std::min(c->readpos - c->writepos, sizeof(c->buffer) - begin);
	    c->writepos += size;
	    EX::set_value(std::move(receiver), NN::const_buffer(c->buffer + begin, size));
	}
	void stop() {
	    EX::set_stopped(std::move(receiver));
	}
    };
    struct sender {
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<NN::const_buffer>>;
        template <template <typename...> class V>
        using error_types = V<>;
        static constexpr bool sends_done = true;

        using completion_signatures = EX::completion_signatures<
            EX::set_value_t(NN::const_buffer),
            EX::set_stopped_t()
            >;

        client* c;

        template <EX::receiver Receiver>
        friend state<Receiver> tag_invoke(EX::connect_t, sender const& self, Receiver&& r) noexcept {
	    return state<Receiver>(self.c, std::forward<Receiver>(r));
	}
    };

    NN::mutable_buffer next_read_buffer() {
        std::size_t begin = this->readpos & mask;
        return NN::buffer(this->buffer + begin, sizeof(this->buffer) - begin);
    }
    void add_read(int n) {
        this->readpos += n;
	if (this->completion) {
	    std::exchange(this->completion, nullptr)->complete();
	}
    }
    sender next_write_buffer() { return sender{this}; }
    void stop() {
        if (this->completion) {
	    std::exchange(this->completion, nullptr)->stop();
	}
    }
};

auto make_reader(NN::io_context& context, client& c)
{
    return EX::repeat_effect_until(
          EX::just()
	  | EX::let_value([&]{
	      return 
                  EX::schedule(context.scheduler())
                | NN::async_read_some(c.stream, c.next_read_buffer())
                | EX::then([&c](int n) {
	              if (0 < n) {
	                  c.add_read(n);
	              }
	              else {
	                  c.done = true;
	              }
		})
		;
	      }),
          [&c]{ return c.done; }
          )
	  | EX::then([&c](auto&&...) {
	      c.stop();
	      std::cout << "reader done\n";
	      })
	  ;
}

auto make_writer(NN::io_context& context, client& c)
{
    return EX::repeat_effect_until(
            c.next_write_buffer()
          | EX::let_value([&](NN::const_buffer b) {
               return EX::schedule(context.scheduler())
                    | NN::async_write(c.stream, b)
                    ;
                }),
	    [&c]{ return c.done; }
          )
	  | EX::then([](auto&&...) { std::cout << "writer done\n"; })
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
