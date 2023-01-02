// nstd/execution/get_stop_token.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_GET_STOP_TOKEN
#define INCLUDED_NSTD_EXECUTION_GET_STOP_TOKEN

#include "nstd/functional/tag_invoke.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/stop_token/stoppable_token.hpp"
#include "nstd/stop_token/never_stop_token.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/utility/as_const.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::get_stop_token {
    struct cpo {
        template <typename Env>
            requires requires(cpo const& tag, Env&& env) {
                { ::nstd::tag_invoke(tag, ::nstd::utility::as_const(env)) } noexcept
                    -> nstd::stop_token_ns::stoppable_token;
            }
        constexpr auto operator()(Env&& env) const noexcept {
            return ::nstd::tag_invoke(*this, ::nstd::utility::as_const(env));
        }
        template <typename Env>
        constexpr auto operator()(Env const&) const noexcept
        {
            return ::nstd::stop_token_ns::never_stop_token();
        }
    };
}

namespace nstd::execution::inline customization_points {
    using get_stop_token_t = ::nstd::hidden_names::get_stop_token::cpo;
    inline constexpr get_stop_token_t get_stop_token{};
}

// ----------------------------------------------------------------------------

#endif
