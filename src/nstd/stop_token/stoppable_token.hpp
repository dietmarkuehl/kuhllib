// nstd/stop_token/stoppable_token.hpp                                -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_STOPPABLE_TOKEN
#define INCLUDED_NSTD_STOP_TOKEN_STOPPABLE_TOKEN

#include "nstd/concepts/boolean_testable.hpp"
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace hidden_names {
        template <template <typename...> class>
        class check_type_alias_exists {
        };
    }

    template <typename StopToken>
    concept stoppable_token
        =  ::std::copy_constructible<StopToken>
        && ::std::move_constructible<StopToken>
        && ::std::is_nothrow_copy_constructible_v<StopToken>
        && ::std::is_nothrow_move_constructible_v<StopToken>
        && ::std::equality_comparable<StopToken>
        && requires(StopToken const& st) {
                { st.stop_requested() } noexcept -> ::nstd::concepts::boolean_testable;
                { st.stop_possible() } noexcept -> ::nstd::concepts::boolean_testable;
                typename ::nstd::hidden_names::check_type_alias_exists<StopToken::template callback_type>;
            }
        ;
}

// ----------------------------------------------------------------------------

#endif
