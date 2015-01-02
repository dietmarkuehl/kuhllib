// kuhl/mini/ios.hpp                                                  -*-C++-*-
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

#ifndef INCLUDED_KUHL_MINI_IOS
#define INCLUDED_KUHL_MINI_IOS

// ----------------------------------------------------------------------------

namespace kuhl {
    namespace mini {
        class streambuf;
        class ios;
    }
}

// ----------------------------------------------------------------------------

class kuhl::mini::ios {
public:
    enum fmtflags {
        left  = 0x01,
        right = 0x02,
        dec   = 0x04,
        oct   = 0x08,
        hex   = 0x0C,
        basefield = dec | oct | hex,
        adjustfield = left | right
    };

private:
    kuhl::mini::streambuf* sbuf;
    int      width_;
    char     fill_;
    fmtflags flags_;

protected:
    auto init(kuhl::mini::streambuf*) -> void;
public:
    ios(kuhl::mini::streambuf* sbuf);
    auto rdbuf() -> kuhl::mini::streambuf* { return this-> sbuf; }

    auto width() const -> int;
    auto width(int) -> int;
    auto fill() const -> char;
    auto fill(char) -> char;
    auto flags() const -> fmtflags;
    auto setf(fmtflags, fmtflags) -> fmtflags;
};

// ----------------------------------------------------------------------------

#endif
