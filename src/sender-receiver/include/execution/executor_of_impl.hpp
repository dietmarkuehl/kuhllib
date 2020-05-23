// include/execution/executor_of_impl.hpp                             -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_EXECUTOR_OF_IMPL
#define INCLUDED_EXECUTION_EXECUTOR_OF_IMPL

#include <execution/execute.hpp>

#include <concepts.hpp>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cxxrt::execution::detail
{
    template <typename E, typename F>
    concept executor_of_impl
        =  std::invocable<std::remove_cvref_t<F>&>
        && std::constructible_from<std::remove_cvref_t<F>, F>
        && std::move_constructible<std::remove_cvref_t<F>>
        && std::copy_constructible<E>
        && std::is_nothrow_copy_constructible_v<E>
        && std::equality_comparable<E>
        && requires(const E& e, F&& f) {
               execution::execute(e, (F&&)f);
           }
        ;
}

// ----------------------------------------------------------------------------

#endif
