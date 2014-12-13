// src/nstd/cursor/array.t.cpp                                        -*-C++-*-
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

#include "nstd/cursor/array.hpp"
#include "kuhl/test.hpp"
#include "nstd/type_traits/is_same.hpp"

namespace NT = nstd::type_traits;

// ----------------------------------------------------------------------------

static kuhl::test::testcase const tests[] = {
    kuhl::test::expect_success("size(array)", []()->bool{
            int                array[]   = { 1 };
            int const          carray[]  = { 1, 2 };
            int volatile       varray[]  = { 1, 2, 3 };
            int const volatile cvarray[] = { 1, 2, 3, 4 };
            return nstd::cursor::size(array)   == 1u
                && nstd::cursor::size(carray)  == 2u
                && nstd::cursor::size(varray)  == 3u
                && nstd::cursor::size(cvarray) == 4u
                && NT::is_same<std::size_t, decltype(nstd::cursor::size(array))>::value
                && NT::is_same<std::size_t, decltype(nstd::cursor::size(carray))>::value
                && NT::is_same<std::size_t, decltype(nstd::cursor::size(varray))>::value
                && NT::is_same<std::size_t, decltype(nstd::cursor::size(cvarray))>::value
                ;
        }),
    kuhl::test::expect_success("begin(array)", []()->bool{
            int                array[]   = { 1 };
            int const          carray[]  = { 1, 2 };
            int volatile       varray[]  = { 1, 2, 3 };
            int const volatile cvarray[] = { 1, 2, 3, 4 };
            return nstd::cursor::begin(array)   == array
                && nstd::cursor::begin(carray)  == carray
                && nstd::cursor::begin(varray)  == varray
                && nstd::cursor::begin(cvarray) == cvarray
                && NT::is_same<int*,                decltype(nstd::cursor::begin(array))>::value
                && NT::is_same<int const *,         decltype(nstd::cursor::begin(carray))>::value
                && NT::is_same<int volatile*,       decltype(nstd::cursor::begin(varray))>::value
                && NT::is_same<int const volatile*, decltype(nstd::cursor::begin(cvarray))>::value
                ;
        }),
    kuhl::test::expect_success("end(array)", []()->bool{
            int                array[]   = { 1 };
            int const          carray[]  = { 1, 2 };
            int volatile       varray[]  = { 1, 2, 3 };
            int const volatile cvarray[] = { 1, 2, 3, 4 };
            return nstd::cursor::end(array)   == array + nstd::cursor::size(array)
                && nstd::cursor::end(carray)  == carray + nstd::cursor::size(carray)
                && nstd::cursor::end(varray)  == varray + nstd::cursor::size(varray)
                && nstd::cursor::end(cvarray) == cvarray + nstd::cursor::size(cvarray)
                && NT::is_same<int*,                decltype(nstd::cursor::end(array))>::value
                && NT::is_same<int const *,         decltype(nstd::cursor::end(carray))>::value
                && NT::is_same<int volatile*,       decltype(nstd::cursor::end(varray))>::value
                && NT::is_same<int const volatile*, decltype(nstd::cursor::end(cvarray))>::value
                ;
        }),
};

int main(int ac, char* av[])
{
    return kuhl::test::run_tests("array tools", ac, av, ::tests);
}
