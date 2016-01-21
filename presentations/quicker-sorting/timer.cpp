// timer.cpp                                                          -*-C++-*-
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

#include "timer.hpp"
#include <ostream>

// ----------------------------------------------------------------------------

utility::timer::timer()
    : d_start() {
    this->start();
}

// ----------------------------------------------------------------------------

void utility::timer::start() {
    this->d_start = std::chrono::high_resolution_clock::now();
}

// ----------------------------------------------------------------------------

utility::timer_duration utility::timer::stop() const
{
    auto end = std::chrono::high_resolution_clock::now();
    return timer_duration(std::chrono::duration_cast<std::chrono::microseconds>(end - this->d_start));
}

// ----------------------------------------------------------------------------

utility::timer_duration::timer_duration(std::chrono::microseconds us)
    : d_us(us) {
}

// ----------------------------------------------------------------------------

std::ostream& utility::timer_duration::print(std::ostream& out) const {
    return out << this->d_us.count() << "us";
}

std::ostream& utility::operator<< (std::ostream& out, utility::timer_duration const& duration) {
    return duration.print(out);
}
