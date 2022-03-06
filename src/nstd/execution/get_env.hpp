// nstd/execution/get_env.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_GET_ENV
#define INCLUDED_NSTD_EXECUTION_GET_ENV

#include "nstd/execution/no_env.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/is_same.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------
// [exec.get_env]

namespace nstd::execution {
    namespace hidden_names::get_env {
        struct cpo {
            template <typename C>
                requires(::nstd::tag_invocable<cpo, C>
                         && not ::nstd::type_traits::is_same_v<::nstd::hidden_names::exec_envs::no_env,
                                                               ::nstd::type_traits::remove_cvref_t<::nstd::tag_invoke_result_t<cpo, C>>>)
            auto operator()(C&& c) const {
                return ::nstd::tag_invoke(*this, ::nstd::utility::forward<C>(c));
            }
        };
    }

    inline namespace customization_points {
        using get_env_t = ::nstd::execution::hidden_names::get_env::cpo;
        inline constexpr get_env_t get_env{};
    }
}

// ----------------------------------------------------------------------------

#endif
