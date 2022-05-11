// nstd/type_traits/is_volatile.hpp                                   -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_IS_VOLATILE
#define INCLUDED_NSTD_TYPE_TRAITS_IS_VOLATILE

#include "nstd/type_traits/integral_constant.hpp"

// ----------------------------------------------------------------------------
// [meta.trans.cv]

namespace nstd::type_traits {
    template <typename T> struct is_volatile: nstd::type_traits::false_type {};
    template <typename T> struct is_volatile<T volatile>: nstd::type_traits::true_type {};
    template <typename T> inline constexpr bool is_volatile_v = is_volatile<T>::value;
}

// ----------------------------------------------------------------------------

#endif
