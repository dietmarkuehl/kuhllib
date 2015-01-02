// kuhl/mini/kuhlmini_streambuf.t.cpp                                 -*-C++-*-
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

#include "kuhl/mini/streambuf.hpp"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

namespace {
    class testbuf
        : KM::streambuf {
        char buffer[1023];
        bool overflow_seen;

        auto overflow(int c) -> int {
            this->overflow_seen = true;
            return -1;
        }
    public:
        testbuf(): overflow_seen(false) {}
        auto after_ctor() -> bool {
            return this->pbase() == 0
                && this->pptr()  == 0
                && this->epptr() == 0
                ;
        }
        auto setup() -> bool {
            this->setp(this->buffer, this->buffer + 1023);
            return this->pbase() == this->buffer
                && this->pptr()  == this->buffer
                && this->epptr() == this->buffer + 1023
                && (this->pbump(10), true)
                && this->pptr()  == this->buffer + 10
                && (this->pbump(-5), true)
                && this->pptr()  == this->buffer + 5
                ;
        }
        auto output0() -> bool {
            this->setp(this->buffer, this->buffer + 1023);
            return 'h' == this->sputc('h')
                && 'e' == this->sputc('e')
                && 'l' == this->sputc('l')
                && 'l' == this->sputc('l')
                && 'o' == this->sputc('o')
                && (*this->pptr() = 0) == 0
                && this->pbase() == this->buffer
                && this->pptr()  == this->buffer + 5
                && this->epptr() == this->buffer + 1023
                && strcmp(this->pbase(), "hello") == 0
                && !this->overflow_seen
                ;
        }
        auto output1() -> bool {
            this->setp(this->buffer, this->buffer + 4);
            return 'h' == this->sputc('h')
                && 'e' == this->sputc('e')
                && 'l' == this->sputc('l')
                && 'l' == this->sputc('l')
                && -1  == this->sputc('o')
                && (*this->pptr() = 0) == 0
                && this->pbase() == this->buffer
                && this->pptr()  == this->buffer + 4
                && this->epptr() == this->buffer + 4
                && strcmp(this->pbase(), "hell") == 0
                && this->overflow_seen
                ;
        }
    };

    class pointer {
        testbuf* sbuf;
    public:
        pointer(testbuf* sbuf): sbuf(sbuf) {}
        ~pointer() { this->sbuf->~testbuf(); }
        auto operator->() -> testbuf* { return this->sbuf; }
    };
}

auto operator new(size_t, char* buffer) -> void* { return buffer; }
auto operator delete(void*, char*) -> void {}

// ----------------------------------------------------------------------------

static auto test() -> bool {
    char buffer[sizeof(testbuf)];
    for (char* it(buffer), * end(it + sizeof(testbuf)); it != end; ++it) {
        *it = 0xA5;
    }
    pointer sbuf(new(buffer) testbuf);
    
    return sbuf->after_ctor()
        && sbuf->setup()
        && sbuf->output0()
        && sbuf->output1()
        ;
}

// ----------------------------------------------------------------------------

auto main()
    -> int
{
    return test()? EXIT_SUCCESS: EXIT_FAILURE;
}
