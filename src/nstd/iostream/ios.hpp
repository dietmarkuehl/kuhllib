// nstd/iostream/ios.hpp                                              -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_IOSTREAM_IOS
#define INCLUDED_NSTD_IOSTREAM_IOS

#include "nstd/iostream/ios_fwd.hpp"
#include "nstd/iostream/streambuf_fwd.hpp"
#include "nstd/iostream/ios_base.hpp"
#include "nstd/iostream/io_types.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    class ios_base;

    ::nstd::ios_base& boolalpha(::nstd::ios_base&);
    ::nstd::ios_base& noboolalpha(::nstd::ios_base&);
    ::nstd::ios_base& showbase(::nstd::ios_base&);
    ::nstd::ios_base& noshowbase(::nstd::ios_base&);
    ::nstd::ios_base& showpoint(::nstd::ios_base&);
    ::nstd::ios_base& noshowpoint(::nstd::ios_base&);
    ::nstd::ios_base& showpos(::nstd::ios_base&);
    ::nstd::ios_base& noshowpos(::nstd::ios_base&);
    ::nstd::ios_base& skipws(::nstd::ios_base&);
    ::nstd::ios_base& noskipws(::nstd::ios_base&);
    ::nstd::ios_base& uppercase(::nstd::ios_base&);
    ::nstd::ios_base& nouppercase(::nstd::ios_base&);
    ::nstd::ios_base& unitbuf(::nstd::ios_base&);
    ::nstd::ios_base& nounitbuf(::nstd::ios_base&);

    ::nstd::ios_base& internal(::nstd::ios_base&);
    ::nstd::ios_base& left(::nstd::ios_base&);
    ::nstd::ios_base& right(::nstd::ios_base&);

    ::nstd::ios_base& dec(::nstd::ios_base&);
    ::nstd::ios_base& hex(::nstd::ios_base&);
    ::nstd::ios_base& oct(::nstd::ios_base&);

    ::nstd::ios_base& fixed(::nstd::ios_base&);
    ::nstd::ios_base& scientific(::nstd::ios_base&);
    ::nstd::ios_base& hexfloat(::nstd::ios_base&);
    ::nstd::ios_base& defaultfloat(::nstd::ios_base&);
}

// ----------------------------------------------------------------------------

template <typename cT, typename Traits>
class nstd::basic_ios {
public:
    basic_ios(nstd::basic_streambuf<cT, Traits>*);
};

// ----------------------------------------------------------------------------

//-dk:TODO extern template class nstd::iostream::basic_ios<char>;
//-dk:TODO extern template class nstd::iostream::basic_ios<wchar_t>;

// ----------------------------------------------------------------------------

template <typename cT, typename Traits>
nstd::basic_ios<cT, Traits>::basic_ios(nstd::basic_streambuf<cT, Traits>*) {
}

// ----------------------------------------------------------------------------

#endif
