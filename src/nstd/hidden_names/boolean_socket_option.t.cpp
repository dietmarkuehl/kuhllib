// nstd/hidden_names/boolean_socket_option.t.cpp                      -*-C++-*-
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

#include "nstd/hidden_names/boolean_socket_option.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace HN = ::nstd::hidden_names;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct option {
            option() noexcept = default;
            option(bool) noexcept {}

            auto value() const noexcept -> bool { return {}; }
            explicit operator bool() const noexcept { return {}; }
            auto operator not() const noexcept -> bool { return {}; }
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("boolean_socket_option", []{
            return not HN::boolean_socket_option<int>
                && HN::boolean_socket_option<TD::option>
                ;
        }),
    KT::expect_success("default ctor", []{
            HN::boolean_socket_option_t<SO_REUSEADDR> const option;
            return not option.value()
                && not static_cast<bool>(option)
                && not option
                && noexcept(HN::boolean_socket_option_t<SO_REUSEADDR>())
                ;
        }),
    KT::expect_success("ctor from false", []{
            HN::boolean_socket_option_t<SO_REUSEADDR> const option(false);
            return not option.value()
                && not static_cast<bool>(option)
                && not option
                && noexcept(HN::boolean_socket_option_t<SO_REUSEADDR>(false))
                ;
        }),
    KT::expect_success("ctor from true", []{
            HN::boolean_socket_option_t<SO_REUSEADDR> const option(true);
            return option.value()
                && static_cast<bool>(option)
                && not not option
                && noexcept(HN::boolean_socket_option_t<SO_REUSEADDR>(true))
                ;
        }),
    KT::expect_success("assignment from bool", []{
            HN::boolean_socket_option_t<SO_REUSEADDR> option;
            decltype(option)& ref = option = true;
            return KT::use(ref)
                && option.value()
                && static_cast<bool>(option)
                && not not option
                && noexcept(option = true)
                ;
        }),
    KT::expect_success("accessors", []{
            HN::boolean_socket_option_t<SO_REUSEADDR> const option;
            int const protocol{};
            return KT::use(option)
                && noexcept(option.level(protocol))
                && option.level(protocol) == SOL_SOCKET
                && noexcept(option.name(protocol))
                && option.name(protocol) == SO_REUSEADDR
                && noexcept(option.data(protocol))
                && option.data(protocol) != nullptr
                && noexcept(option.size(protocol))
                && option.size(protocol) == sizeof(int)
                && KT::type<bool> == KT::type<decltype(option.value())>
                && noexcept(static_cast<bool>(option))
                && noexcept(not option)
                && KT::type<bool> == KT::type<decltype(not option)>
                ;
        }),
};

static KT::add_tests suite("boolean_socket_option", ::tests);
