// nstd/execution/run.hpp                                             -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_RUN
#define INCLUDED_NSTD_EXECUTION_RUN

#include "nstd/execution/sender.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/utility/forward.hpp"
#include <atomic>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names
{
    struct run_receiver
    {
        ::std::atomic<bool>* done;
        template <typename... Args>
        friend auto tag_invoke(::nstd::execution::set_value_t, run_receiver&& r, Args&&...)
            noexcept -> void
        {
            *r.done = true;
        }
        template <typename Error>
        friend auto tag_invoke(::nstd::execution::set_error_t, run_receiver&& r, Error&&)
            noexcept -> void
        {
            *r.done = true;
        }
        friend auto tag_invoke(::nstd::execution::set_stopped_t, run_receiver&& r)
            noexcept -> void
        {
            *r.done = true;
        }
    };
}

namespace nstd::execution {
    template<typename Context, ::nstd::execution::sender Sender>
    auto run(Context& context, Sender&& sender)
    {
        ::std::atomic<bool> done(false);
        auto state(::nstd::execution::connect(::nstd::utility::forward<Sender>(sender),
                                              ::nstd::hidden_names::run_receiver{&done}));
        ::nstd::execution::start(state);
        while (!done  && 1u == context.run_one()) {
        }
        if (!done) {
            //-dk:TODO need to cancel the operation before destroying the state
        }
    }
}

// ----------------------------------------------------------------------------

#endif
