// nstd/functional/less.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_PLUS
#define INCLUDED_NSTD_FUNCTIONAL_PLUS

#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace functional {
        template <typename = void> class less;
        template <> class less<void>;
    }
}

// ----------------------------------------------------------------------------

template <typename T>
class nstd::functional::less {
public:
    constexpr bool operator()(T const& a0, T const& a1) const {
        return a0 < a1;
    }
};

// ----------------------------------------------------------------------------

template <>
class nstd::functional::less<void> {
public:
    template <typename T0, typename T1>
    constexpr auto operator()(T0&& a0, T1&& a1) const {
        return nstd::utility::forward<T0>(a0) < nstd::utility::forward<T1>(a1);
    }
};

// ----------------------------------------------------------------------------

#endif
