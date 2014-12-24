// nstd/functional/mem_fn.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_MEM_FN
#define INCLUDED_NSTD_FUNCTIONAL_MEM_FN

#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        namespace detail {
            template <typename R, typename T>
            class mem_fn {
                R T::*mem_fun;
            public:
                explicit mem_fn(R T::*mem_fun): mem_fun(mem_fun) {}
                template <typename Object, typename... Args>
                auto operator()(Object&& object, Args&&... args)
                    -> decltype((object.*(this->mem_fun))(nstd::utility::forward<Args>(args)...)) {
                    return (object.*(this->mem_fun))(nstd::utility::forward<Args>(args)...);
                }
                bool operator==(mem_fn const& other) const {
                    return this->mem_fun == other.mem_fun;
                }
                bool operator!=(mem_fn const& other) const {
                    return !(*this == other);
                }
            };
        }
        template <typename R, typename T>
        nstd::functional::detail::mem_fn<R, T> mem_fn(R T::*mem_fun) {
            return nstd::functional::detail::mem_fn<R, T>(mem_fun);
        }
    }

}

// ----------------------------------------------------------------------------

#endif
