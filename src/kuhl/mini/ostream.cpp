// kuhl/mini/ostream.cpp                                              -*-C++-*-
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
#include <string.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

KM::ostream::ostream(KM::streambuf* sbuf)
    : KM::ios(sbuf) {
    this->init(sbuf);
}

// ----------------------------------------------------------------------------

auto KM::left(KM::ostream& out) -> KM::ostream& {
    out.setf(KM::ios::left, KM::ios::adjustfield);
    return out;
}

auto KM::right(KM::ostream& out) -> KM::ostream& {
    out.setf(KM::ios::right, KM::ios::adjustfield);
    return out;
}

auto KM::ostream::operator<< (KM::ostream& (*manip)(KM::ostream&))
    -> KM::ostream& {
    manip(*this);
    return *this;
}

// ----------------------------------------------------------------------------

auto KM::ostream::operator<< (char value)
    -> KM::ostream& {
    this->rdbuf()->sputc(value);
    return *this;
}

// ----------------------------------------------------------------------------

static auto format(KM::ostream& out, bool negative, unsigned long long value)
    -> KM::ostream& {
    char buffer[256];
    char* it(buffer + sizeof(buffer) - 1);
    *--it = 0;
    do {
        *--it = "0123456789"[value % 10];
        value /= 10;
    } while (value != 0);
    if (negative) {
        *--it = '-';
    }
    return out << it;
}

auto KM::ostream::operator<< (int value)
    -> KM::ostream& {
    return format(*this, value < 0, value < 0? -value: value);
}
auto KM::ostream::operator<< (long value)
    -> KM::ostream& {
    return format(*this, value < 0, value < 0? -value: value);
}
auto KM::ostream::operator<< (long long value)
    -> KM::ostream& {
    return format(*this, value < 0, value < 0? -value: value);
}

auto KM::ostream::operator<< (unsigned int value)
    -> KM::ostream& {
    return format(*this, false, value);
}
auto KM::ostream::operator<< (unsigned long value)
    -> KM::ostream& {
    return format(*this, false, value);
}
auto KM::ostream::operator<< (unsigned long long value)
    -> KM::ostream& {
    return format(*this, false, value);
}

auto KM::ostream::operator<< (void const volatile* value)
    -> KM::ostream& {
    return format(*this, false, reinterpret_cast<unsigned long>(value));
}

// ----------------------------------------------------------------------------

static auto get_fill(KM::ostream& out, char const* value)
    -> size_t {
    size_t width(out.width(0));
    if (width) {
        size_t len{strlen(value)};
        return width < len? 0: width - len;
    }
    return 0u;
}

auto KM::operator<< (KM::ostream& out, char const* value)
    -> KM::ostream& {
    KM::streambuf* sbuf(out.rdbuf());
    size_t fill{get_fill(out, value)};
    if (fill && (out.flags() & KM::ios::right)) {
        for (; fill; --fill) {
            sbuf->sputc(out.fill());
        }
    }
    for (; *value; ++value) {
        sbuf->sputc(*value);
    }
    if (fill) {
        for (; fill; --fill) {
            sbuf->sputc(out.fill());
        }
    }
    return out;
}
