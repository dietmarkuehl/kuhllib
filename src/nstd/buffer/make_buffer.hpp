// nstd/buffer/make_buffer.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_BUFFER_MAKE_BUFFER
#define INCLUDED_NSTD_BUFFER_MAKE_BUFFER

#include "nstd/memory/addressof.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names {
    template <typename B, typename C>
    auto make_buffer(C&& c) -> B;
}

// ----------------------------------------------------------------------------

template <typename B, typename C>
auto nstd::net::hidden_names::make_buffer(C&& c) -> B
{
    return c.begin() == c.end()
        ? B()
        : B(::nstd::memory::addressof(*c.begin()),
            sizeof(*c.begin()) * (c.end() - c.begin()));
}

// ----------------------------------------------------------------------------

#endif
