// kuhl/test/context.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_KUHL_TEST_CONTEXT
#define INCLUDED_KUHL_TEST_CONTEXT

#include "kuhl/mini/streambuf.hpp"
#include "kuhl/mini/ostream.hpp"

// ----------------------------------------------------------------------------

namespace kuhl
{
    namespace test {
        class context_sbuf;
        class context;
    }

}

// ----------------------------------------------------------------------------

class kuhl::test::context_sbuf
    : public kuhl::mini::streambuf
{
    char buffer[256];
public:
    context_sbuf();
    auto reset() -> void;
    auto empty() const -> bool;
    auto c_str() const -> char const*;
};

// ----------------------------------------------------------------------------

class kuhl::test::context
    : private virtual kuhl::test::context_sbuf
    , public kuhl::mini::ostream
{
public:
    context();
    ~context();
    auto reset() -> void;
    auto empty() const -> bool;
    auto c_str() const -> const char*;
};

// ----------------------------------------------------------------------------

#endif
