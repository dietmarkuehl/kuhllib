// nstd/net/timer.t.cpp                                               -*-C++-*-
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

#include "nstd/net/timer.hpp"
#include "nstd/execution/run.hpp"
#include "nstd/execution/then.hpp"
#include "kuhl/test.hpp"
#include <chrono>
#include <iostream>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace EX = ::nstd::execution;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("wait_traits", []{
            return KT::assert_template_exists<NN::wait_traits, ::std::chrono::system_clock>
                ;
        }),
    KT::expect_success("basic_waitable_timer", []{
            return KT::assert_template_exists<NN::basic_waitable_timer, ::std::chrono::system_clock>
                && KT::assert_template_exists<NN::basic_waitable_timer, ::std::chrono::system_clock, ::NN::wait_traits<::std::chrono::system_clock>>
                ;
        }),
    KT::expect_success("aliases", []{
            return KT::type<NN::basic_waitable_timer<::std::chrono::system_clock, ::NN::wait_traits<::std::chrono::system_clock>>>
                    == KT::type<NN::system_timer>
                && KT::type<NN::basic_waitable_timer<::std::chrono::steady_clock, ::NN::wait_traits<::std::chrono::steady_clock>>>
                    == KT::type<NN::steady_timer>
                && KT::type<NN::basic_waitable_timer<::std::chrono::high_resolution_clock, ::NN::wait_traits<::std::chrono::high_resolution_clock>>>
                    == KT::type<NN::high_resolution_timer>
                ;
        }),
    KT::expect_success("wait_traits::to_wait_duration(duration)", []{
            using clock = ::std::chrono::system_clock;
            clock::duration d(42);
            return KT::use(d)
                && NN::wait_traits<clock>::to_wait_duration(d) == d
                && NN::wait_traits<clock>::to_wait_duration(d) != clock::duration(43)
                && KT::type<decltype(NN::wait_traits<clock>::to_wait_duration(d))> == KT::type<clock::duration>
                ;
        }),
    KT::expect_success("wait_traits::to_wait_duration(time_point)", []{
            //-dkTODO use a custom clock to actually test....
            using clock = ::std::chrono::system_clock;
            clock::time_point t(clock::now() + clock::duration(100000));
            auto d = NN::wait_traits<clock>::to_wait_duration(t);

            return KT::use(d)
                && clock::duration(0) <= d && d <= clock::duration(100000)
                && KT::type<decltype(NN::wait_traits<clock>::to_wait_duration(t))> == KT::type<clock::duration>
                ;
        }),
    KT::expect_success("basic_waitable_timer typedef", []{
            using timer = NN::system_timer;
            return KT::type<timer::scheduler_type> == KT::type<NN::io_context::scheduler_type>
                && KT::type<timer::clock_type> == KT::type<::std::chrono::system_clock>
                && KT::type<timer::duration> == KT::type<::std::chrono::system_clock::duration>
                && KT::type<timer::time_point> == KT::type<::std::chrono::system_clock::time_point>
                && KT::type<timer::traits_type> == KT::type<NN::wait_traits<::std::chrono::system_clock>>
                ;
        }),
    KT::expect_success("basic_waitable_timer ctor from context only", []{
            try {
            using clock = ::std::chrono::system_clock;
            NN::io_context   context;
            NN::system_timer timer(context);
            return timer.get_scheduler() == context.scheduler()
                && timer.expiry() == clock::time_point()
                ;
            }
            catch (std::exception const& ex) {
                return false;
            }
        }),
    KT::expect_success("basic_waitable_timer ctor from time_point", []{
            try {
            using clock = ::std::chrono::system_clock;
            auto now = clock::now();
            NN::io_context   context;
            NN::system_timer timer(context, now);
            return timer.get_scheduler() == context.scheduler()
                && timer.expiry() == now
                ;
            }
            catch (std::exception const& ex) {
                return false;
            }
        }),
    KT::expect_success("basic_waitable_timer ctor from duration", []{
            try {
            using clock = ::std::chrono::system_clock;
            NN::io_context   context;
            clock::duration  d(300);
            auto before = clock::now();
            NN::system_timer timer(context, d);
            auto after = clock::now();
            return timer.get_scheduler() == context.scheduler()
                && (before + d <= timer.expiry()) && (timer.expiry() <= after + d)
                ;
            }
            catch (std::exception const& ex) {
                return false;
            }
        }),
    KT::expect_success("async_wait", []{
            try {
            using namespace std::chrono_literals;
            using clock = ::std::chrono::system_clock;
            NN::io_context   context;
            clock::duration  d(13ms);

            auto before = clock::now();
            auto count = 100000;
            NN::system_timer timer(context, d);
            EX::run(context, NN::async_wait(timer) | EX::then([before, &count](auto&&...){
                auto dur = clock::now() - before;
                count = ::std::chrono::duration_cast<::std::chrono::milliseconds>(dur).count();
                }));
                return 13 <= count;
            }
            catch (std::exception const& ex) {
                return false;
            }
        }),
            
};

static KT::add_tests suite("timer", ::tests);
