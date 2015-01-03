// nstd/iostream/iosfwd.t.cpp                                         -*-C++-*-
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

#include "nstd/iostream/iosfwd.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NC = ::nstd;
namespace NM = ::nstd;
namespace NS = ::nstd;
namespace KT = ::kuhl::test;

struct mbstate_t; //-dk:TODO pull from a suitable location

// ----------------------------------------------------------------------------

namespace {
    struct foo {};
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("char specializations exist", [](KT::context& c)->bool{
            return KT::assert_declared<NS::char_traits<char> >(c, "char_traits<char>")
                && KT::assert_declared<NS::char_traits<char16_t> >(c, "char_traits<char16_t>")
                && KT::assert_declared<NS::char_traits<char32_t> >(c, "char_traits<char32_t>")
                && KT::assert_declared<NS::char_traits<wchar_t> >(c, "char_traits<wchar_t>")
                ;
        }),
    KT::expect_success("allocator<T> is declared", [](KT::context& c)->bool{
            return KT::assert_declared<NM::allocator<foo> >(c, "allocator<T> is declared")
                ;
        }),
    KT::expect_success("basic_ios<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_ios<char, NS::char_traits<char> >,
                                   NI::basic_ios<char> >(c, "basic_ios<char>")
                && KT::assert_type<NI::basic_ios<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_ios<char16_t> >(c, "basic_ios<char16_t>")
                && KT::assert_type<NI::basic_ios<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_ios<char32_t> >(c, "basic_ios<char32_t>")
                && KT::assert_type<NI::basic_ios<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_ios<wchar_t> >(c, "basic_ios<wchar_t>")
                && KT::assert_type<NI::basic_ios<char>, NI::ios>(c, "ios")
                && KT::assert_type<NI::basic_ios<wchar_t>, NI::wios>(c, "wios")
                ;
        }),
    KT::expect_success("basic_streambuf<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_streambuf<char, NS::char_traits<char> >,
                                   NI::basic_streambuf<char> >(c, "basic_streambuf<char>")
                && KT::assert_type<NI::basic_streambuf<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_streambuf<char16_t> >(c, "basic_streambuf<char16_t>")
                && KT::assert_type<NI::basic_streambuf<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_streambuf<char32_t> >(c, "basic_streambuf<char32_t>")
                && KT::assert_type<NI::basic_streambuf<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_streambuf<wchar_t> >(c, "basic_streambuf<wchar_t>")
                && KT::assert_type<NI::basic_streambuf<char>, NI::streambuf>(c, "streambuf")
                && KT::assert_type<NI::basic_streambuf<wchar_t>, NI::wstreambuf>(c, "wstreambuf")
                ;
        }),
    KT::expect_success("basic_istream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_istream<char, NS::char_traits<char> >,
                                   NI::basic_istream<char> >(c, "basic_istream<char>")
                && KT::assert_type<NI::basic_istream<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_istream<char16_t> >(c, "basic_istream<char16_t>")
                && KT::assert_type<NI::basic_istream<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_istream<char32_t> >(c, "basic_istream<char32_t>")
                && KT::assert_type<NI::basic_istream<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_istream<wchar_t> >(c, "basic_istream<wchar_t>")
                && KT::assert_type<NI::basic_istream<char>, NI::istream>(c, "istream")
                && KT::assert_type<NI::basic_istream<wchar_t>, NI::wistream>(c, "wistream")
                ;
        }),
    KT::expect_success("basic_ostream<T> is declared", [](KT::context& c)->bool{
            return KT::assert_type<NI::basic_ostream<char, NS::char_traits<char> >,
                                   NI::basic_ostream<char> >(c, "basic_ostream<char>")
                && KT::assert_type<NI::basic_ostream<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_ostream<char16_t> >(c, "basic_ostream<char16_t>")
                && KT::assert_type<NI::basic_ostream<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_ostream<char32_t> >(c, "basic_ostream<char32_t>")
                && KT::assert_type<NI::basic_ostream<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_ostream<wchar_t> >(c, "basic_ostream<wchar_t>")
                && KT::assert_type<NI::basic_ostream<char>, NI::ostream>(c, "ostream")
                && KT::assert_type<NI::basic_ostream<wchar_t>, NI::wostream>(c, "wostream")
                ;
        }),
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
    KT::expect_success("istreambuf_iterator is declared", [](KT::context& c)->bool{
            return KT::assert_type<NC::istreambuf_iterator<char, NS::char_traits<char> >,
                                   NC::istreambuf_iterator<char> >(c, "istreambuf_iterator<char>")
                && KT::assert_type<NC::istreambuf_iterator<char16_t, NS::char_traits<char16_t> >,
                                   NC::istreambuf_iterator<char16_t> >(c, "istreambuf_iterator<char16_t>")
                && KT::assert_type<NC::istreambuf_iterator<char32_t, NS::char_traits<char32_t> >,
                                   NC::istreambuf_iterator<char32_t> >(c, "istreambuf_iterator<char32_t>")
                && KT::assert_type<NC::istreambuf_iterator<wchar_t, NS::char_traits<wchar_t> >,
                                   NC::istreambuf_iterator<wchar_t> >(c, "istreambuf_iterator<wchar_t>")
                ;
        }),
    KT::expect_success("ostreambuf_iterator is declared", [](KT::context& c)->bool{
            return KT::assert_type<NC::ostreambuf_iterator<char, NS::char_traits<char> >,
                                   NC::ostreambuf_iterator<char> >(c, "ostreambuf_iterator<char>")
                && KT::assert_type<NC::ostreambuf_iterator<char16_t, NS::char_traits<char16_t> >,
                                   NC::ostreambuf_iterator<char16_t> >(c, "ostreambuf_iterator<char16_t>")
                && KT::assert_type<NC::ostreambuf_iterator<char32_t, NS::char_traits<char32_t> >,
                                   NC::ostreambuf_iterator<char32_t> >(c, "ostreambuf_iterator<char32_t>")
                && KT::assert_type<NC::ostreambuf_iterator<wchar_t, NS::char_traits<wchar_t> >,
                                   NC::ostreambuf_iterator<wchar_t> >(c, "ostreambuf_iterator<wchar_t>")
                ;
        }),
    KT::expect_success("fpos<T> is declared", [](KT::context& c)->bool{
            return KT::assert_declared<NI::fpos<mbstate_t> >(c, "fpos<mbstate_t>")
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("iostream/iosfwd", ac, av, ::tests);
}
