// src/nstd/hidden_names/add_signatures.hpp                           -*-C++-*-
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

#ifndef INCLUDED_SRC_NSTD_HIDDEN_NAMES_ADD_SIGNATURES
#define INCLUDED_SRC_NSTD_HIDDEN_NAMES_ADD_SIGNATURES

#include "nstd/execution/completion_signatures.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    namespace add_signatures_impl {
        template <typename, typename...> struct aux;
        template <typename, typename, typename> struct unique;
        template <typename I, typename... R>
        struct unique<I, ::nstd::execution::completion_signatures<R...>, ::nstd::execution::completion_signatures<>> {
            using type = ::nstd::execution::completion_signatures<R..., I>;
        };
        template <typename I, typename... R, typename... T>
        struct unique<I, ::nstd::execution::completion_signatures<R...>, ::nstd::execution::completion_signatures<I, T...>> {
            using type = ::nstd::execution::completion_signatures<R..., I, T...>;
        };
        template <typename I, typename... R, typename H, typename... T>
        struct unique<I, ::nstd::execution::completion_signatures<R...>, ::nstd::execution::completion_signatures<H, T...>> {
            using type = typename ::nstd::hidden_names::add_signatures_impl::unique<I, ::nstd::execution::completion_signatures<R..., H>, ::nstd::execution::completion_signatures<T...>>::type;
        };

        template <typename List>
        struct aux<List> {
            using type = List;
        };
        template <typename List, typename Item>
        struct aux<List, Item> {
            using type = typename ::nstd::hidden_names::add_signatures_impl::unique<Item, ::nstd::execution::completion_signatures<>, List>::type;
        };
        template <typename List, typename Head, typename Item, typename... Tail>
        struct aux<List, Head, Item, Tail...> {
            using type = typename ::nstd::hidden_names::add_signatures_impl::aux<typename ::nstd::hidden_names::add_signatures_impl::aux<List, Head>::type, Item, Tail...>::type;
        };
    }

    template <typename List, typename... Item>
    using add_signatures_t = typename ::nstd::hidden_names::add_signatures_impl::aux<List, Item...>::type;
}

// ----------------------------------------------------------------------------

#endif
