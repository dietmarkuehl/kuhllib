// nstd/cursor/model_single_pass.t.cpp                                -*-C++-*-
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

#include "nstd/cursor/model_single_pass.hpp"
#include "nstd/cursor/single_pass.hpp"
#include "nstd/cursor/category.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace NC = nstd::cursor;
namespace NT = nstd::type_traits;

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("category is single_pass", []()->bool{
            return NT::is_same<NC::single_pass, NC::category_t<NC::model_single_pass<int>>>::value
                ;
        }),
    kuhl::test::expect_success("single_pass_begin() return", []()->bool{
            int array[] = { 123 };
            auto begin(NC::single_pass_begin(array));
            return NT::is_same<NC::model_single_pass<int>, decltype(begin)>::value
                && begin.get_pointer() == array
                && begin.get_key().get_key() == array[0]
                ;
        }),
    kuhl::test::expect_success("single_pass_end() return", []()->bool{
            int array[] = { 123 };
            auto end(NC::single_pass_end(array));
            return NT::is_same<NC::model_single_pass_end<int>, decltype(end)>::value
                //-dk:TODO && end.get_pointer() == nstd::cursor::end(array)
                && end.get_pointer() == array + 1
                ;
        }),
    kuhl::test::expect_success("begin isn't the end", []()->bool{
            int array[] = { 123 };
            return !NC::at_same_pos(NC::single_pass_begin(array), NC::single_pass_end(array))
                ;
        }),
    kuhl::test::expect_success("key at single_pass_begin()", []()->bool{
            int array[] = { 123 };
            auto begin(NC::single_pass_begin(array));
            return NT::is_same<NC::model_key<int>, decltype(NC::key(begin))>::value
                && NC::key(begin).get_key() == array[0]
                ;
        }),
    kuhl::test::expect_success("advanced begin is the end", []()->bool{
            int array[] = { 123 };
            auto begin(NC::single_pass_begin(array));
            NC::step(begin);
            return NC::at_same_pos(begin, NC::single_pass_end(array))
                ;
        }),
};

int main(int ac, char* av[])
{
    return kuhl::test::run_tests("cursor::model_single_pass", ac, av, ::tests);
}
