// nstd/execution/receiver_of.t.cpp                                   -*-C++-*-
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

#include "nstd/execution/receiver_of.hpp"
#include <exception>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    template <typename Error, bool SetErrorNoexcept, bool SetDoneNoexcept>
    struct receiver
    {
        receiver(receiver&);
        receiver(receiver&&);
        friend auto tag_invoke(EX::set_value_t, receiver&&, int, double) -> void {}
        friend auto tag_invoke(EX::set_error_t, receiver&&, Error) noexcept(SetErrorNoexcept) -> void {}
        friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept(SetDoneNoexcept) -> void {}
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("receiver_of", [](KT::context& )->bool{
            return EX::receiver_of<TD::receiver<::std::exception_ptr, true, true>, int, double>
                && !EX::receiver_of<TD::receiver<::std::exception_ptr, true, true>, char const*, double>
                && !EX::receiver_of<TD::receiver<::std::exception_ptr, false, true>, int, double>
                && !EX::receiver_of<TD::receiver<::std::exception_ptr, true, false>, int, double>
                ;
        }),
};

static KT::add_tests suite("receiver_of", ::tests);
