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
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

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
};

static KT::add_tests suite("execution_context", ::tests);
