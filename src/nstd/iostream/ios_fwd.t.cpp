// nstd/iostream/ios_fwd.t.cpp                                        -*-C++-*-
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

#include "nstd/iostream/ios_fwd.hpp"
#include "nstd/string/char_traits.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_failure("basic_ios is declared", [](KT::context& c)->bool{
            NI::basic_ios<char>*     ios(0);
            NI::basic_ios<wchar_t>*  wios(0);
            NI::basic_ios<char16_t>* ios16(0);
            NI::basic_ios<char32_t>* ios32(0);
            return KT::assert_false(c, "char", ios)
                && KT::assert_false(c, "wchar_t", wios)
                && KT::assert_false(c, "char16_t", ios16)
                && KT::assert_false(c, "char32_t", ios32)
                && KT::assert_type<NI::basic_ios<char>,
                                   NI::basic_ios<char, NS::char_traits<char> > >(c, "char default")
                && KT::assert_type<NI::basic_ios<wchar_t>,
                                   NI::basic_ios<wchar_t, NS::char_traits<wchar_t> > >(c, "wchar_t default")
                && KT::assert_type<NI::basic_ios<char16_t>,
                                   NI::basic_ios<char16_t, NS::char_traits<char16_t> > >(c, "char16_t default")
                && KT::assert_type<NI::basic_ios<char32_t>,
                                   NI::basic_ios<char32_t, NS::char_traits<char32_t> > >(c, "char32_t default")
                ;
        }),
    KT::expect_success("type alias", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_ios<char>, NI::ios>(c, "char alias")
                && KT::assert_type<NI::basic_ios<wchar_t>, NI::wios>(c, "wchar_t alias")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("iostream::ios_fwd", ac, av, ::tests);
}
