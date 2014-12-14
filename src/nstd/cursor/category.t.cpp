// nstd/cursor/category.t.cpp                                         -*-C++-*-
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

#include "nstd/cursor/category.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "kuhl/test.hpp"

namespace NC = nstd::cursor;
namespace NT = nstd::type_traits;

// ----------------------------------------------------------------------------

namespace
{
    struct single_pass: NC::single_pass {};
    struct forward: NC::forward {};
    struct bidirectional: NC::bidirectional {};
    struct random_access: NC::random_access {};
}

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("detect single_pass from tagged", []()->bool{
            return NT::is_same<NC::single_pass, typename NC::category<single_pass>::type>::value
                && NT::is_same<NC::single_pass, NC::category_t<single_pass>>::value
                ;
        }),
    kuhl::test::expect_success("detect forward from tagged", []()->bool{
            return NT::is_same<NC::forward, typename NC::category<forward>::type>::value
                && NT::is_same<NC::forward, NC::category_t<forward>>::value
                ;
        }),
    kuhl::test::expect_success("detect bidirectional from tagged", []()->bool{
            return NT::is_same<NC::bidirectional, typename NC::category<bidirectional>::type>::value
                && NT::is_same<NC::bidirectional, NC::category_t<bidirectional>>::value
                ;
        }),
    kuhl::test::expect_success("detect random_access from tagged", []()->bool{
            return NT::is_same<NC::random_access, typename NC::category<random_access>::type>::value
                && NT::is_same<NC::random_access, NC::category_t<random_access>>::value
                ;
        }),
    kuhl::test::expect_success("char is a random_access cursor", []()->bool{
            return NT::is_same<NC::random_access, typename NC::category<char>::type>::value
                && NT::is_same<NC::random_access, NC::category_t<char>>::value
                ;
        }),
        //-dk:TODO detect cursor categories from iterator traits
};

int main(int ac, char* av[])
{
    return kuhl::test::run_tests("TODO", ac, av, ::tests);
}
