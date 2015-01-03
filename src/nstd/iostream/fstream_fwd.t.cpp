// nstd/iostream/fstream_fwd.t.cpp                                    -*-C++-*-
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

#include "nstd/iostream/fstream_fwd.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic_filebuf<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_filebuf<char, NS::char_traits<char> >,
                                   NI::basic_filebuf<char> >(c, "basic_filebuf<char>")
                && KT::assert_type<NI::basic_filebuf<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_filebuf<char16_t> >(c, "basic_filebuf<char16_t>")
                && KT::assert_type<NI::basic_filebuf<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_filebuf<char32_t> >(c, "basic_filebuf<char32_t>")
                && KT::assert_type<NI::basic_filebuf<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_filebuf<wchar_t> >(c, "basic_filebuf<wchar_t>")
                && KT::assert_type<NI::basic_filebuf<char>, NI::filebuf>(c, "filebuf")
                && KT::assert_type<NI::basic_filebuf<wchar_t>, NI::wfilebuf>(c, "wfilebuf")
                ;
        }),
    KT::expect_success("basic_ifstream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_ifstream<char, NS::char_traits<char> >,
                                   NI::basic_ifstream<char> >(c, "basic_ifstream<char>")
                && KT::assert_type<NI::basic_ifstream<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_ifstream<char16_t> >(c, "basic_ifstream<char16_t>")
                && KT::assert_type<NI::basic_ifstream<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_ifstream<char32_t> >(c, "basic_ifstream<char32_t>")
                && KT::assert_type<NI::basic_ifstream<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_ifstream<wchar_t> >(c, "basic_ifstream<wchar_t>")
                && KT::assert_type<NI::basic_ifstream<char>, NI::ifstream>(c, "ifstream")
                && KT::assert_type<NI::basic_ifstream<wchar_t>, NI::wifstream>(c, "wifstream")
                ;
        }),
    KT::expect_success("basic_ofstream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_ofstream<char, NS::char_traits<char> >,
                                   NI::basic_ofstream<char> >(c, "basic_ofstream<char>")
                && KT::assert_type<NI::basic_ofstream<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_ofstream<char16_t> >(c, "basic_ofstream<char16_t>")
                && KT::assert_type<NI::basic_ofstream<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_ofstream<char32_t> >(c, "basic_ofstream<char32_t>")
                && KT::assert_type<NI::basic_ofstream<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_ofstream<wchar_t> >(c, "basic_ofstream<wchar_t>")
                && KT::assert_type<NI::basic_ofstream<char>, NI::ofstream>(c, "ofstream")
                && KT::assert_type<NI::basic_ofstream<wchar_t>, NI::wofstream>(c, "wofstream")
                ;
        }),
    KT::expect_success("basic_fstream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_fstream<char, NS::char_traits<char> >,
                                   NI::basic_fstream<char> >(c, "basic_fstream<char>")
                && KT::assert_type<NI::basic_fstream<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_fstream<char16_t> >(c, "basic_fstream<char16_t>")
                && KT::assert_type<NI::basic_fstream<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_fstream<char32_t> >(c, "basic_fstream<char32_t>")
                && KT::assert_type<NI::basic_fstream<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_fstream<wchar_t> >(c, "basic_fstream<wchar_t>")
                && KT::assert_type<NI::basic_fstream<char>, NI::fstream>(c, "fstream")
                && KT::assert_type<NI::basic_fstream<wchar_t>, NI::wfstream>(c, "wfstream")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("TODO", ac, av, ::tests);
}
