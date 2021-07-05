// nstd/execution/set_error.t.cpp                                     -*-C++-*-
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

#include "nstd/execution/set_error.hpp"
#include "nstd/utility/move.hpp"
#include <exception>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace UTIL = ::nstd::utility;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    struct error {};

    template <typename Error, bool Noexcept>
    struct member_set_error
    {
        bool* const ptr;
        friend auto tag_invoke(EX::set_error_t, member_set_error&& r, Error) noexcept(Noexcept) -> void { *r.ptr = true; }
    };

    template <typename Error, bool Noexcept>
    struct non_member_set_error
    {
        bool* const ptr;
    };
    template <typename Error, bool Noexcept>
    auto tag_invoke(EX::set_error_t, non_member_set_error<Error, Noexcept>&& r, Error) noexcept(Noexcept) -> void {
        *r.ptr = true;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("member set_error(error) noexcept(true)", []{
        bool                                  value(false);
        TD::error                             err;
        TD::member_set_error<TD::error, true> r{&value};
        EX::set_error(UTIL::move(r), TD::error());
        KT::use(err);
        return value
            && noexcept(EX::set_error(UTIL::move(r), err))
            ;
        }),
    KT::expect_success("member set_error(error) noexcept(false)", []{
        bool                                   value(false);
        TD::error                              err;
        TD::member_set_error<TD::error, false> r{&value};
        EX::set_error(UTIL::move(r), TD::error());
        return value
            && !noexcept(EX::set_error(UTIL::move(r), err))
            ;
        }),
    KT::expect_success("non-member set_error(error) noexcept(true)", []{
        bool                                      value(false);
        TD::error                                 err;
        TD::non_member_set_error<TD::error, true> r{&value};
        EX::set_error(UTIL::move(r), err);
        return value
            && noexcept(EX::set_error(UTIL::move(r), err))
            ;
        }),
    KT::expect_success("non-member set_error(error) noexcept(false)", []{
        bool                                       value(false);
        TD::error                                  err;
        TD::non_member_set_error<TD::error, false> r{&value};
        EX::set_error(UTIL::move(r), err);
        return value
            && !noexcept(EX::set_error(UTIL::move(r), err))
            ;
        }),
};

static KT::add_tests suite("set_error", ::tests);
