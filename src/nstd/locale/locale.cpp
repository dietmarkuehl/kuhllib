// nstd/locale/locale.cpp                                             -*-C++-*-
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

#include "nstd/locale/locale.hpp"

namespace NL = ::nstd;

// ----------------------------------------------------------------------------

constexpr NL::locale::category NL::locale::none;
constexpr NL::locale::category NL::locale::collate;
constexpr NL::locale::category NL::locale::ctype;
constexpr NL::locale::category NL::locale::monetary;
constexpr NL::locale::category NL::locale::numeric;
constexpr NL::locale::category NL::locale::time;
constexpr NL::locale::category NL::locale::messages;
constexpr NL::locale::category NL::locale::all;

// ----------------------------------------------------------------------------

NL::locale::facet::facet(NL::size_t) {
}

NL::locale::facet::~facet() {
}

// ----------------------------------------------------------------------------

NL::locale::id::id() {
}

// ----------------------------------------------------------------------------

NL::locale::locale() noexcept(true) {
}

NL::locale::~locale() {
}

// ----------------------------------------------------------------------------

auto NL::locale::operator== (NL::locale const& other) const
    -> bool {
    return true;
}

auto NL::locale::operator!= (NL::locale const& other) const
    -> bool {
    return !this->operator==(other);
}

// ----------------------------------------------------------------------------

auto NL::locale::classic()
    -> NL::locale const& {
    static NL::locale rc; //-dk:TODO use a statically initialized version
    return rc;
}

