// nstd/execution/run.t.cpp                                           -*-C++-*-
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

#include "nstd/execution/run.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <deque>
#include <functional>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::type_traits;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        class context
        {
        private:
            ::std::deque<::std::function<auto()->void>> d_jobs;
            ::std::size_t                               d_run{};
        
        public:
            auto run_one() -> ::std::size_t {
                if (this->d_jobs.empty()) {
                    return 0u;
                }
                this->d_jobs.front()();
                this->d_jobs.pop_front();
                ++this->d_run;
                return 1u;
            }
            auto count() const -> ::std::size_t { return this->d_run; }
            auto submit(::std::function<auto()->void> job) {
                this->d_jobs.emplace_back(::nstd::utility::move(job));
            }
        };

        class sender
        {
        public:
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<>;
            static constexpr bool sends_done = false;
        
            TD::context* d_context;

            template <typename Receiver>
            struct state {
                TT::remove_cvref_t<Receiver> d_receiver;
                TD::context*                 d_context;
                friend auto tag_invoke(EX::start_t, state& s)
                    noexcept -> void
                {
                    s.d_context->submit([&receiver = s.d_receiver]{
                        EX::set_value(UT::move(receiver));
                    });
                }
            };
            template <typename Receiver>
            friend auto tag_invoke(EX::connect_t, sender s, Receiver&& r) -> state<Receiver>
            {
                return state<Receiver>{ UT::forward<Receiver>(r), s.d_context };
            }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("sender already complete when run", []{
            TD::context context;
            EX::run(context, EX::just());
            return 0u == context.count();
                ;
        }),
    KT::expect_success("sender completes upon context run", []{
            TD::context context;
            EX::run(context, TD::sender{&context});
            return 1u == context.count();
                ;
        }),
};

static KT::add_tests suite("run", ::tests);
