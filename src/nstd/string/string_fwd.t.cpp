// nstd/string/string_fwd.t.cpp                                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/string/string_fwd.hpp"
#include "kuhl/test.hpp"

namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_failure("string is declared", [](KT::context& c)->bool{
            return KT::assert_declared<NS::string>(c, "string is declared")
                && KT::assert_type<NS::basic_string<char, NS::char_traits<char> >,
                                   NS::basic_string<char> >(c, "basic_string<char> default")
                && KT::assert_type<NS::basic_string<wchar_t, NS::char_traits<wchar_t> >,
                                   NS::basic_string<wchar_t> >(c, "basic_string<wchar_t> default")
                && KT::assert_type<NS::basic_string<char>, NS::string>(c, "string")
                && KT::assert_type<NS::basic_string<wchar_t>, NS::wstring>(c, "wstring")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("string/string_fwd", ac, av, ::tests);
}