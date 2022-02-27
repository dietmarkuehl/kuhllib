// src/nstd/execution/get_completion_signatures.hpp                   -*-C++-*-
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

#ifndef INCLUDED_SRC_NSTD_EXECUTION_GET_COMPLETION_SIGNATURES
#define INCLUDED_SRC_NSTD_EXECUTION_GET_COMPLETION_SIGNATURES

#include "nstd/hidden_names/is_instance_of.hpp"
#include "nstd/hidden_names/no_completion_signatures.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/dependent_completion_signatures.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename T>
    concept is_completion_signatures
        =  ::nstd::hidden_names::is_instance_of<T, ::nstd::execution::completion_signatures>
        || ::nstd::hidden_names::is_instance_of<T, ::nstd::execution::dependent_completion_signatures_impl::type>
        ;
}

namespace nstd::hidden_names::get_completion_signatures
{
    struct cpo {
        template <typename S>
        constexpr auto operator()(S&& s) const noexcept {
            return (*this)(::nstd::utility::forward<S>(s), ::nstd::hidden_names::exec_envs::no_env{});
        }
        template <typename S, typename E>
            requires(::nstd::hidden_names::is_completion_signatures<::nstd::tag_invoke_result_t<cpo, S, E>>)
        constexpr auto operator()(S&&, E&&) const noexcept {
            return ::nstd::tag_invoke_result_t<cpo, S, E>{};
        }
        template <typename S, typename E>
            requires(::nstd::hidden_names::is_completion_signatures<typename ::nstd::type_traits::remove_cvref_t<S>::completion_signatures>)
        constexpr auto operator()(S&&, E&&) const noexcept {
            using type = typename ::nstd::type_traits::remove_cvref_t<S>::completion_signatures;
            return type{};
        }
        //-dk:TODO deal with awaitables!
        template <typename S, typename E>
        constexpr auto operator()(S&&, E&&) const noexcept {
            return nstd::hidden_names::no_completion_signatures::type{};
        }
    };
}

namespace nstd::execution {
    using get_completion_signatures_t = ::nstd::hidden_names::get_completion_signatures::cpo;
    inline constexpr get_completion_signatures_t get_completion_signatures{};
}

// ----------------------------------------------------------------------------

#endif
