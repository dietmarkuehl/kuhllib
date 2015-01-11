// nstd/iostream/ios_base.cpp                                         -*-C++-*-
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

#include "nstd/iostream/ios_base.hpp"

namespace NI = ::nstd;

// ----------------------------------------------------------------------------

NI::ios_base::failure::failure(::nstd::string const& message, ::nstd::error_code const& ec)
    : NI::system_error(ec, message) {
}

NI::ios_base::failure::failure(char const* message, ::nstd::error_code const& ec)
    : NI::system_error(ec, message) {
}

// ----------------------------------------------------------------------------

constexpr NI::ios_base::fmtflags NI::ios_base::left;
constexpr NI::ios_base::fmtflags NI::ios_base::right;
constexpr NI::ios_base::fmtflags NI::ios_base::internal;
constexpr NI::ios_base::fmtflags NI::ios_base::adjustfield;
constexpr NI::ios_base::fmtflags NI::ios_base::dec;
constexpr NI::ios_base::fmtflags NI::ios_base::hex;
constexpr NI::ios_base::fmtflags NI::ios_base::oct;
constexpr NI::ios_base::fmtflags NI::ios_base::basefield;
constexpr NI::ios_base::fmtflags NI::ios_base::fixed;
constexpr NI::ios_base::fmtflags NI::ios_base::scientific;
constexpr NI::ios_base::fmtflags NI::ios_base::floatfield;
constexpr NI::ios_base::fmtflags NI::ios_base::boolalpha;
constexpr NI::ios_base::fmtflags NI::ios_base::showbase;
constexpr NI::ios_base::fmtflags NI::ios_base::showpoint;
constexpr NI::ios_base::fmtflags NI::ios_base::showpos;
constexpr NI::ios_base::fmtflags NI::ios_base::skipws;
constexpr NI::ios_base::fmtflags NI::ios_base::unitbuf;
constexpr NI::ios_base::fmtflags NI::ios_base::uppercase;

constexpr NI::ios_base::iostate NI::ios_base::goodbit;
constexpr NI::ios_base::iostate NI::ios_base::failbit;
constexpr NI::ios_base::iostate NI::ios_base::badbit;
constexpr NI::ios_base::iostate NI::ios_base::eofbit;

constexpr NI::ios_base::openmode NI::ios_base::app;
constexpr NI::ios_base::openmode NI::ios_base::ate;
constexpr NI::ios_base::openmode NI::ios_base::binary;
constexpr NI::ios_base::openmode NI::ios_base::in;
constexpr NI::ios_base::openmode NI::ios_base::out;
constexpr NI::ios_base::openmode NI::ios_base::trunc;

// ----------------------------------------------------------------------------

NI::ios_base::ios_base() {
}

NI::ios_base::~ios_base() {
}
