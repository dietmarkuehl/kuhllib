// nstd/execution/error_types_of_t.hpp                                -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_ERROR_TYPES_OF
#define INCLUDED_NSTD_EXECUTION_ERROR_TYPES_OF

#include "nstd/execution/completion_signatures_of_t.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/hidden_names/filter_completions.hpp"
#include "nstd/hidden_names/variant_or_empty.hpp"

// ----------------------------------------------------------------------------
// [exec.sndtraitst]

namespace nstd::execution::hidden_names::error_types_of_t {
    template <typename> struct transform;
    template <typename Arg>
    struct transform<::nstd::execution::set_error_t(Arg)> {
        using type = Arg;
    };
    template <typename, template <typename...> class>
    struct transform_list;
    template <typename... Signature, template <typename...> class Variant>
    struct transform_list<::nstd::execution::completion_signatures<Signature...>, Variant> {
        using type = Variant<typename transform<Signature>::type...>;
    };
}

namespace nstd::execution {
    template <::nstd::execution::sender Sender, typename Env, template <typename...> class Variant = nstd::hidden_names::variant_or_empty>
    using error_types_of_t = typename ::nstd::execution::hidden_names::error_types_of_t::transform_list<
            typename nstd::hidden_names::filter_completions<
                ::nstd::execution::set_error_t,
                ::nstd::execution::completion_signatures<>,
                ::nstd::execution::completion_signatures_of_t<Sender, Env>
            >::type, Variant>::type;
}

// ----------------------------------------------------------------------------

#endif
