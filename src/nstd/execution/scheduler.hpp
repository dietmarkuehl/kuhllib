// nstd/execution/scheduler.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_SCHEDULER
#define INCLUDED_NSTD_EXECUTION_SCHEDULER

#include "nstd/concepts/copy_constructible.hpp"
#include "nstd/concepts/equality_comparable.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include <concepts>

// ----------------------------------------------------------------------------
// [exec.sched]

namespace nstd::execution {
    template <typename Scheduler>
    concept scheduler
        =  ::nstd::concepts::copy_constructible<::nstd::type_traits::remove_cvref_t<Scheduler>>
        && ::nstd::concepts::equality_comparable<::nstd::type_traits::remove_cvref_t<Scheduler>>
        && requires(Scheduler&& s, ::nstd::execution::get_completion_scheduler_t<::nstd::execution::set_value_t> const& cpo) {
            { ::nstd::execution::schedule(::nstd::utility::forward<Scheduler>(s)) } -> ::nstd::execution::sender;
	    { tag_invoke(cpo, ::nstd::execution::schedule(::nstd::utility::forward<Scheduler>(s))) } -> ::std::same_as<::nstd::type_traits::remove_cvref_t<Scheduler>>;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
