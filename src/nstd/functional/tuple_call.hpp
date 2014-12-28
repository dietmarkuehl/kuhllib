// nstd/functional/tuple_call.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_TUPLE_CALL
#define INCLUDED_NSTD_FUNCTIONAL_TUPLE_CALL

#include "nstd/utility/forward.hpp"
#include "nstd/utility/integer_sequence.hpp"
#include "nstd/type_traits/decay.hpp"
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        namespace detail {
            template <typename>
            struct tuple_call;
            template <std::size_t... I>
            struct tuple_call<nstd::utility::index_sequence<I...> > {
                template <typename Fun, typename Args>
                static auto call(Fun&& fun, Args&& args)
                    -> decltype(nstd::utility::forward<Fun>(fun)(std::get<I>(args)...)) {
                    return nstd::utility::forward<Fun>(fun)(std::get<I>(args)...);
                }
            };
        }
        template <typename Fun, typename Args>
        auto tuple_call(Fun&& fun, Args&& args)
            -> decltype(nstd::functional::detail::tuple_call<nstd::utility::make_index_sequence<std::tuple_size<nstd::type_traits::decay_t<Args> >::value> >::call(nstd::utility::forward<Fun>(fun),
                                                                         nstd::utility::forward<Args>(args))) {
            return nstd::functional::detail::tuple_call<nstd::utility::make_index_sequence<std::tuple_size<nstd::type_traits::decay_t<Args> >::value> >::call(nstd::utility::forward<Fun>(fun),
                                                                    nstd::utility::forward<Args>(args));
        }
    }

}

// ----------------------------------------------------------------------------

#endif
