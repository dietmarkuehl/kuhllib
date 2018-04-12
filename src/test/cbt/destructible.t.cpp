// cbt/destructible.t.cpp                                             -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "cbt/destructible.hpp"
#include "cbt/test.hpp"
#include <iostream>

// ----------------------------------------------------------------------------

namespace {
    struct indestructible {
        indestructible() = delete;
        ~indestructible() = delete;
    };
    struct throwing {
        throwing() = delete;
        ~throwing() noexcept(false) {}
    };
}

// ----------------------------------------------------------------------------

int main()
{
    int i(0);
    return cbt::main_test({ !cbt::Destructible<indestructible>(std::cout, "indestructible")
                          , !cbt::Destructible<throwing>(std::cout, "throwing")
                          , cbt::Destructible(std::cout, "int", 17)
                          , cbt::Destructible(std::cout, "int&", i)
                          });
}
