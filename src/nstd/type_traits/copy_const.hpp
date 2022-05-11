// nstd/type_traits/copy_const.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_COPY_CONST
#define INCLUDED_NSTD_TYPE_TRAITS_COPY_CONST

#include "nstd/type_traits/add_const.hpp"
#include "nstd/type_traits/conditional.hpp"
#include "nstd/type_traits/is_const.hpp"

// ----------------------------------------------------------------------------

namespace nstd::type_traits {
    template <typename F, typename T> struct copy_const;
    template <typename F, typename T> using copy_const_t = typename ::nstd::type_traits::copy_const<F, T>::type;
}

// ----------------------------------------------------------------------------

template <typename F, typename T>
struct nstd::type_traits::copy_const
{
    using type = ::nstd::type_traits::conditional_t<::nstd::type_traits::is_const_v<F>,
                                                    ::nstd::type_traits::add_const_t<T>,
                                                    T>;
};

// ----------------------------------------------------------------------------

#endif
