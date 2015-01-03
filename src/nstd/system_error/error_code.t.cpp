// nstd/system_error/error_code.t.cpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/system_error/error_code.hpp"
#include "nstd/string/string.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "kuhl/test.hpp"

namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    enum custom_error {};
}
namespace nstd {
    template <>
    struct is_error_code_enum<custom_error>
        : ::nstd::type_traits::true_type {
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("error_code is declared", [](KT::context& c)->bool{
            return KT::assert_declared<NS::error_code>(c, "error_code")
                ;
        }),
    KT::expect_success("error_code operations", [](KT::context& c)->bool{
            NS::error_category const* cat{0};
            custom_error          err{};
            NS::error_code*       ec{0};
            NS::error_code const* cec{0};
            KT::use(cat, err, ec, cec);
            return KT::assert_type<NS::error_code, decltype(NS::error_code())>(c, "default ctor")
                && KT::assert_true(c, "default doesn't throw", noexcept(NS::error_code()))
                && KT::assert_type<NS::error_code, decltype(NS::error_code(0, *cat))>(c, "ctor from category")
                && KT::assert_true(c, "category ctor doesn't throw", noexcept(NS::error_code(0, *cat)))
                && KT::assert_type<NS::error_code, decltype(NS::error_code(err))>(c, "ctor from ErrorCode")
                && KT::assert_true(c, "ErrorCode ctor doesn't throw", noexcept(NS::error_code(err)))
                && KT::assert_type<void, decltype(ec->assign(0, *cat))>(c, "assign()")
                && KT::assert_true(c, "assign() doesn't throw", noexcept(ec->assign(0, *cat)))
                && KT::assert_type<NS::error_code&, decltype(*ec = err)>(c, "operator=()")
                && KT::assert_true(c, "operator=() doesn't throw", noexcept(*ec = err))
                && KT::assert_type<void, decltype(ec->clear())>(c, "clear()")
                && KT::assert_true(c, "clear() doesn't throw", noexcept(ec->clear()))
                && KT::assert_type<int, decltype(cec->value())>(c, "value()")
                && KT::assert_true(c, "value() doesn't throw", noexcept(cec->value()))
                && KT::assert_type<NS::error_category const&, decltype(cec->category())>(c, "category()")
                && KT::assert_true(c, "category() doesn't throw", noexcept(cec->category()))
                && KT::assert_type<NS::error_condition,
                                   decltype(cec->default_error_condition())>(c, "default_error_condition()")
                && KT::assert_true(c, "default_error_condition() doesn't throw",
                                   noexcept(cec->default_error_condition()))
                && KT::assert_type<NS::string, decltype(cec->message())>(c, "message()")
                && KT::assert_type<bool, decltype(bool(*cec))>(c, "conversion to bool")
                && KT::assert_true(c, "conversion to bool doesn't throw", noexcept(bool(*cec)))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("system_error/error_code", ac, av, ::tests);
}
