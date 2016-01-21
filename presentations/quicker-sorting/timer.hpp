// timer.hpp                                                          -*-C++-*-
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

#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

#include <chrono>
#include <iosfwd>

// ----------------------------------------------------------------------------

namespace utility {
    class timer;
    class timer_duration;
    std::ostream& operator<< (std::ostream& out, timer_duration const&);
}

// ----------------------------------------------------------------------------

class utility::timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> d_start;
public:
    timer();
    void start();
    timer_duration stop() const;
};

// ----------------------------------------------------------------------------

class utility::timer_duration
{
    std::chrono::microseconds d_us;
public:
    explicit timer_duration(std::chrono::microseconds);
    std::ostream& print(std::ostream& out) const;
};

// ----------------------------------------------------------------------------

#endif
