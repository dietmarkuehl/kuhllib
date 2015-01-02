// kuhl/mini/iomanip.cpp                                              -*-C++-*-
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

#include "kuhl/mini/iomanip.hpp"
#include "kuhl/mini/ostream.hpp"

namespace KM = kuhl::mini;

// ----------------------------------------------------------------------------

KM::setter_fill::setter_fill(char fill)
    : fill(fill) {
}

auto KM::setfill(char fill)
    -> KM::setter_fill {
    return KM::setter_fill(fill);
}

auto KM::operator<< (KM::ostream& out, KM::setter_fill const& fill)
    -> KM::ostream& {
    out.fill(fill.fill);
    return out;
}

// ----------------------------------------------------------------------------

KM::setter_width::setter_width(int width)
    : width(width) {
}

auto KM::setw(int width)
    -> KM::setter_width {
    return KM::setter_width(width);
}

auto KM::operator<< (KM::ostream& out, KM::setter_width const& width)
    -> KM::ostream& {
    out.width(width.width);
    return out;
}
