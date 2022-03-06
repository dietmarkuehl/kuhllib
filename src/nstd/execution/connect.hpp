// nstd/execution/connect.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_CONNECT
#define INCLUDED_NSTD_EXECUTION_CONNECT

#include "nstd/execution/completion_signatures_of_t.hpp"
#include "nstd/execution/env_of_t.hpp"
#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/receiver_of.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------
// [exec.connect]

namespace nstd::execution {
    namespace hidden_names::connect {
        struct cpo {
            template <::nstd::execution::sender Sender, ::nstd::execution::receiver Receiver>
                requires ::nstd::execution::receiver_of<
                        Receiver,
                        ::nstd::execution::completion_signatures_of_t<Sender, ::nstd::execution::env_of_t<Receiver>>
                        >
                    && requires(Sender&& sender, Receiver&& receiver) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::hidden_names::connect::cpo>(),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Receiver>(receiver))
                    }
                    -> nstd::execution::operation_state;
                }
            constexpr auto operator()(Sender&& sender, Receiver&& receiver) const
                noexcept(noexcept(::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::hidden_names::connect::cpo>(),
                                                     ::nstd::utility::forward<Sender>(sender),
                                                     ::nstd::utility::forward<Receiver>(receiver))))
            {
                return ::nstd::tag_invoke(*this, //connect_t
                                         ::nstd::utility::forward<Sender>(sender),
                                         ::nstd::utility::forward<Receiver>(receiver));
            }
        };
    }
    inline namespace customization_points {
        using connect_t = ::nstd::execution::hidden_names::connect::cpo;
        inline constexpr connect_t  connect{};
    }
}

// ----------------------------------------------------------------------------

#endif