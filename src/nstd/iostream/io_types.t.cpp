// nstd/iostream/io_types.t.cpp                                       -*-C++-*-
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

#include "nstd/iostream/io_types.hpp"
#include "nstd/limits/numeric_limits.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NL = ::nstd;
namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("types [stream.types]", [](KT::context& c)->bool{
            return KT::assert_true(c, "streamoff: integer", NL::numeric_limits<NI::streamoff>::is_integer)
                && KT::assert_true(c, "streamoff: signed", NL::numeric_limits<NI::streamoff>::is_signed)
                && KT::assert_true(c, "streamsize: integer", NL::numeric_limits<NI::streamsize>::is_integer)
                && KT::assert_true(c, "streamsize: signed", NL::numeric_limits<NI::streamsize>::is_signed)
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
};

int main(int ac, char* av[])
{
    return KT::run_tests("iostream/io_types", ac, av, ::tests);
}
