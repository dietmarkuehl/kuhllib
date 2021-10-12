// nstd/execution/timeout.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_TIMEOUT
#define INCLUDED_NSTD_EXECUTION_TIMEOUT

#include "nstd/execution/receiver.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct timeout_t
    {
        template <::nstd::execution::receiver>
        struct receiver;
        template <::nstd::execution::receiver>
        struct state;
        template <::nstd::execution::sender, ::nstd::execution::sender>
        struct sender;

        template <::nstd::execution::sender Sender,
                  ::nstd::execution::scheduler Scheduler,
                  typename Time>
        auto operator()(Sender&&, Scheduler&&, Time&&) const
            -> sender<Sender, Sender>;
    } timeout;
}

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender, ::nstd::execution::sender Timer>
struct nstd::execution::timeout_t::sender
{
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender,
          ::nstd::execution::scheduler Scheduler,
          typename Time>
auto ::nstd::execution::timeout_t::operator()(Sender&&    ,
                                              Scheduler&& ,
                                              Time&&      ) const
    -> sender<Sender, Sender>
{
    return {};
}

// ----------------------------------------------------------------------------

#endif
