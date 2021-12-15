// nstd/execution/timeout.t.cpp                                       -*-C++-*-
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

#include "nstd/execution/timeout.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/stop_token_type.hpp"
#include "nstd/execution/get_stop_token.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <chrono>
#include <exception>
#include <optional>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct never
        {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<::std::exception_ptr>;
            static constexpr bool sends_done = true;

            template <EX::receiver Receiver>
            struct state {
                struct stopper {
                    state* d_state;
                    auto operator()() noexcept -> void {
                        state* s = this->d_state;
                        s->d_callback.reset(); 
                        EX::set_done(UT::move(s->d_receiver));
                    }
                };
                using stop_token = EX::stop_token_type_t<TT::remove_cvref_t<Receiver>>;
                using callback   = typename stop_token::template callback_type<stopper>;
                TT::remove_cvref_t<Receiver> d_receiver;
                ::std::optional<callback>    d_callback;
                friend auto tag_invoke(EX::start_t, state& s) noexcept -> void {
                    EX::get_stop_token(s.d_receiver);
                    s.d_callback.emplace(EX::get_stop_token(s.d_receiver), stopper{&s});
                }
            };

            template <EX::receiver Receiver>
            friend auto tag_invoke(EX::connect_t, never&&, Receiver&& receiver) noexcept
                -> state<Receiver> {
                    return { UT::forward<Receiver>(receiver), {} };
            }
        };
        static_assert(EX::operation_state<never::state<EX::test_receiver>>);
        static_assert(EX::sender<never>);
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            using namespace ::std::chrono_literals;
            NN::io_context context;
            auto sender = EX::timeout(TD::never(), context.scheduler(), 1ms); 
            EX::run(context, UT::move(sender));
            return KT::use(sender)
                ;
        }),
};

static KT::add_tests suite("timeout", ::tests);
