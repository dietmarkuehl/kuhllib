// kuhl/mini/kuhlmini_string.t.cpp                                    -*-C++-*-
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

#include "kuhl/mini/string.hpp"
#include "kuhl/mini/ostream.hpp"
#include "kuhl/mini/streambuf.hpp"
#include <stdlib.h>
#include <string.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

namespace {
    class testbuf
        : public KM::streambuf {
        char buffer[1024];
    public:
        testbuf() { this->setp(this->buffer, this->buffer + sizeof(buffer) - 1); }
        auto c_str() const -> char const* {
            *this->pptr() = 0;
            return this->pbase();
        }
    };

}

// ----------------------------------------------------------------------------

static auto test_concat()
    -> bool {
    KM::string s0("hello");
    s0 += KM::string(" world");
    return s0 == "hello world"
        && (KM::string("goodbye") + KM::string(" world")) == "goodbye world"
        ;
}

// ----------------------------------------------------------------------------

static auto test_output()
    -> bool {
    testbuf     sbuf;
    KM::ostream out(&sbuf);
    out << KM::string("hello world");
    return strcmp(sbuf.c_str(), "hello world") == 0
        ;
}

// ----------------------------------------------------------------------------

static auto test()
    -> bool {
    KM::string str("hello");
    return strlen(KM::string().c_str()) == 0
        && strcmp(KM::string("hello world").c_str(), "hello world") == 0
        && KM::string("hello")   == KM::string("hello")
        && !(KM::string("hello") == KM::string("world"))
        && !(KM::string("hello") != KM::string("hello"))
        && KM::string("hello")   != KM::string("world")
        && "hello"   == KM::string("hello")
        && !("hello" == KM::string("world"))
        && !("hello" != KM::string("hello"))
        && "hello"   != KM::string("world")
        && KM::string("hello")   == "hello"
        && !(KM::string("hello") == "world")
        && !(KM::string("hello") != "hello")
        && KM::string("hello")   != "world"
        && KM::string(str) == "hello"
        && test_output()
        && test_concat()
        ;
}

// ----------------------------------------------------------------------------

auto main()
    -> int
{
    return test()? EXIT_SUCCESS: EXIT_FAILURE;
}
