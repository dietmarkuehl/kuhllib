// nstd/net/io_context.t.cpp                                          -*-C++-*-
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

#include "nstd/net/io_context.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/start_detached.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/utility/move.hpp"
#include <chrono>
#include <type_traits>
#include <iostream>
#include "kuhl/test.hpp"

namespace NN = ::nstd::net;
namespace EX = ::nstd::execution;
namespace UT = ::nstd::utility;
namespace TT = ::std;
namespace TM = ::std::chrono;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    //-dk:TODO KT::expect_success("io_context derives from exection_context", []{
    //        return ::TT::is_base_of_v<NN::execution_context, NN::io_context>;
    //    }),
    KT::expect_success("io_context member types", []{
            return true // KT::assert_type_exists<NN::io_context::executor_type>
                && KT::assert_type_exists<NN::io_context::count_type>
                ;
        }),
    KT::expect_success("io_context structors", []{
            return TT::is_default_constructible_v<NN::io_context>
                && TT::is_constructible_v<NN::io_context, int>
                && !TT::is_copy_constructible_v<NN::io_context>
                && !TT::is_move_constructible_v<NN::io_context>
                && !TT::is_copy_assignable_v<NN::io_context>
                && !TT::is_move_assignable_v<NN::io_context>
                ;
        }),
    KT::expect_success("io_context operators", []{
            using namespace ::std::chrono_literals;
            NN::io_context* ctxt{};
            auto             now{TM::system_clock::now()};

            return true // KT::type<NN::io_context::executor_type>
                        // == KT::type<decltype(ctxt->get_executor())>
                && true // noexcept(ctxt->get_executor())
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->run())>
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->run_for(2s))>
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->run_until(now))>
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->run_one())>
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->run_one_for(2s))>
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->run_one_until(now))>
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->poll())>
                && KT::type<NN::io_context::count_type>
                    == KT::type<decltype(ctxt->poll_one())>
                && KT::type<void> == KT::type<decltype(ctxt->stop())>
                && KT::type<bool> == KT::type<decltype(ctxt->stopped())>
                && noexcept(ctxt->stopped())
                && KT::type<void> == KT::type<decltype(ctxt->restart())>
                ;
        }),
    KT::expect_success("io_context schedule (static)", []{
            NN::io_context ctxt{};
            return EX::scheduler<decltype(ctxt.scheduler())>
                && EX::sender<decltype(EX::schedule(ctxt.scheduler()))>
                ;
        }),
    KT::expect_success("io_context schedule", []{
            bool value = false;
            NN::io_context ctxt{};
            auto sender = EX::schedule(ctxt.scheduler())
                | EX::then([&value](auto...){ value = true; })
                ;
            EX::start_detached(UT::move(sender));
            auto count = ctxt.run_one();
            return KT::use(sender)
                && count == 1u
                && value
                ;

        }),
};

static KT::add_tests suite("io_context", ::tests);
