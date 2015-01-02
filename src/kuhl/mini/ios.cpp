// kuhl/mini/ios.cpp                                                  -*-C++-*-
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

#include "kuhl/mini/ios.hpp"

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

KM::ios::ios(KM::streambuf* sbuf)
    : sbuf(sbuf)
    , width_()
    , fill_(' ')
    , flags_(KM::ios::dec) {
}

// ----------------------------------------------------------------------------

auto KM::ios::init(KM::streambuf* sbuf)
    -> void {
    this->sbuf = sbuf;
}

// ----------------------------------------------------------------------------

auto KM::ios::width() const
    -> int {
    return this->width_;
}

auto KM::ios::width(int value)
    -> int {
    int rc(this->width_);
    this->width_ = value;
    return rc;
}

// ----------------------------------------------------------------------------

auto KM::ios::fill() const
    -> char {
    return this->fill_;
}

auto KM::ios::fill(char value)
    -> char {
    char rc(this->fill_);
    this->fill_ = value;
    return rc;
}

// ----------------------------------------------------------------------------

auto KM::ios::flags() const
    -> KM::ios::fmtflags {
    return this->flags_;
}

auto KM::ios::setf(KM::ios::fmtflags flags, KM::ios::fmtflags mask)
    -> KM::ios::fmtflags {
    KM::ios::fmtflags rc(this->flags_);
    this->flags_ = KM::ios::fmtflags((this->flags_ & ~mask) | flags);
    return rc;
}
