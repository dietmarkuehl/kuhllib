// nstd/stop_token/never_stop_token.hpp                               -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_NEVER_STOP_TOKEN
#define INCLUDED_NSTD_STOP_TOKEN_NEVER_STOP_TOKEN

#include "nstd/concepts/invocable.hpp"
#include <concepts>

// ----------------------------------------------------------------------------

namespace nstd::stop_token_ns {
    class never_stop_token
    {
    public:
        template <::nstd::concepts::invocable C>
        struct callback_type{
            template<typename I>
            constexpr explicit callback_type(never_stop_token, I&&) noexcept {}
        };

        constexpr auto operator== (never_stop_token const&) const { return true; }

        static constexpr auto stop_requested() noexcept -> bool { return false; }
        static constexpr auto stop_possible() noexcept -> bool { return false; }
    };
}

// ----------------------------------------------------------------------------

#endif
