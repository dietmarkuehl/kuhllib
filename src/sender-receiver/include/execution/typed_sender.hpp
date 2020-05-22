// include/execution/typed_sender.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_TYPED_SENDER
#define INCLUDED_EXECUTION_TYPED_SENDER

#include <execution/has_sender_types.hpp>
#include <execution/sender.hpp>
#include <execution/sender_traits.hpp>

#include <type_traits>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    template<typename S>
    concept typed_sender
        =  sender<S>
        && detail::has_sender_types<sender_traits<std::remove_cvref_t<S>>>
        ;
}

// ----------------------------------------------------------------------------

#endif
