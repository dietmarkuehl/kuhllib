// cbt/movable.hpp                                                    -*-C++-*-
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

#ifndef INCLUDED_CBT_MOVABLE
#define INCLUDED_CBT_MOVABLE

#include "cbt/level.hpp"
#include <ostream>
#include <string>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {
    template <typename T>
    bool Movable(std::ostream& out, std::string_view name, T&&);
}

// ----------------------------------------------------------------------------

template <typename T>
bool cbt::Movable(std::ostream& out, std::string_view name, T&&) {
    if (!std::is_object_v<T>) {
        out << cbt::level::error << " " << name << " isn't an object type\n";
        return false;
    }
    // MoveConstructible<T> &&
    // Assignable<T&, T> &&
    // Swappable<T>;
    return true;
}

// ----------------------------------------------------------------------------

#endif
