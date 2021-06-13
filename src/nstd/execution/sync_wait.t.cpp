// nstd/execution/sync_wait.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/sync_wait.hpp"
#include "nstd/sender/just.hpp"
#include "nstd/sender/just_error.hpp"
#include "nstd/sender/just_done.hpp"
#include <exception>
#include <stdexcept>
#include <system_error>
#include "kuhl/test.hpp"

namespace EX  = ::nstd::execution;
namespace NET = ::nstd::net;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("sync_wait usage", []{
        try {
            EX::sync_wait(NET::just(17));
            return true;
        }
        catch (...) {
            return false;
        }
    }),
    KT::expect_success("sync_wait error_code", []{
        try {
            EX::sync_wait(NET::just_error(::std::error_code(0, ::std::generic_category())));
            return false;
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
            EX::sync_wait(NET::just_error(::std::runtime_error("error")));
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
            EX::sync_wait(NET::just_error(::std::make_exception_ptr(::std::logic_error("error"))));
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
            EX::sync_wait(NET::just_done());
            return true;
        }
        catch (...) {
            return false;
        }
    }),
};

static KT::add_tests suite("sync_wait", ::tests);
