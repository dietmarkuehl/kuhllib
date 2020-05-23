// src/io_context.cpp                                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include <io_context.hpp>

#include <iostream> //-dk:TOOD remove
#include <limits>

// ----------------------------------------------------------------------------

int io_context = 0;

// ----------------------------------------------------------------------------

cxxrt::net::io_context::io_context()
{
}

// ----------------------------------------------------------------------------

cxxrt::net::io_context::count_type
cxxrt::net::io_context::run()
{
    count_type n{};
    while (this->run_one())
    {
        if (n != std::numeric_limits<count_type>::max()) {
            ++n;
        }
    }
    return n;
}

cxxrt::net::io_context::count_type
cxxrt::net::io_context::run_one()
{
    std::cout << "*** WARNING: io_context is still on strike and doesn't do any work\n";
    //-dk:TODO execute some work
    return 0u;
}
