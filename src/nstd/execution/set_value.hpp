// nstd/execution/set_value.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_SET_VALUE
#define INCLUDED_NSTD_EXECUTION_SET_VALUE

#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace hidden_names
    {
        void set_value();

        template <typename Receiver, typename... Args>
        concept has_member_set_value
            = requires(Receiver receiver, Args...args){
                ::nstd::utility::forward<Receiver>(receiver).set_value(::nstd::utility::forward<Args>(args)...);
            }
            ;

        template <typename Receiver, typename... Args>
        concept has_non_member_set_value
            =  !has_member_set_value<Receiver, Args...>
            && requires(Receiver&& receiver, Args&&... args){
                set_value(::nstd::utility::forward<Receiver>(receiver), ::nstd::utility::forward<Args>(args)...);
            }
            ;
    }
    namespace execution::inline customization_points
    {
        inline constexpr struct {
            template <typename Receiver, typename... Args>
                requires ::nstd::hidden_names::has_member_set_value<Receiver, Args...>
            auto operator()(Receiver&& receiver, Args&&... args) const
                noexcept(noexcept(::nstd::utility::forward<Receiver>(receiver).set_value(::nstd::utility::forward<Args>(args)...)))
            {
                return ::nstd::utility::forward<Receiver>(receiver).set_value(::nstd::utility::forward<Args>(args)...);
            }

            template <typename Receiver, typename... Args>
                requires ::nstd::hidden_names::has_non_member_set_value<Receiver, Args...>
            auto operator()(Receiver&& receiver, Args&&... args) const
                noexcept(noexcept(set_value(::nstd::utility::forward<Receiver>(receiver), ::nstd::utility::forward<Args>(args)...)))
            {
                return set_value(::nstd::utility::forward<Receiver>(receiver), ::nstd::utility::forward<Args>(args)...);
            }

            //template <typename... Args>
            //auto operator()(Args...) = delete;
        } set_value;
    }
}

// ----------------------------------------------------------------------------

#endif
