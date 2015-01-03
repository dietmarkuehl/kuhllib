// nstd/iostream/iostream_fwd.t.cpp                                   -*-C++-*-
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

#include "nstd/iostream/iostream_fwd.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic_iostream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_iostream<char, NS::char_traits<char> >,
                                   NI::basic_iostream<char> >(c, "basic_iostream<char>")
                && KT::assert_type<NI::basic_iostream<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_iostream<char16_t> >(c, "basic_iostream<char16_t>")
                && KT::assert_type<NI::basic_iostream<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_iostream<char32_t> >(c, "basic_iostream<char32_t>")
                && KT::assert_type<NI::basic_iostream<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_iostream<wchar_t> >(c, "basic_iostream<wchar_t>")
                && KT::assert_type<NI::basic_iostream<char>, NI::iostream>(c, "iostream")
                && KT::assert_type<NI::basic_iostream<wchar_t>, NI::wiostream>(c, "wiostream")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("iostream/iostream_fwd", ac, av, ::tests);
}
