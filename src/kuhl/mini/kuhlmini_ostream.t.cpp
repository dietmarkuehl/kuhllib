// kuhl/mini/kuhlmini_ostream.t.cpp                                   -*-C++-*-
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

#include "kuhl/mini/ostream.hpp"
#include "kuhl/mini/streambuf.hpp"
#include <stdlib.h>
#include <string.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

namespace {
    class teststream
        : public KM::ostream {
        KM::streambuf sbuf;
    public:
        teststream(KM::streambuf* sbuf)
            : KM::ios(&this->sbuf)
            , KM::ostream(sbuf) {
        }
    };

    class testbuf
        : public KM::streambuf {
        char buffer[1024];
    public:
        testbuf() { this->setp(buffer, buffer + sizeof(buffer) - 1); }
        auto c_str() const -> char const* {
            *this->pptr() = 0;
            return this->pbase();
        }
    };
}

// ----------------------------------------------------------------------------

static auto test_left()
    -> bool {
    KM::streambuf sbuf;
    KM::ostream   out(&sbuf);
    return out.flags() == KM::ios::dec
        && (out << KM::left).flags() == (KM::ios::dec | KM::ios::left)
        && out.setf(KM::ios::right, KM::ios::adjustfield) == (KM::ios::dec | KM::ios::left)
        && out.flags() == (KM::ios::dec | KM::ios::right)
        && (out << KM::left).flags() == (KM::ios::dec | KM::ios::left)
        ;
}

static auto test_right()
    -> bool {
    KM::streambuf sbuf;
    KM::ostream   out(&sbuf);
    return out.flags() == KM::ios::dec
        && (out << KM::right).flags() == (KM::ios::dec | KM::ios::right)
        && out.setf(KM::ios::left, KM::ios::adjustfield) == (KM::ios::dec | KM::ios::right)
        && out.flags() == (KM::ios::dec | KM::ios::left)
        && (out << KM::right).flags() == (KM::ios::dec | KM::ios::right)
        ;
}

// ----------------------------------------------------------------------------

static auto test_string_literal()
    -> bool {
    testbuf     sbuf;
    KM::ostream out(&sbuf);
    out << "hello world";
    return strcmp("hello world", sbuf.c_str()) == 0
        ;
}

// ----------------------------------------------------------------------------

template <typename T>
static auto test_value(T value, char const* expect)
    -> bool {
    testbuf     sbuf;
    KM::ostream out(&sbuf);
    out << value;
    return strcmp(sbuf.c_str(), expect) == 0
        ;
}

// ----------------------------------------------------------------------------

static auto test_char()
    -> bool {
    return test_value('a', "a")
        && test_value(' ', " ")
        ;
}

// ----------------------------------------------------------------------------

static auto test_int()
    -> bool {
    return test_value(0, "0")
        && test_value(12345, "12345")
        && test_value(-12345, "-12345")
        ;
}
static auto test_long()
    -> bool {
    return test_value(0l, "0")
        && test_value(12345l, "12345")
        && test_value(-12345l, "-12345")
        ;
}
static auto test_long_long()
    -> bool {
    return test_value(0ll, "0")
        && test_value(12345ll, "12345")
        && test_value(-12345ll, "-12345")
        ;
}

static auto test_unsigned_int()
    -> bool {
    return test_value(0u, "0")
        && test_value(12345u, "12345")
        ;
}

static auto test_unsigned_long()
    -> bool {
    return test_value(0ul, "0")
        && test_value(12345ul, "12345")
        ;
}

static auto test_unsigned_long_long()
    -> bool {
    return test_value(0ull, "0")
        && test_value(12345ull, "12345")
        ;
}


static auto test_void_pointer()
    -> bool {
    return test_value(static_cast<void const*>(0), "0")
        && test_value(reinterpret_cast<void const*>(12345ul), "12345")
        ;
}

// ----------------------------------------------------------------------------

static auto test()
    -> bool {
    KM::streambuf sbuf;
    KM::ostream   out(&sbuf);
    teststream    tout(&sbuf);
    return out.rdbuf()  == &sbuf
        && tout.rdbuf() == &sbuf
        && test_left()
        && test_right()
        && test_string_literal()
        && test_char()
        && test_int()
        && test_long()
        && test_long_long()
        && test_unsigned_int()
        && test_unsigned_long()
        && test_unsigned_long_long()
        && test_void_pointer()
        ;
}

// ----------------------------------------------------------------------------

auto main()
    -> int
{
    return test()? EXIT_SUCCESS: EXIT_FAILURE;
}
