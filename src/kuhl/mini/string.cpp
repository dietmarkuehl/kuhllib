// kuhl/mini/string.cpp                                               -*-C++-*-
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
#include <string.h>
#include <stddef.h>

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

KM::string::string()
    : value(new char[1]) {
    *value = 0;
}

KM::string::string(char const* str)
    : value(new char[strlen(str) + 1]) {
    strcpy(this->value, str);
}

KM::string::string(KM::string const& str)
    : value(new char[strlen(str.value) + 1]) {
    strcpy(this->value, str.value);
}

// ----------------------------------------------------------------------------

KM::string::~string() {
    delete[] this->value;
}

// ----------------------------------------------------------------------------

auto KM::string::c_str() const
    -> char const* {
    return this->value;
}

// ----------------------------------------------------------------------------

auto KM::string::operator+= (KM::string const& other)
    -> KM::string& {
    size_t s0(strlen(this->value));
    char* tmp(new char[s0 + strlen(other.value) + 1]);
    strcpy(tmp, this->value);
    strcpy(tmp + s0, other.value);
    delete[] this->value;
    this->value = tmp;
    return *this;
}

auto KM::operator+ (KM::string const& s0, KM::string const& s1)
    -> KM::string {
    return KM::string(s0) += s1;
}

// ----------------------------------------------------------------------------

auto KM::operator== (KM::string const& s0, KM::string const& s1) -> bool {
    return strcmp(s0.c_str(), s1.c_str()) == 0;
}
auto KM::operator!= (KM::string const& s0, KM::string const& s1) -> bool {
    return !(s0 == s1);
}

auto KM::operator== (char const* s0, KM::string const& s1) -> bool {
    return strcmp(s0, s1.c_str()) == 0;
}
auto KM::operator!= (char const* s0, KM::string const& s1) -> bool {
    return !(s0 == s1);
}

auto KM::operator== (KM::string const& s0, char const* s1) -> bool {
    return strcmp(s0.c_str(), s1) == 0;
}
auto KM::operator!= (KM::string const& s0, char const* s1) -> bool {
    return !(s0 == s1);
}

// ----------------------------------------------------------------------------

auto KM::operator<< (KM::ostream& out, KM::string const& value)
    -> KM::ostream& {
    return out << value.c_str();
}
