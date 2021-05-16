// nstd/projection/model_readable.t.cpp                               -*-C++-*-
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

#include "nstd/projection/model_readable.hpp"
#include "kuhl/test.hpp"

namespace NP = nstd::projection;
namespace NC = nstd::cursor;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("result", [](KT::context& c)->bool{
            constexpr NP::model_readable<> projection;
            return assert_equal(c, "three", 3, projection(NC::model_key<int>(3)).get_value())
                ;
        }),
    KT::expect_success("type", [](KT::context& c)->bool{
            constexpr NP::model_readable<> projection;
            auto result(projection(NC::model_key<long>(3l)));
            return KT::assert_type<NP::model_value<long>, decltype(result)>(c, "model_value<long>")
                ;
        }),
};

static KT::add_tests suite("projection::model_readable", ::tests);
