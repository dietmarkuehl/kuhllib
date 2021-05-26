// nstd/executor/system_executor.t.cpp                                -*-C++-*-
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

#include "nstd/executor/system_executor.hpp"
#include <memory>
#include <type_traits>
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace MEM = ::std;
namespace TT  = ::std;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("system_executor structors", []{
            return TT::is_default_constructible_v<NET::system_executor>
                ;
        }),
    KT::expect_success("system_executor operations", []{
            NET::system_executor       ex;
            NET::system_executor const cex;
            auto                       fun = []{};
            MEM::allocator<void> const allocator;       
            return KT::type<NET::system_context&> == KT::type<decltype(cex.context())>
                && noexcept(cex.context())
                && KT::type<void> == KT::type<decltype(cex.on_work_started())>
                && noexcept(cex.on_work_started())
                && KT::type<void> == KT::type<decltype(cex.on_work_started())>
                && noexcept(cex.on_work_finished())
                && KT::type<void> == KT::type<decltype(cex.dispatch(fun, allocator))>
                && KT::type<void> == KT::type<decltype(cex.post(fun, allocator))>
                && KT::type<void> == KT::type<decltype(cex.defer(fun, allocator))>
                && KT::type<bool> == KT::type<decltype(cex == cex)>
                && KT::type<bool> == KT::type<decltype(cex != cex)>
                ;
        }),
};

static KT::add_tests suite("system_executor", ::tests);
