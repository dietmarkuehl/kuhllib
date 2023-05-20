// nstd/stop_token/stoppable_token_for.hpp                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_STOPPABLE_TOKEN_FOR
#define INCLUDED_NSTD_STOP_TOKEN_STOPPABLE_TOKEN_FOR

#include "nstd/stop_token/stoppable_token.hpp"
#include "nstd/concepts/invocable.hpp"
#include "nstd/concepts/constructible_from.hpp"
#include <concepts>

// ----------------------------------------------------------------------------

namespace nstd::stop_token_ns {
    template<typename Token, typename Callback, typename Initializer = Callback>
    concept stoppable_token_for
        =  ::nstd::stop_token_ns::stoppable_token<Token>
        && ::nstd::concepts::invocable<Callback>
        && requires {
            typename Token::template callback_type<Callback>;
        }
        && ::nstd::concepts::constructible_from<Callback, Initializer>
        && ::nstd::concepts::constructible_from<typename Token::template callback_type<Callback>, Token, Initializer>
        && ::nstd::concepts::constructible_from<typename Token::template callback_type<Callback>, Token&, Initializer>
        && ::nstd::concepts::constructible_from<typename Token::template callback_type<Callback>, Token const, Initializer>
        && ::nstd::concepts::constructible_from<typename Token::template callback_type<Callback>, Token const&, Initializer>
        ;
}

// ----------------------------------------------------------------------------

#endif
