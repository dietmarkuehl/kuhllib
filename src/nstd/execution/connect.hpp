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

#include "nstd/execution/operation_state.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace hidden_names
    {
        template <typename Sender, typename Receiver>
        concept has_member_connect
            = requires(Sender&& sender, Receiver&& receiver){
                sender.connect(::nstd::utility::forward<Receiver>(receiver));
                requires ::nstd::execution::operation_state<decltype(sender.connect(receiver))>;
            }
            ;

        void connect();
        template <typename Sender, typename Receiver>
        concept has_non_member_connect
            =  !has_member_connect<Sender, Receiver>
            && requires(Sender&& sender, Receiver&& receiver){
                connect(sender, ::nstd::utility::forward<Receiver>(receiver));
                requires ::nstd::execution::operation_state<decltype(connect(sender, receiver))>;
            }
            ;
    }
    namespace execution::inline customization_points
    {
        inline constexpr struct connect_cpo {
            template <::nstd::execution::sender Sender,
                      ::nstd::execution::receiver Receiver>
                requires ::nstd::hidden_names::has_member_connect<Sender, Receiver>
            auto operator()(Sender&& sender, Receiver&& receiver) const
                noexcept(noexcept(sender.connect(::nstd::utility::forward<Receiver>(receiver))))
            {
                return sender.connect(::nstd::utility::forward<Receiver>(receiver));
            }
            template <::nstd::execution::sender Sender,
                      ::nstd::execution::receiver Receiver>
                requires ::nstd::hidden_names::has_non_member_connect<Sender, Receiver>
            auto operator()(Sender&& sender, Receiver&& receiver) const
                noexcept(noexcept(connect(sender, ::nstd::utility::forward<Receiver>(receiver))))
            {
                return connect(sender, ::nstd::utility::forward<Receiver>(receiver));
            }
            //-dk:TODO connect for as-operation{s, r}
            //-dk:TODO 
            // auto operator()(auto&&...) const = delete;
        } connect;
    }
}

// ----------------------------------------------------------------------------

#endif
