// nstd/execution/sender.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_SENDER
#define INCLUDED_NSTD_EXECUTION_SENDER

#include "nstd/hidden_names/sender_base.hpp"
#include "nstd/execution/sender_traits.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <concepts>

// ----------------------------------------------------------------------------
// [exec.snd]

namespace nstd::execution {

    template <typename Sender, typename Env = ::nstd::hidden_names::exec_envs::no_env>
    concept sender
        =  ::nstd::hidden_names::sender_base<Sender, Env>
        && ::nstd::hidden_names::sender_base<Sender, ::nstd::hidden_names::exec_envs::no_env>
        && ::std::move_constructible<::nstd::type_traits::remove_cvref_t<Sender>>
        ;

    template <::nstd::execution::sender Sender, typename Fun>
    auto operator|(Sender&& sender, Fun&& fun)
    {
        return ::nstd::utility::forward<Fun>(fun)(::nstd::utility::forward<Sender>(sender));
    }
}

// ----------------------------------------------------------------------------

#endif
