// include/execution/receiver_of.hpp                                  -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_EXECUTION_RECEIVER_OF
#define INCLUDED_EXECUTION_RECEIVER_OF

#include <execution/receiver.hpp>
#include <execution/set_value.hpp>

#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    template<typename R, typename... A>
    concept receiver_of
        =  receiver<R>
        && requires(std::remove_cvref_t<R>&& r, A&&... a)
           {
               execution::set_value(std::move(r), std::forward<A>(a)...);
           }
        ;

    template<typename R, typename... A>
    inline constexpr bool is_nothrow_receiver_of_v =
        receiver_of<R, A...> &&
        std::is_nothrow_invocable_v<decltype(set_value), R, A...>;
}

// ----------------------------------------------------------------------------

#endif
