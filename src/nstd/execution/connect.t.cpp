// nstd/execution/connect.t.cpp                                       -*-C++-*-
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

#include "nstd/execution/connect.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace EX = ::nstd::execution;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    template <bool Noexcept>
    struct member_sender
        : EX::sender_base
    {
        struct state { void start() & noexcept {}; };
        int* const ptr;
        template <typename Receiver>
        auto connect(Receiver&&) noexcept(Noexcept) -> state {
            *this->ptr = 42;
            return {};
        }
    };
    template <bool Noexcept, typename Receiver>
    auto connect(member_sender<Noexcept>&, Receiver&&) -> void = delete;
    template <bool Noexcept, typename Receiver>
    auto connect(member_sender<Noexcept> const&, Receiver&&) -> void = delete;
    template <bool Noexcept, typename Receiver>
    auto connect(member_sender<Noexcept>&&, Receiver&&) -> void = delete;

    template <bool Noexcept>
    struct non_member_sender
        : EX::sender_base
    {
        struct state { void start() & noexcept {}; };
        int* const ptr;
    };
    template <bool Noexcept, typename Receiver>
    auto connect(non_member_sender<Noexcept>& sender, Receiver&&)
        noexcept(Noexcept)
        -> typename non_member_sender<Noexcept>::state
    {
        *sender.ptr = 42;
        return {};
    }
    template <bool Noexcept, typename Receiver>
    auto connect(non_member_sender<Noexcept> const&, Receiver&&) -> void = delete;
    template <bool Noexcept, typename Receiver>
    auto connect(non_member_sender<Noexcept>&&, Receiver&&) -> void = delete;

    struct receiver {
        auto set_error(::std::exception_ptr) && noexcept -> void {}
        auto set_done() && noexcept -> void {}
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("member connect() noexcept(true)", []{
            int                     value(17);
            TD::member_sender<true> sender{{}, &value};

            EX::connect(sender, TD::receiver());
            return value == 42
                && noexcept(EX::connect(sender, TD::receiver()))
                ;
        }),
    KT::expect_success("member connect() noexcept(false)", []{
            int                      value(17);
            TD::member_sender<false> sender{{}, &value};

            EX::connect(sender, TD::receiver());
            return value == 42
                && !noexcept(EX::connect(sender, TD::receiver()))
                ;
        }),
    KT::expect_success("non_member connect() noexcept(true)", []{
            int                         value(17);
            TD::non_member_sender<true> sender{{}, &value};

            EX::connect(sender, TD::receiver());
            return value == 42
                && noexcept(EX::connect(sender, TD::receiver()))
                ;
        }),
    KT::expect_success("non_member connect() noexcept(false)", []{
            int                          value(17);
            TD::non_member_sender<false> sender{{}, &value};

            EX::connect(sender, TD::receiver());
            return value == 42
                && !noexcept(EX::connect(sender, TD::receiver()))
                ;
        }),
};

static KT::add_tests suite("connect", ::tests);
