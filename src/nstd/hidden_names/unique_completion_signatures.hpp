// nstd/hidden_names/unique_completion_signatures.hpp                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_UNIQUE_COMPLETION_SIGNATURES
#define INCLUDED_NSTD_HIDDEN_NAMES_UNIQUE_COMPLETION_SIGNATURES

#include "nstd/execution/completion_signatures.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <bool, typename, typename... T>
    struct unique_aux {
        using type = ::nstd::execution::completion_signatures<T...>;
    };
    template <typename S0, typename... T>
    struct unique_aux<true, S0, T...> {
        using type = ::nstd::execution::completion_signatures<T..., S0>;
    };

    template <typename, typename> struct unique_completion_signatures;
    template <typename T>
    struct unique_completion_signatures<T, ::nstd::execution::completion_signatures<>> {
        using type = T;
    };
    template <typename... T, typename S0, typename... S1>
    struct unique_completion_signatures<::nstd::execution::completion_signatures<T...>,
                  ::nstd::execution::completion_signatures<S0, S1...>> {

        using type = typename unique_completion_signatures<typename unique_aux<(true && ... && not ::std::same_as<S0, T>), S0, T...>::type,
                                                    ::nstd::execution::completion_signatures<S1...>
                                                    >::type; 
    };
}

// ----------------------------------------------------------------------------

#endif
