// nstd/file/context.t.cpp                                            -*-C++-*-
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

#include "nstd/file/context.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NF = ::nstd::file;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        class context
            : public NF::context
        {
            auto do_run_one() -> NF::context::count_type override { return NF::context::count_type(); }
            auto do_cancel(io_base*, io_base*) -> void override {}
            auto do_nop(io_base*) -> void override {}
            auto do_timer(::nstd::file::context::time_spec*, io_base*) -> void override {}
            auto do_accept(native_handle_type, ::sockaddr*, ::socklen_t*, int, io_base*) -> void override {}
            auto do_connect(native_handle_type, ::sockaddr const*, ::socklen_t, io_base*) -> void override {}
            auto do_sendmsg(native_handle_type, ::msghdr const*, int, io_base*) -> void override {}
            auto do_recvmsg(native_handle_type, ::msghdr*, int, io_base*) -> void override {}
            auto do_read(int, ::iovec*, ::std::size_t, io_base*) -> void override {}
            auto do_open_at(int, char const*, int, io_base*) -> void override {}
            auto do_recvfrom(native_handle_type, void*, ::std::size_t, int, ::sockaddr*, ::socklen_t*, io_base*) -> void override {}
            auto do_sendto(native_handle_type, void const*, ::std::size_t, int, ::sockaddr*, ::socklen_t, io_base*) -> void override {}
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breathing", []{
            TD::context context;
            return true
                ;
        }),
};

static KT::add_tests suite("context", ::tests);
