// nstd/hidden_names/filter_completions.hpp                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_FILTER_COMPLETIONS
#define INCLUDED_NSTD_HIDDEN_NAMES_FILTER_COMPLETIONS

#include "nstd/execution/completion_signatures.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename CPO, typename, typename> struct filter_completions;
    template <typename CPO, typename List>
    struct filter_completions<CPO, List, ::nstd::execution::completion_signatures<>> {
        using type = List;
    };
    template <typename CPO, typename... To, typename... From, typename... Args>
    struct filter_completions<CPO,
                  ::nstd::execution::completion_signatures<To...>,
                  ::nstd::execution::completion_signatures<CPO(Args...), From...>> {
        using type = typename ::nstd::hidden_names::filter_completions<CPO,
                                     ::nstd::execution::completion_signatures<To..., CPO(Args...)>,
                                     ::nstd::execution::completion_signatures<From...>>::type;
    };
    template <typename CPO, typename... To, typename... From, typename Fun, typename... Args>
    struct filter_completions<CPO,
                  ::nstd::execution::completion_signatures<To...>,
                  ::nstd::execution::completion_signatures<Fun(Args...), From...>> {
        using type = typename ::nstd::hidden_names::filter_completions<CPO,
                                     ::nstd::execution::completion_signatures<To...>,
                                     ::nstd::execution::completion_signatures<From...>>::type;
    };

}

// ----------------------------------------------------------------------------

#endif
