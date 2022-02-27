// nstd/execution/get_completion_scheduler.hpp                        -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_GET_COMPLETION_SCHEDULER
#define INCLUDED_NSTD_EXECUTION_GET_COMPLETION_SCHEDULER

#include "nstd/concepts/same_as.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/utility/as_const.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::get_completion_scheduler {
    template <typename CPO>
        requires ::nstd::concepts::same_as<::nstd::execution::set_value_t, CPO>
              || ::nstd::concepts::same_as<::nstd::execution::set_error_t, CPO>
              || ::nstd::concepts::same_as<::nstd::execution::set_stopped_t, CPO>
    struct cpo {
        template <::nstd::execution::sender Sender>
            requires requires(cpo<CPO> const& gcs, Sender&& sender) {
                { ::nstd::tag_invoke(gcs, ::nstd::utility::as_const(sender)) } noexcept -> ::nstd::execution::scheduler;
            }
        auto operator()(Sender&& sender) const noexcept
        {
            return ::nstd::tag_invoke(*this, ::nstd::utility::as_const(sender));
        }
    };
}

namespace nstd::execution {
    template <typename CPO>
    using get_completion_scheduler_t = ::nstd::hidden_names::get_completion_scheduler::cpo<CPO>;
    
    template <typename CPO>
        requires ::nstd::concepts::same_as<::nstd::execution::set_value_t, CPO>
              || ::nstd::concepts::same_as<::nstd::execution::set_error_t, CPO>
              || ::nstd::concepts::same_as<::nstd::execution::set_stopped_t, CPO>
    inline constexpr get_completion_scheduler_t<CPO> get_completion_scheduler;
}

// ----------------------------------------------------------------------------

#endif
