// nstd/net/async_send.t.cpp                                          -*-C++-*-
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

#include "nstd/net/async_send.hpp"
#include "nstd/net/socket_base.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "nstd/buffer/sequence.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/execution/on.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/file/operation.hpp"
#include "nstd/file/test_context.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <array>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace NF = ::nstd::file;
namespace NN = ::nstd::net;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct socket {
            using native_handle_type = int;
            using protocol_type      = int;
            socket() = default;
            socket(socket&&) = default;
            socket(socket const&) = delete;
            
            auto native_handle() const noexcept -> native_handle_type { return 17; }
            auto protocol() const -> protocol_type { return {}; }
            template <typename CBS>
            auto enqueue(NF::operation_send<CBS>& op) {
                op.submit();
            }
            template <typename CBS>
            auto complete(NF::operation_send<CBS>&) {
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("dummy", []{ return true; }),
#if 1
    KT::expect_success("one buffer, no flags", []{
            bool             sendmsg_called{false};
            bool             completion_called{false};
            NF::test_context test;
            NN::io_context   context(test);
            TD::socket       socket;
            char const       message[] = "hello, world";

            test.on_sendmsg = [&](int fd, ::msghdr const* msg, int flags, ::nstd::file::context::io_base* cont){
                sendmsg_called
                    =  fd == socket.native_handle()
                    && flags == int()
                    && msg
                    && msg->msg_name == nullptr
                    && msg->msg_namelen == 0
                    && msg->msg_iov != nullptr
                    && msg->msg_iovlen == 1
                    && msg->msg_iov[0].iov_base != nullptr
                    && msg->msg_iov[0].iov_len == sizeof(message)
                    && ::std::string_view(message)
                       == ::std::string_view(static_cast<char const*>(msg->msg_iov[0].iov_base), msg->msg_iov[0].iov_len - 1)
                    && msg->msg_control == nullptr
                    && msg->msg_controllen == 0
                    && msg->msg_flags == 0
                    ;
                test.make_ready(sizeof(message), 0, cont);
            };

            auto buffer = NN::buffer(message);
            auto sender = EX::on(context.scheduler(),
                  NN::async_send(socket, buffer) //NN::buffer(message))
                | EX::then([&](::std::int64_t){ completion_called = true; })
            );

            //-dk:TODO EX::run(context, UT::move(sender));
            (void)sender;

            return sendmsg_called
                && completion_called
                ;
        }),
#endif
#if 0
    KT::expect_success("multiple buffers, no flags", []{
            bool             sendmsg_called{false};
            bool             completion_called{false};
            NF::test_context test;
            NN::io_context   context(test);
            TD::socket       socket;
            char const       msg1[] = "hello, ";
            char const       msg2[] = "world";

            test.on_sendmsg = [&](int fd, ::msghdr const* msg, int flags, ::nstd::file::context::io_base* cont){
                sendmsg_called
                    =  fd == socket.native_handle()
                    && flags == int()
                    && msg
                    && msg->msg_name == nullptr
                    && msg->msg_namelen == 0
                    && msg->msg_iov != nullptr
                    && msg->msg_iovlen == 2
                    && msg->msg_iov[0].iov_base != nullptr
                    && msg->msg_iov[0].iov_len == sizeof(msg1)
                    && msg->msg_iov[1].iov_base != nullptr
                    && msg->msg_iov[1].iov_len == sizeof(msg2)
                    && ::std::string_view(msg1)
                       == ::std::string_view(static_cast<char const*>(msg->msg_iov[0].iov_base), msg->msg_iov[0].iov_len - 1)
                    && ::std::string_view(msg2)
                       == ::std::string_view(static_cast<char const*>(msg->msg_iov[1].iov_base), msg->msg_iov[1].iov_len - 1)
                    && msg->msg_control == nullptr
                    && msg->msg_controllen == 0
                    && msg->msg_flags == 0
                    ;
                test.make_ready(sizeof(msg1) + sizeof(msg2), 0, cont);
            };

            ::std::array<NN::const_buffer, 2> buffers{ NN::buffer(msg1), NN::buffer(msg2) };
            auto sender = EX::on(context.scheduler(),
                  NN::async_send(socket, buffers)
                | EX::then([&](::std::int64_t){ completion_called = true; })
            );

            EX::run(context, UT::move(sender));

            return sendmsg_called
                && completion_called
                ;
        }),
#endif
#if 0
    KT::expect_success("one buffer, flags", []{
            bool             sendmsg_called{false};
            bool             completion_called{false};
            NF::test_context test;
            NN::io_context   context(test);
            TD::socket       socket;
            char const       message[] = "hello, world";

            test.on_sendmsg = [&](int fd, ::msghdr const* msg, int flags, ::nstd::file::context::io_base* cont){
                sendmsg_called
                    =  fd == socket.native_handle()
                    && flags == int(NN::socket_base::message_peek)
                    && msg
                    && msg->msg_name == nullptr
                    && msg->msg_namelen == 0
                    && msg->msg_iov != nullptr
                    && msg->msg_iovlen == 1
                    && msg->msg_iov[0].iov_base != nullptr
                    && msg->msg_iov[0].iov_len == sizeof(message)
                    && ::std::string_view(message)
                       == ::std::string_view(static_cast<char const*>(msg->msg_iov[0].iov_base), msg->msg_iov[0].iov_len - 1)
                    && msg->msg_control == nullptr
                    && msg->msg_controllen == 0
                    && msg->msg_flags == 0
                    ;
                test.make_ready(sizeof(message), 0, cont);
            };

            auto sender = EX::on(context.scheduler(),
                  NN::async_send(socket, NN::buffer(message), NN::socket_base::message_peek)
                | EX::then([&](::std::int64_t){ completion_called = true; })
            );

            EX::run(context, UT::move(sender));

            return sendmsg_called
                && completion_called
                ;
        }),
#endif
};

static KT::add_tests suite("nstd::net:async_send", ::tests);
