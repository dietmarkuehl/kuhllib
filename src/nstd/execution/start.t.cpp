// nstd/execution/start.t.cpp                                         -*-C++-*-
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

#include "nstd/execution/start.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    template <bool Noexcept>
    struct member_start {
        int* const ptr;
        void start() & noexcept(Noexcept) { *this->ptr = 42; }
    };
    template <bool Noexcept>
    void start(member_start<Noexcept>&) {}

    template <bool Noexcept>
    struct non_member_start {
        int* const ptr;
    };
    template <bool Noexcept>
    void start(non_member_start<Noexcept>& s) noexcept(Noexcept) { *s.ptr = 42; }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("member start() is called noexcept(true)", []{
            int                    value(17);
            TD::member_start<true> state{&value};
            EX::start(state);
            return value == 42
                && noexcept(EX::start(state))
                ;
        }),
    KT::expect_success("member start() is called noexcept(false)", []{
            int                     value(17);
            TD::member_start<false> state{&value};
            EX::start(state);
            return value == 42
                && !noexcept(EX::start(state))
                ;
        }),
    KT::expect_success("non-member start() is called noexcept(true)", []{
            int                        value(17);
            TD::non_member_start<true> state{&value};
            EX::start(state);
            return value == 42
                && noexcept(EX::start(state))
                ;
        }),
    KT::expect_success("non-member start() is called noexcept(false)", []{
            int                         value(17);
            TD::non_member_start<false> state{&value};
            EX::start(state);
            return value == 42
                && !noexcept(EX::start(state))
                ;
        }),
};

static KT::add_tests suite("start", ::tests);
