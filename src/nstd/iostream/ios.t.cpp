// nstd/iostream/ios.t.cpp                                            -*-C++-*-
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

#include "nstd/iostream/ios.hpp"
#include "nstd/limits/numeric_limits.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NL = ::nstd;
namespace NS = ::nstd;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("declared types", [](KT::context& c)->bool{
            return KT::assert_declared<NI::streamoff>(c, "streamoff")
                && KT::assert_declared<NI::streamsize>(c, "streamsize")
                && KT::assert_declared<NI::ios_base>(c, "ios_base")
                && KT::assert_type<NI::basic_ios<char, NS::char_traits<char> >,
                                   NI::basic_ios<char> >(c, "basic_ios<char>")
                && KT::assert_type<NI::basic_ios<char16_t, NS::char_traits<char16_t> >,
                                   NI::basic_ios<char16_t> >(c, "basic_ios<char16_t>")
                && KT::assert_type<NI::basic_ios<char32_t, NS::char_traits<char32_t> >,
                                   NI::basic_ios<char32_t> >(c, "basic_ios<char32_t>")
                && KT::assert_type<NI::basic_ios<wchar_t, NS::char_traits<wchar_t> >,
                                   NI::basic_ios<wchar_t> >(c, "basic_ios<wchar_t>")
                ;
        }),
    KT::expect_success("manipulator declarations", [](KT::context& c)->bool{
            return KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::boolalpha)>(c, "boolalpha()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::noboolalpha)>(c, "noboolalpha()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::showbase)>(c, "showbase()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::noshowbase)>(c, "noshowbase()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::showpoint)>(c, "showpoint()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::noshowpoint)>(c, "noshowpoint()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::showpos)>(c, "showpos()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::noshowpos)>(c, "noshowpos()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::skipws)>(c, "skipws()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::noskipws)>(c, "noskipws()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::uppercase)>(c, "uppercase()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::nouppercase)>(c, "nouppercase()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::unitbuf)>(c, "unitbuf()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::nounitbuf)>(c, "nounitbuf()")

                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::internal)>(c, "internal()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::left)>(c, "left()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::right)>(c, "right()")

                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::dec)>(c, "dec()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::hex)>(c, "hex()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::oct)>(c, "oct()")

                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::fixed)>(c, "fixed()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::scientific)>(c, "scientific()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::hexfloat)>(c, "hexfloat()")
                && KT::assert_type<NI::ios_base&(NI::ios_base&), decltype(NI::defaultfloat)>(c, "defaultfloat()")
                ;
        }),
    KT::expect_success("error codes", [](KT::context& c)->bool{
            //-dk:TODO NS::error_code (*make)(NI::io_errc) = NI::make_error_code;
            return KT::assert_type<NI::io_errc, decltype(NI::io_errc::stream)>(c, "io_errc::stream")
                && KT::assert_equal(c, "io_errc::stream", 1, int(NI::io_errc::stream))
                && KT::assert_static_true<NI::is_error_code_enum<NI::io_errc> >(c, "io_errc is_error_code_enum")
                //-dk:TODO make_error_code() may be overloaded && KT::assert_true(c, "make_error_code", make)
                && KT::assert_type<NS::error_code(NI::io_errc), decltype(NI::make_error_code)>(c, "make_error_code")
                && KT::assert_true(c, "make_error_code(io_errc) noexcept",
                                   noexcept(NI::make_error_code(NI::io_errc::stream)))
                //-dk:TODO make_error_condition() may also be overloaded
                && KT::assert_type<NS::error_condition(NI::io_errc),
                                   decltype(NI::make_error_condition)>(c, "make_error_condition")
                && KT::assert_true(c, "make_error_condition(io_errc) noexcept",
                                   noexcept(NI::make_error_condition(NI::io_errc::stream)))
                && KT::assert_type<NS::error_category const&(), decltype(NI::iostream_category)>(c, "iostream_category")
                && KT::assert_true(c, "iostream_category() noexcept", noexcept(NI::iostream_category()))
                ;
        }),
    KT::expect_success("types [stream.types]", [](KT::context& c)->bool{
            return KT::assert_true(c, "streamoff: integer", NL::numeric_limits<NI::streamoff>::is_integer)
                && KT::assert_true(c, "streamoff: signed", NL::numeric_limits<NI::streamoff>::is_signed)
                && KT::assert_true(c, "streamsize: integer", NL::numeric_limits<NI::streamsize>::is_integer)
                && KT::assert_true(c, "streamsize: signed", NL::numeric_limits<NI::streamsize>::is_signed)
                ;
        }),
    KT::expect_success("class ios_base [ios.base]", [](KT::context& c)->bool{
            return KT::assert_declared<NI::ios_base::failure>(c, "ios_base::failure")
                ;
        }),
    KT::expect_success("concrete object", [](KT::context& c)->bool{
            NI::ios  ios(0);
            NI::wios wios(0);
            KT::use(ios);
            KT::use(wios);
           return true;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("iostream::ios", ac, av, ::tests);
}
