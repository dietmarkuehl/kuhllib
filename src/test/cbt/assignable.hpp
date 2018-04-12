// cbt/assignable.hpp                                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2018 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CBT_ASSIGNABLE
#define INCLUDED_CBT_ASSIGNABLE

// ----------------------------------------------------------------------------

#include "cbt/commonreference.hpp"
#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include "cbt/test.hpp"
#include <ostream>
#include <string>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {
    // template <typename T, typename U>
    // concept Assignable =
    //     is_lvalue_reference<T>::value
    //     && CommonReference<const remove_reference_t<T>&, const remove_reference_t<U>&>
    //     && requires(T t, U&& u) {
    //         { t = std::forward<U>(u) } -> Same<T>&&;
    //     };

    template <typename To, typename From>
    using OpAssignable = decltype(std::declval<To>() = std::declval<From>());

    template <typename T, typename U, typename V>
    bool Assignable(std::ostream& out, std::string_view name, V&& value) {
        (void)value;
        return cbt::test({ [&]{
                               if (!std::is_lvalue_reference_v<T>) {
                                   out << cbt::level::error << " lhs of assignment isn't an lvalue\n";
                                   return false;
                               }
                               return true;
                           }()
                         , cbt::CommonReference<std::remove_reference_t<T> const&,
                                                std::remove_reference_t<U> const&>(out, name)
                         , cbt::SimpleRequirement<OpAssignable, T&, U&&>(out, name, "assignment")
                         , cbt::CompoundRequirement<OpAssignable, T&, T&, U&&>(out, name, "assignment")
                         });
    }
}


// ----------------------------------------------------------------------------

#endif
