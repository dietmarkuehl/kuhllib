// nstd/iostream/sstream_fwd.t.cpp                                    -*-C++-*-
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

#include "nstd/iostream/sstream_fwd.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NM = ::nstd;
namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic_stringbuf<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_stringbuf<char, NS::char_traits<char>, NM::allocator<char> >,
                                   NI::basic_stringbuf<char> >(c, "basic_stringbuf<char>")
                && KT::assert_type<NI::basic_stringbuf<char16_t, NS::char_traits<char16_t>, NM::allocator<char16_t> >,
                                   NI::basic_stringbuf<char16_t> >(c, "basic_stringbuf<char16_t>")
                && KT::assert_type<NI::basic_stringbuf<char32_t, NS::char_traits<char32_t>, NM::allocator<char32_t> >,
                                   NI::basic_stringbuf<char32_t> >(c, "basic_stringbuf<char32_t>")
                && KT::assert_type<NI::basic_stringbuf<wchar_t, NS::char_traits<wchar_t>, NM::allocator<wchar_t> >,
                                   NI::basic_stringbuf<wchar_t> >(c, "basic_stringbuf<wchar_t>")
                && KT::assert_type<NI::basic_stringbuf<char>, NI::stringbuf>(c, "stringbuf")
                && KT::assert_type<NI::basic_stringbuf<wchar_t>, NI::wstringbuf>(c, "wstringbuf")
                ;
        }),
    KT::expect_success("basic_istringstream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_istringstream<char, NS::char_traits<char>, NM::allocator<char> >,
                                   NI::basic_istringstream<char> >(c, "basic_istringstream<char>")
                && KT::assert_type<NI::basic_istringstream<char16_t, NS::char_traits<char16_t>, NM::allocator<char16_t> >,
                                   NI::basic_istringstream<char16_t> >(c, "basic_istringstream<char16_t>")
                && KT::assert_type<NI::basic_istringstream<char32_t, NS::char_traits<char32_t>, NM::allocator<char32_t> >,
                                   NI::basic_istringstream<char32_t> >(c, "basic_istringstream<char32_t>")
                && KT::assert_type<NI::basic_istringstream<wchar_t, NS::char_traits<wchar_t>, NM::allocator<wchar_t> >,
                                   NI::basic_istringstream<wchar_t> >(c, "basic_istringstream<wchar_t>")
                && KT::assert_type<NI::basic_istringstream<char>, NI::istringstream>(c, "istringstream")
                && KT::assert_type<NI::basic_istringstream<wchar_t>, NI::wistringstream>(c, "wistringstream")
                ;
        }),
    KT::expect_success("basic_ostringstream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_ostringstream<char, NS::char_traits<char>, NM::allocator<char> >,
                                   NI::basic_ostringstream<char> >(c, "basic_ostringstream<char>")
                && KT::assert_type<NI::basic_ostringstream<char16_t, NS::char_traits<char16_t>, NM::allocator<char16_t> >,
                                   NI::basic_ostringstream<char16_t> >(c, "basic_ostringstream<char16_t>")
                && KT::assert_type<NI::basic_ostringstream<char32_t, NS::char_traits<char32_t>, NM::allocator<char32_t> >,
                                   NI::basic_ostringstream<char32_t> >(c, "basic_ostringstream<char32_t>")
                && KT::assert_type<NI::basic_ostringstream<wchar_t, NS::char_traits<wchar_t>, NM::allocator<wchar_t> >,
                                   NI::basic_ostringstream<wchar_t> >(c, "basic_ostringstream<wchar_t>")
                && KT::assert_type<NI::basic_ostringstream<char>, NI::ostringstream>(c, "ostringstream")
                && KT::assert_type<NI::basic_ostringstream<wchar_t>, NI::wostringstream>(c, "wostringstream")
                ;
        }),
    KT::expect_success("basic_stringstream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_stringstream<char, NS::char_traits<char>, NM::allocator<char> >,
                                   NI::basic_stringstream<char> >(c, "basic_stringstream<char>")
                && KT::assert_type<NI::basic_stringstream<char16_t, NS::char_traits<char16_t>, NM::allocator<char16_t> >,
                                   NI::basic_stringstream<char16_t> >(c, "basic_stringstream<char16_t>")
                && KT::assert_type<NI::basic_stringstream<char32_t, NS::char_traits<char32_t>, NM::allocator<char32_t> >,
                                   NI::basic_stringstream<char32_t> >(c, "basic_stringstream<char32_t>")
                && KT::assert_type<NI::basic_stringstream<wchar_t, NS::char_traits<wchar_t>, NM::allocator<wchar_t> >,
                                   NI::basic_stringstream<wchar_t> >(c, "basic_stringstream<wchar_t>")
                && KT::assert_type<NI::basic_stringstream<char>, NI::stringstream>(c, "stringstream")
                && KT::assert_type<NI::basic_stringstream<wchar_t>, NI::wstringstream>(c, "wstringstream")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("iostream/sstream_fwd", ac, av, ::tests);
}
