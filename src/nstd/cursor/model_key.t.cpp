// nstd/cursor/model_key.t.cpp                                        -*-C++-*-
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

#include "nstd/cursor/model_key.hpp"
#include "kuhl/test.hpp"

namespace KT = kuhl::test;
namespace NC = nstd::cursor;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("simple construction", [](KT::context& c)->bool{
            return assert_equal(c, "explicit construction", NC::model_key<int>('a').get_key(), 'a')
                ;
        }),
    KT::expect_success("assert type of get_key()", [](KT::context& c)->bool{
            NC::model_key<int> key(10);
            return assert_equal(c, "get_key()", key.get_key(), 10)
                && KT::assert_type<int const&, decltype(key.get_key())>(c, "type");
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("cursor::model_key", ac, av, ::tests);
}
