// nstd/system_error/system_error.t.cpp                               -*-C++-*-
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

#include "nstd/system_error/system_error.hpp"
#include "kuhl/test.hpp"

namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("system_error", [](KT::context& c)->bool{
            return KT::assert_declared<NS::system_error>(c, "system_error declared")
                ;
        }),
    KT::expect_success("system_error oprations", [](KT::context& c)->bool{
            NS::error_category const* cat{0};
            NS::error_code*           ec{0};
            NS::string const*         str{0};
            NS::system_error const*   error{0};
            KT::use(cat, ec, str, error);
            //-dk:TODO system_error derives from runtime_error
            return KT::assert_type<NS::system_error, decltype(NS::system_error(*ec, *str))>(c, "string ctor")
                && KT::assert_type<NS::system_error, decltype(NS::system_error(*ec, "err"))>(c, "literal ctor")
                && KT::assert_type<NS::system_error, decltype(NS::system_error(*ec))>(c, "error_code ctor")
                && KT::assert_type<NS::system_error,
                                   decltype(NS::system_error(0, *cat, *str))>(c, "category/string ctor")
                && KT::assert_type<NS::system_error,
                                   decltype(NS::system_error(0, *cat, "err"))>(c, "category/literal ctor")
                && KT::assert_type<NS::system_error,
                                   decltype(NS::system_error(0, *cat))>(c, "category ctor")
                && KT::assert_type<NS::error_code const&, decltype(error->code())>(c, "code type")
                && KT::assert_true(c, "code() doesn't throw", noexcept(error->code()))
                && KT::assert_type<char const*, decltype(error->what())>(c, "what type")
                && KT::assert_true(c, "what() doesn't throw", noexcept(error->what()))
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("system_error/system_error", ac, av, ::tests);
}
