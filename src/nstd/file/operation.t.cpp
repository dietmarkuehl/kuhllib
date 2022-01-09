// nstd/file/operation.t.cpp                                          -*-C++-*-
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

#include "nstd/file/operation.hpp"
#include "nstd/buffer/const_buffer.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace NF = ::nstd::file;
namespace NH = ::nstd::hidden_names;
namespace NN = ::nstd::net;
namespace TD = test_declarations;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    namespace {
        template <typename ConstantBufferSequence>
        struct operation_send
            : NF::operation_send<ConstantBufferSequence>
        {
            operation_send(ConstantBufferSequence const& cbs, NH::message_flags flags)
                : NF::operation_send<ConstantBufferSequence>(cbs, flags)
            {
            }
            auto do_result(::std::int32_t, ::std::uint32_t) -> void override {}
            auto do_submit() -> void override {}
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            char const array[] = "hello, world";
            TD::operation_send os(NN::buffer(array), NH::message_flags{});

            return KT::use(os)
                ;
        }),
};

static KT::add_tests suite("nstd::file::operation", ::tests);
