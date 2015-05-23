// nstd/operators/equality.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_OPERATORS_EQUALITY
#define INCLUDED_NSTD_OPERATORS_EQUALITY

#include "nstd/type_traits/is_convertible.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/decay.hpp"

// ----------------------------------------------------------------------------
// This components provides an inequality operator based on the quality
// operator. The result of `operator!=()` is simply the negation of equality
// copmarision of the arguments. The implementation correctly propages whether
// the operator is `noexcept` and/or `constexpr`. To have the inequality
// operator be generated a class needs to derive publicly from
// `nstd::operators::equality::tag`.

namespace nstd {
    namespace operators {
        namespace equality {
            struct tag {};
            template <typename T0, typename T1,
                      typename = nstd::type_traits::enable_if_t<
                          nstd::type_traits::is_convertible<nstd::type_traits::decay_t<T0>*, nstd::operators::equality::tag*>::value
                          && nstd::type_traits::is_convertible<nstd::type_traits::decay_t<T1>*, nstd::operators::equality::tag*>::value
                       >
                      >
            constexpr auto operator!= (T0&& value0, T1&& value1)
                noexcept(noexcept(bool(value0 == value1)))
                -> bool;
        }

        using equality_tag = ::nstd::operators::equality::tag;
    }
}

// ----------------------------------------------------------------------------

template <typename T0, typename T1, typename>
constexpr auto nstd::operators::equality::operator!= (T0&& value0, T1&& value1)
    noexcept(noexcept(bool(value0 == value1)))
    -> bool {
    return !bool(value0 == value1);
}

// ----------------------------------------------------------------------------

#endif
