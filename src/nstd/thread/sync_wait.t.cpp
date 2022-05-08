// nstd/thread/sync_wait.t.cpp                                        -*-C++-*-
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

#include "nstd/thread/sync_wait.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"
#include <optional>
#include <variant>
#include <system_error>
#include <exception>
#include <stdexcept>

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace HN = ::nstd::hidden_names::sync_wait;
namespace TR = ::nstd::this_thread;
namespace TT = ::nstd::type_traits;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("sync_wait receiver", []{
        return EX::receiver<HN::receiver>
            //&& EX::scheduler<EX::get_scheduler(EX::get_env(TT::declval<HN::receiver const&>()))>
            ;
        }),
#if 0
    KT::expect_success("sync_wait usage", []{
            auto res = TT::sync_wait(EX::just(64));
            return KT::use(res)
                && KT::type<decltype(res)> == KT::type<::std::optional<::std::variant<int>>>
                && res
                && ::std::get<0>(*res) == 64
                ;
        }),
    KT::expect_success("sync_wait error_code", []{
        try {
            TT::sync_wait(EX::just_error(::std::error_code(0, ::std::generic_category())));
            return false;
        }
        catch (::std::error_code const&) {
            //-dk:TODO error_code should become a system_error
            return true;
        }
        catch (::std::system_error const&) {
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait exception object", []{
        try {
            TT::sync_wait(EX::just_error(::std::runtime_error("error")));
            return false;
        }
        catch (::std::runtime_error const&) {
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait exception_ptr", []{
        try {
            TT::sync_wait(EX::just_error(::std::make_exception_ptr(::std::logic_error("error"))));
            return false;
        }
        catch (::std::logic_error const&) {
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait cancel", []{
        try {
            TT::sync_wait(EX::just_done());
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait with void result", []{
        TT::sync_wait(EX::just());
        return true;
    }),
#endif
};

static KT::add_tests suite("[exec.sync_wait]", ::tests);
