// cbt/iterator_declarations.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_CBT_ITERATOR_DECLARATIONS
#define INCLUDED_CBT_ITERATOR_DECLARATIONS

#include "cbt/detection.hpp"
#include <cstddef>
#include <iterator>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {

    // ------------------------------------------------------------------------

    template <typename T>
    using CopyCtorFor = decltype(std::decay_t<T>(std::declval<T>()));
    
    // ------------------------------------------------------------------------

    template <typename T>
    using OpDereference = decltype(*std::declval<T>());

    template <typename T>
    using OpPreIncrement = decltype(++std::declval<T&>());
    template <typename T>
    using OpPostIncrement = decltype(std::declval<T&>()++);

    template <typename T>
    using OpPreDecrement = decltype(--std::declval<T&>());
    template <typename T>
    using OpPostDecrement = decltype(std::declval<T&>()--);

    // ------------------------------------------------------------------------
    // partial iterator_category

    template <typename T>
    using IteratorCategory = typename T::iterator_category;

    template <typename T, bool = cbt::is_detected_v<IteratorCategory, T>>
    struct iterator_category {
    };
    template <typename T>
    using iterator_category_t = typename iterator_category<T>::type;

    template <typename T, bool B>
    struct iterator_category<T*, B>
        : std::enable_if<std::is_object_v<T>, std::random_access_iterator_tag> {
    };

    template <typename T, bool B>
    struct iterator_category<T const, B>
        : iterator_category<T> {
    };
    template <typename T>
    struct iterator_category<T, true> {
        using type = typename T::iterator_category;
    };

    // ------------------------------------------------------------------------
    // partial value_type

    template <typename T>
    using ValueType = typename T::value_type;

    template <typename T, bool = cbt::is_detected_v<ValueType, T>>
    struct value_type {
    };
    template <class T>
    using value_type_t = typename value_type<T>::type;

    template <typename T, bool B>
    struct value_type<T*, B>
        : std::enable_if<std::is_object_v<T>, std::remove_cv_t<T>> {
    };

    template <typename It, bool B>
    struct value_type<It const, B>
        : value_type<std::decay_t<It>> {
    };

    template <typename It>
    struct value_type<It, true>
        : std::enable_if<std::is_object_v<typename It::value_type>, typename It::value_type> {
    };

    // ------------------------------------------------------------------------
    // partial difference_type

    template <typename T>
    using DifferenceType = typename T::difference_type;

    template <typename T, bool = cbt::is_detected_v<DifferenceType, T>>
    struct difference_type {
    };
    template <typename T>
    using difference_type_t = typename difference_type<T>::type;

    template <typename T, bool B>
    struct difference_type<T*, B> {
        using type = std::ptrdiff_t;
    };

    template <typename T>
    struct difference_type<T, true> {
        using type = typename T::difference_type;
    };
}

// ----------------------------------------------------------------------------

#endif
