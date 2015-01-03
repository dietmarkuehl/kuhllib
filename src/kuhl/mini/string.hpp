// kuhl/mini/string.hpp                                               -*-C++-*-
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

#ifndef INCLUDED_KUHL_MINI_STRING
#define INCLUDED_KUHL_MINI_STRING

#include "kuhl/mini/ostream.hpp"

// ----------------------------------------------------------------------------

namespace kuhl {
    namespace mini {
        class ostream;
        class string;

        auto operator+ (string const&, string const&) -> string;

        auto operator== (string const&, string const&) -> bool;
        auto operator!= (string const&, string const&) -> bool;
        auto operator== (char const*, string const&) -> bool;
        auto operator!= (char const*, string const&) -> bool;
        auto operator== (string const&, char const*) -> bool;
        auto operator!= (string const&, char const*) -> bool;
        
        auto operator<< (ostream&, string const&) -> ostream&;
    }
}

// ----------------------------------------------------------------------------

class kuhl::mini::string {
    char* value;
public:
    string();
    explicit string(char const*);
    string(string const&);
    ~string();
    void operator= (string const&) = delete;

    auto operator+=(string const&) -> string&;

    auto c_str() const -> char const*;
};

// ----------------------------------------------------------------------------

#endif