// nstd/hidden_names/merge_completion_signatures.hpp                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_MERGE_COMPLETION_SIGNATURES
#define INCLUDED_NSTD_HIDDEN_NAMES_MERGE_COMPLETION_SIGNATURES

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/dependent_completion_signatures.hpp"
#include "nstd/hidden_names/unique_completion_signatures.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names
{
        template <typename E, typename...> struct merge_completion_signatures;
        template <typename E> struct merge_completion_signatures<E> {
            using type = ::nstd::execution::completion_signatures<>;
        };
        template <typename E, typename T> struct merge_completion_signatures<E, T> {
            using type = ::nstd::execution::dependent_completion_signatures<E>;
        };
        template <typename E, typename... T> struct merge_completion_signatures<E, ::nstd::execution::completion_signatures<T...>> {
            using type = ::nstd::execution::completion_signatures<T...>;
        };
        template <typename E, typename List1, typename List2> struct merge_completion_signatures<E, List1, List2> {
            using type = ::nstd::execution::dependent_completion_signatures<E>;
        };
        template <typename E, typename... T1, typename... T2> struct merge_completion_signatures<E, ::nstd::execution::completion_signatures<T1...>, ::nstd::execution::completion_signatures<T2...>> {
            using type = typename unique_completion_signatures<::nstd::execution::completion_signatures<T1...>, ::nstd::execution::completion_signatures<T2...>>::type;
        };
        template <typename E, typename List1, typename List2, typename Head, typename... Tail> struct merge_completion_signatures<E, List1, List2, Head, Tail...> {
            using type = typename merge_completion_signatures<E, typename merge_completion_signatures<E, List1, List2>::type, Head, Tail...>::type;
        };
        template <typename E, typename... T>
        using merge_completion_signatures_t = typename merge_completion_signatures<E, T...>::type;

}

// ----------------------------------------------------------------------------

#endif
