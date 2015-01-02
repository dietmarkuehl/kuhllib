// kuhl/mini/ostream.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_KUHL_MINI_OSTREAM
#define INCLUDED_KUHL_MINI_OSTREAM

#include "kuhl/mini/ios.hpp"

// ----------------------------------------------------------------------------

namespace kuhl {
    namespace mini {
        class streambuf;
        class ostream;

        ostream& right(ostream&);
        ostream& left(ostream&);

        ostream& operator<< (ostream&, char const*);
    }
}

// ----------------------------------------------------------------------------

class kuhl::mini::ostream
    : public virtual kuhl::mini::ios {
public:
    ostream(kuhl::mini::streambuf* sbuf);
    ostream(ostream const&) = delete;
    void operator=(ostream const&) = delete;

    auto operator<< (ostream& (*)(ostream&)) -> ostream&; //-dk:TODO late return type vs. gcc
    auto operator<< (char) -> ostream&;
    auto operator<< (int) -> ostream&;
    auto operator<< (long) -> ostream&;
    auto operator<< (long long) -> ostream&;
    auto operator<< (unsigned int) -> ostream&;
    auto operator<< (unsigned long) -> ostream&;
    auto operator<< (unsigned long long) -> ostream&;

    auto operator<< (void const volatile*) -> ostream&;
};

// ----------------------------------------------------------------------------

#endif
