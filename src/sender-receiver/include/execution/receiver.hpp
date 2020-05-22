// include/execution/receiver.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_INCLUDE_EXECUTION_RECEIVER
#define INCLUDED_INCLUDE_EXECUTION_RECEIVER

#include <execution/set_done.hpp>
#include <execution/set_error.hpp>

#include <concepts>
#include <exception>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    template <typename R, typename E = std::exception_ptr>
    concept receiver
        =  std::move_constructible<std::remove_cvref_t<R>>
        && std::constructible_from<std::remove_cvref_t<R>, R>
        && requires(std::remove_cvref_t<R>&& r, E&& e)
           {
               { execution::set_done(std::move(r)) } noexcept;
               { execution::set_error(std::move(r), std::forward<E>(e)) } noexcept;
           }
        ;
    
}

// ----------------------------------------------------------------------------

#endif
