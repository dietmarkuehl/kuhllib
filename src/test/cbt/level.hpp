// cbt/level.hpp                                                      -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CBT_LEVEL
#define INCLUDED_CBT_LEVEL

#include <iomanip>
#include <ostream>

// ----------------------------------------------------------------------------

namespace cbt {
    enum class level {
        info,
        warning,
        error
    };
    inline std::ostream& operator<< (std::ostream& out, level l) {
        std::streamsize width = out.width(0);
        switch (l) {
        case cbt::level::info:
            out << "\x1b[32m" << std::setw(width) << "info" << "\x1b[0m";
            break;
        case cbt::level::warning:
            out << "\x1b[33m" << std::setw(width) << "warning" << "\x1b[0m";
            break;
        case cbt::level::error:
            out << "\x1b[31m" << std::setw(width) << "error" << "\x1b[0m";
            break;
        }
        return out << std::flush;;
    }
}

// ----------------------------------------------------------------------------

#endif
