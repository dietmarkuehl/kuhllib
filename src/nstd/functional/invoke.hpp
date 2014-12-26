// nstd/functional/invoke.hpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_INVOKE
#define INCLUDED_NSTD_FUNCTIONAL_INVOKE

#include "nstd/functional/mem_fn.hpp"
#include "nstd/type_traits/result_of.hpp"
#include "nstd/type_traits/decay.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/is_member_pointer.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        template <typename Fun, typename... Args>
        auto invoke(Fun&& fun, Args&&...args)
            -> typename ::nstd::type_traits::enable_if<!::nstd::type_traits::is_member_pointer< ::nstd::type_traits::decay_t<Fun>>::value,
                                                       ::nstd::type_traits::result_of_t<Fun&&(Args&&...)>>::type {
            return ::nstd::utility::forward<Fun>(fun)(::nstd::utility::forward<Args>(args)...);
        }
#if 0
        template <typename Fun, typename... Args>
        auto invoke(Fun&& fun, Args&&...args)
            -> typename ::nstd::type_traits::enable_if< ::nstd::type_traits::is_member_pointer< ::nstd::type_traits::decay_t<Fun>>::value,
                                                       ::nstd::type_traits::result_of_t<Fun(Args...)>>::type {
            return ::nstd::functional::mem_fn(::nstd::utility::forward<Fun>(fun))(::nstd::utility::forward<Args>(args)...);
        }
#endif
    }

}

// ----------------------------------------------------------------------------

#endif
