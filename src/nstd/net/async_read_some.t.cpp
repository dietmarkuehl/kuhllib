// nstd/net/async_read_some.t.cpp                                     -*-C++-*-
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

#include "nstd/net/async_read_some.hpp"
#include "nstd/net/basic_stream_socket.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/net/ip/tcp.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/on.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/let.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "nstd/hidden_names/print_completion_signatures.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names;
namespace NN = ::nstd::net;
namespace TT = ::nstd::type_traits;
namespace TH = ::nstd::this_thread;
namespace UT = ::nstd::utility;
namespace KT = ::kuhl::test;
namespace TD = ::test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct sender;
        struct scheduler {
            auto operator== (scheduler const&) const -> bool = default;
            friend auto tag_invoke(EX::schedule_t, scheduler const&) -> TD::sender;

            auto recvmsg(auto&&...) -> void {}
        };
        template <EX::receiver Receiver>
        struct state {
            TT::remove_cvref_t<Receiver> d_receiver;
            friend auto tag_invoke(EX::start_t, state& self) noexcept -> void {
                EX::set_value(UT::move(self.d_receiver));
            }
        };
        struct sender {
            using completion_signatures = EX::completion_signatures<EX::set_value_t()>;
            friend auto tag_invoke(EX::get_completion_scheduler_t<EX::set_value_t>, sender const&) noexcept -> TD::scheduler { return {}; }
            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, sender const&, Receiver&& receiver) -> state<Receiver> {
                return { UT::forward<Receiver>(receiver) };
            }
        };
        auto tag_invoke(EX::schedule_t, scheduler const&) -> TD::sender { return {}; }

        struct env {
            friend auto tag_invoke(EX::get_scheduler_t, env const&) noexcept -> TD::scheduler { return {}; }
        };
        struct receiver {
            friend auto tag_invoke(EX::get_env_t, receiver const&) noexcept -> TD::env { return {}; }

#if 0
            friend auto tag_invoke(EX::set_value_t, receiver&&, auto&& x) noexcept -> void {
                (void)x;
                static_assert(::std::same_as<int, decltype(x)>);
            }
#endif
            friend auto tag_invoke(EX::set_value_t, receiver&&, ::std::size_t) noexcept -> void {}
            friend auto tag_invoke(EX::set_error_t, receiver&&, auto&&) noexcept -> void {}
            friend auto tag_invoke(EX::set_error_t, receiver&&, ::std::error_code) noexcept -> void {}
            friend auto tag_invoke(EX::set_error_t, receiver&&, ::std::exception_ptr) noexcept -> void {}
            friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept -> void {}
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            NN::basic_stream_socket<NN::ip::tcp> socket(NN::ip::tcp::v4());
            char buffer[10];
            auto sender = NN::async_read_some(socket, NN::buffer(buffer));
            //HN::print_completion_signatures(sender);
            //std::cout << "is completion signatures: " << std::boolalpha << HN::is_completion_signatures<TT::remove_cvref_t<decltype(sender)>::completion_signatures> << "\n";
            return KT::use(sender)
                && HN::is_completion_signatures<decltype(EX::get_completion_signatures(sender))>
                ;
        }),
    KT::expect_success("using async_read with let", []{
            NN::io_context context;
            NN::basic_stream_socket<NN::ip::tcp> socket(NN::ip::tcp::v4());
            char buffer[10];
            auto sender
                = EX::let_value(
                    EX::on(context.scheduler(), NN::async_read_some(socket, NN::buffer(buffer))),
                    // [](::std::size_t){ return EX::just(); }
                    [](auto&&...){ return EX::just(); }
                );
            using completion_signatures = decltype(EX::get_completion_signatures(sender));
            //TH::sync_wait(UT::move(sender));
            return KT::use(sender)
                && HN::is_completion_signatures<completion_signatures>
                ;
        }),
    KT::expect_success("using async_read with let and pipe", []{
            NN::io_context context;
            NN::basic_stream_socket<NN::ip::tcp> socket(NN::ip::tcp::v4());
            char buffer[10];
            auto sender
                = EX::on(context.scheduler(), NN::async_read_some(socket, NN::buffer(buffer)))
                | EX::let_value([](::std::size_t){ return EX::just(); })
                ;
            using completion_signatures = decltype(EX::get_completion_signatures(sender));
            //TH::sync_wait(UT::move(sender));
            return KT::use(sender)
                && HN::is_completion_signatures<completion_signatures>
                ;
        }),
    KT::expect_success("test", []{
            NN::io_context context;
            NN::basic_stream_socket<NN::ip::tcp> socket(NN::ip::tcp::v4());
            char buffer[10];
            // auto sender = EX::on(context.scheduler(), NN::async_read_some(socket, NN::buffer(buffer))) ;
            // auto sender = EX::on(context.scheduler(), NN::async_read_some_adapter(EX::just(NN::buffer(buffer)), socket));
            EX::get_scheduler(TD::env{});
#if 1
            auto sender = NN::async_read_some_adapter(EX::just(NN::buffer(buffer)), socket);
#elif 1
            auto sender = EX::on(TD::scheduler{}, NN::async_read_some_adapter(EX::just(NN::buffer(buffer)), socket));
#else
            auto sender = EX::on(context.scheduler(), NN::async_read_some_adapter(EX::just(NN::buffer(buffer)), socket));
#endif

            ::std::cout << "just1:";
            HN::print_completion_signatures(EX::just());
            ::std::cout << "just:";
            HN::print_completion_signatures(EX::just(NN::buffer(buffer)));
            ::std::cout << "just -- end\n";
            ::std::cout << "async_read_some\n";
            HN::print_completion_signatures(sender);
            ::std::cout << "async_read_some -- end\n";

#if 1
            auto state = EX::connect(sender, TD::receiver{});
            EX::start(state);
            KT::use(state);
#else
            TH::sync_wait(UT::move(sender));
#endif

            return KT::use(sender)
                && KT::use(buffer)
                //&& false
                ;
        }),
};

static KT::add_tests suite("async_read_some", ::tests);
