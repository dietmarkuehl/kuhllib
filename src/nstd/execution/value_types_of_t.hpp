// src/nstd/execution/value_types_of_t.hpp                            -*-C++-*-
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

#ifndef INCLUDED_SRC_NSTD_EXECUTION_VALUE_TYPES_OF
#define INCLUDED_SRC_NSTD_EXECUTION_VALUE_TYPES_OF

#include "nstd/execution/completion_signatures_of_t.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/hidden_names/decayed_tuple.hpp"
#include "nstd/hidden_names/filter_completions.hpp"
#include "nstd/hidden_names/variant_or_empty.hpp"

// ----------------------------------------------------------------------------
// [exec.sndtraitst]

namespace nstd::execution::hidden_names::value_types_of_t {
    template <typename, template <typename...> class> struct transform;
    template <typename... Args, template <typename...> class Tuple>
    struct transform<::nstd::execution::set_value_t(Args...), Tuple> {
        using type = Tuple<Args...>;
    };
    template <typename, template <typename...> class, template <typename...> class>
    struct transform_list;
    template <typename... Signature, template <typename...> class Tuple, template <typename...> class Variant>
    struct transform_list<::nstd::execution::completion_signatures<Signature...>, Tuple, Variant> {
        using type = Variant<typename transform<Signature, Tuple>::type...>;
    };

    template <typename Sender,
              typename Env,
              template <typename...> class Tuple,
              template <typename...> class Variant>
    struct helper {
        using list = ::nstd::execution::completion_signatures_of_t<Sender, Env>;
        using filtered = typename nstd::hidden_names::filter_completions<
            ::nstd::execution::set_value_t,
            ::nstd::execution::completion_signatures<>, list
            >::type;
        using type = typename transform_list<filtered, Tuple, Variant>::type;
    };
}

namespace nstd::execution {
    template <typename Sender,
              typename Env = ::nstd::hidden_names::exec_envs::no_env,
              template <typename...> class Tuple = ::nstd::hidden_names::decayed_tuple,
              template <typename...> class Variant = ::nstd::hidden_names::variant_or_empty>
        requires ::nstd::execution::sender<Sender, Env>
    using value_types_of_t = typename ::nstd::execution::hidden_names::value_types_of_t::helper<Sender, Env, Tuple, Variant>::type;
}

// ----------------------------------------------------------------------------

#endif
