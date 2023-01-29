// nstd/hidden_names/valid_completion_for.hpp                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_VALID_COMPLETION_FOR
#define INCLUDED_NSTD_HIDDEN_NAMES_VALID_COMPLETION_FOR

#include "nstd/functional/tag_invoke.hpp"

// ----------------------------------------------------------------------------
// [exec.recv]

namespace nstd::hidden_names {
    template <typename, typename>
    struct nothrow_tag_invocable_helper_t {
        static constexpr bool value{false};
    };
    template <typename T, typename Tag, typename... A>
    struct nothrow_tag_invocable_helper_t<T, Tag(*)(A...)> {
        static constexpr bool value{::nstd::nothrow_tag_invocable<Tag, T, A...>};
    };

    template <typename Signature, typename T>
    concept valid_completion_for
#ifndef _MSC_VER
        = requires(Signature* sig) {
            []<typename Tag, typename...A>(Tag(*)(A...))
                requires ::nstd::nothrow_tag_invocable<Tag, T, A...>
            {}(sig);
        }
#else
        = nothrow_tag_invocable_helper_t<T, Signature>::value;
#endif
        ;
}

// ----------------------------------------------------------------------------

#endif
