// nstd/executor/execution_context.t.cpp                              -*-C++-*-
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

#include "nstd/executor/execution_context.hpp"
#include "nstd/executor/fork_event.hpp"
#include "nstd/type_traits/declval.hpp"
#include <utility>
#include <vector>
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;
namespace test_declarations {}
namespace TD = ::test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    enum class op {
        dtor,
        fork,
        shutdown
    };
    template <int N>
    struct service
        : NET::execution_context::service
    {
        using key_type = service;
        using NET::execution_context::service::service;

        service(NET::execution_context& ctxt, ::std::vector<::std::pair<TD::op, int>>* vector = nullptr)
            : NET::execution_context::service(ctxt)
            , d_vector(vector)
        {
            
        }
        ~service() { this->record(TD::op::dtor); }
        auto notify_fork(::NET::fork_event) noexcept -> void override { this->record(TD::op::fork); }
        auto shutdown() noexcept -> void override { this->record(TD::op::shutdown); }

        void record(TD::op op){
            if (this->d_vector) {
                this->d_vector->emplace_back(op, N);
            }
        }
        ::std::vector<::std::pair<TD::op, int>>* d_vector = nullptr;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("execution_context members", []{
            struct context
                : NET::execution_context
            {
                    using NET::execution_context::shutdown;
                    using NET::execution_context::destroy;
            };
            NET::execution_context ctxt;
            ctxt.notify_fork(NET::fork_event::prepare);

            return KT::assert_type_exists<NET::execution_context::service>
                && KT::type<void> == KT::type<decltype(ctxt.notify_fork(NET::fork_event::prepare))>
                && KT::type<void> == KT::type<decltype(TT::declval<context&>().shutdown())>
                && KT::type<void> == KT::type<decltype(TT::declval<context&>().destroy())>
                ;
        }),
    KT::expect_success("execution_context::service members", []{
            struct service
                : NET::execution_context::service 
            {
                service(NET::execution_context& ctxt)
                    : NET::execution_context::service(ctxt) {

                }
                using NET::execution_context::service::context;
                auto shutdown() noexcept -> void override {}
                auto notify_fork(NET::fork_event) -> void override {}
            };
            NET::execution_context ctxt;
            service                svc(ctxt);
            return KT::type<NET::execution_context&> == KT::type<decltype(svc.context())>
                && &ctxt == &svc.context()
                ;
        }),
    KT::expect_success("service creation", []{
            NET::execution_context ctxt;
            return not NET::has_service<TD::service<1>>(ctxt)
                && not NET::has_service<TD::service<2>>(ctxt)
                && (NET::use_service<TD::service<1>>(ctxt), NET::has_service<TD::service<1>>(ctxt))
                && (NET::make_service<TD::service<2>>(ctxt), NET::has_service<TD::service<2>>(ctxt))
                ;
        }),
    KT::expect_success("operation order", []{
        ::std::vector<::std::pair<TD::op, int>> vector;
        {
            NET::execution_context ctxt;
            NET::make_service<TD::service<1>>(ctxt, &vector);
            NET::make_service<TD::service<2>>(ctxt, &vector);

            ctxt.notify_fork(NET::fork_event::prepare);
            ctxt.notify_fork(NET::fork_event::parent);
            ctxt.notify_fork(NET::fork_event::child);
        }
        ::std::vector<::std::pair<TD::op, int>> expect{
            { TD::op::fork, 1 },
            { TD::op::fork, 2 },
            { TD::op::fork, 2 },
            { TD::op::fork, 1 },
            { TD::op::fork, 2 },
            { TD::op::fork, 1 },
            { TD::op::shutdown, 2 },
            { TD::op::shutdown, 1 },
            { TD::op::dtor, 2 },
            { TD::op::dtor, 1 },
        };
        return vector == expect
            ;
    }),
    KT::expect_success("only one shutdown/destroy", []{
        ::std::vector<::std::pair<TD::op, int>> vector;
        {
            struct context
                : NET::execution_context {
                using NET::execution_context::shutdown;
                using NET::execution_context::destroy;
            } ctxt;
            NET::make_service<TD::service<1>>(ctxt, &vector);

            ctxt.shutdown();
            ctxt.shutdown();
            ctxt.destroy();
        }
        ::std::vector<::std::pair<TD::op, int>> expect{
            { TD::op::shutdown, 1 },
            { TD::op::dtor, 1 },
        };
        return vector == expect
            ;

    })
};

static KT::add_tests suite("execution_context", ::tests);
