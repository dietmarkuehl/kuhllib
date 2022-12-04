// nstd/net/async_accept.t.cpp                                        -*-C++-*-
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

#include "nstd/net/async_accept.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/file/test_context.hpp"
#include "nstd/execution/on.hpp"
#include "nstd/execution/start_detached.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/inject_cancel.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/utility/move.hpp"
#include <tuple>
#include <cstring>
#include "kuhl/test.hpp"
#include "netinet/in.h"
#include "arpa/inet.h"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;
namespace NF = ::nstd::file;
namespace NI = ::nstd::net::ip;
namespace NN = ::nstd::net;
namespace ST = ::nstd::stop_token_ns;
namespace UT = ::nstd::utility;
namespace TT = ::nstd::this_thread;
namespace HN = ::nstd::hidden_names;
namespace TD = test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace {
        class stream
        {
        public:
            using protocol_type = int;
            using native_handle_type = int;

            int handle;

            stream() = default;
            stream(protocol_type, native_handle_type handle): handle(handle) {}
        };

        struct endpoint {
            sockaddr_in addr;
            auto set_address(::sockaddr_storage const* addr, ::socklen_t len) -> void {
                if (len == sizeof(sockaddr_in)) {
                    std::memcpy(&this->addr, addr, len);
                }
            }
        };

        class acceptor
        {
        public:
            using socket_type = TD::stream;
            using protocol_type = int;
            using native_handle_type = int;
            using endpoint_type = TD::endpoint;

            auto protocol() const -> socket_type::protocol_type { return {}; }
            auto native_handle() const -> native_handle_type { return {}; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("async_accept yields a sender", []{
        TD::acceptor acceptor;
        return true
            && EX::sender<decltype(NN::async_accept(acceptor))>
            ;
    }),
    KT::expect_success("breathing", []{
            NF::test_context test_context;
            NN::io_context   context(&test_context);
            test_context.on_accept = [&test_context](int, ::sockaddr* addr, ::socklen_t* len, int, NF::context::io_base* cont){
                if (addr && len && sizeof(sockaddr_in) <= *len) {
                    ::sockaddr_in* addr_in = reinterpret_cast<::sockaddr_in*>(addr);
                    addr_in->sin_family = AF_INET;
                    addr_in->sin_port   = htons(17);
                    addr_in->sin_addr.s_addr = htonl(0x01020304);
                    *len = sizeof(sockaddr_in);
                }
                test_context.make_ready(5, 0, cont);
            };

            bool         completion_called(false);
            int          handle{};
            TD::endpoint endpoint;
            TD::acceptor acceptor;
            auto accept
                = EX::on(context.scheduler(),
                      NN::async_accept(acceptor)
                    | EX::then([&](TD::stream stream, TD::endpoint ep){
                        handle   = stream.handle;
                        endpoint = ep;
                        completion_called = true;
                    })
                )
                ;

            EX::start_detached(accept);
            auto rc(context.run());
            return rc == 2
                && completion_called
                && handle == 5
                && endpoint.addr.sin_family == AF_INET
                && endpoint.addr.sin_port   == htons(17)
                && endpoint.addr.sin_addr.s_addr == htonl(0x01020304);
                ;
        }),
#if 0
    KT::expect_success("cancelation", []{
            NF::test_context test_context;
            NN::io_context   context(&test_context);
            test_context.on_accept = [](int, ::sockaddr*, ::socklen_t*, int, NF::context::io_base*){ };
            test_context.on_nop = [&test_context](NF::context::io_base* cont){
                test_context.make_ready(0, 0, cont);
            };
            test_context.on_cancel = [&test_context](NF::context::io_base* to_cancel, NF::context::io_base* cont){
                test_context.make_ready(0, 0, cont);
                test_context.make_ready(0, 0, to_cancel);
            };

            ST::in_place_stop_source source;
            bool completion_called{false};
            bool cancellation_called{false};
            TD::acceptor acceptor;
            auto accept
                = EX::schedule(context.scheduler())
                | EX::then([]{})
                | NN::async_accept(acceptor)
                | EX::inject_cancel(source.token())
                | EX::then([&](auto, TD::stream){ completion_called = true; })
                | EX::upon_stopped([&](auto&&...){ cancellation_called = true; })
                ;
            EX::start_detached(UT::move(accept));
            source.stop();
            auto rc(context.run());
            return true
                && KT::use(acceptor)
                && KT::use(accept)
                && rc == 3
                && not completion_called
                && cancellation_called
                ;
        }),
#endif
};

static KT::add_tests suite("async_accept", ::tests);
