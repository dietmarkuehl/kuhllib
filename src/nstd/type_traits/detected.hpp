// nstd/type_traits/detected.hpp                                      -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_DETECTED
#define INCLUDED_NSTD_TYPE_TRAITS_DETECTED

#include "nstd/type_traits/integral_constant.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace type_traits {
        template <typename...>
        using void_t = void;

        struct nonesuch;

        // --------------------------------------------------------------------
        // The heart of the detection idiom: the detect class template has a
        // default definition deriving from false_type and a specialization for
        // void deriving from true_type. The actual specializtion is declared
        // using void_t<Operation<Args...>> which is a valid spelling ony when
        // Operation<Args> happens to be a valid type. The detection of a
        // member m taking arguments a... works using an appropriately declared
        // template alias:
        //
        //     template <typename Type, typename... Args>
        //     using m_op = decltype(std::declval<Type>().m(std::declval<Args>()...));
        //     ...
        //     constexpr bool has_m = is_detected_v<m_op, X, A...>;
        //
        // A nested type is provided referring to the type of the detected
        // operation when such an operation is present and to a default type
        // otherwise.
        template <typename Type, template <typename...> class, typename, typename...>
        struct detect
            : nstd::type_traits::false_type {
            using type = Type;
        };
        template <typename Default,
                  template <typename...> class Operation,
                  typename... Args>
        struct detect<Default, Operation, void_t<Operation<Args...>>, Args...> 
            : nstd::type_traits::true_type {
            using type = Operation<Args...>;
        };
        template <typename Type, template <typename...> class Operation, typename Void, typename... Args>
        using detect_t = typename detect<Type, Operation, Void, Args...>::type;
        
        template <template <typename...> class Operation, typename... Args>
        using is_detected = detect<nonesuch, Operation, void, Args...>;
        template <template <typename...> class Operation, typename... Args>
        constexpr bool is_detected_v = is_detected<Operation, Args...>::value;
    }
}

// ----------------------------------------------------------------------------
// The type nonesuch is defined but otherwise not usable. It is used as the
// default type in some uses of the detection idiom.

struct nstd::type_traits::nonesuch {
    nonesuch(nonesuch&)       = delete;
    ~nonesuch()               = delete;
    void operator=(nonesuch&) = delete;
};

// ----------------------------------------------------------------------------

#endif
