// nstd/execution/get_delegatee_scheduler.hpp                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_GET_DELEGATEE_SCHEDULER
#define INCLUDED_NSTD_EXECUTION_GET_DELEGATEE_SCHEDULER

// ----------------------------------------------------------------------------

#include "nstd/concepts/same_as.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/as_const.hpp"

// ----------------------------------------------------------------------------
// [exec.queries.get_delegatee_scheduler]

namespace nstd::hidden_names::get_delegatee_scheduler {
    struct cpo {
        template <typename Env>
            requires requires(Env&& env, cpo const& get_delegatee_scheduler) {
                    { ::nstd::tag_invoke(get_delegatee_scheduler, ::nstd::utility::as_const(env)) } noexcept
                        -> nstd::execution::scheduler;
                }
                && (not ::nstd::concepts::same_as<::nstd::hidden_names::exec_envs::no_env,
                                                  ::nstd::type_traits::remove_cvref_t<Env>>)
        constexpr auto operator()(Env&& env) const noexcept
        {
            return ::nstd::tag_invoke(*this, ::nstd::utility::as_const(env));
        }
        //-dk:TODO no arg overload
    };
}

namespace nstd::execution::inline customization_points {
    using get_delegatee_scheduler_t = ::nstd::hidden_names::get_delegatee_scheduler::cpo;
    inline constexpr get_delegatee_scheduler_t get_delegatee_scheduler{};
}

// ----------------------------------------------------------------------------

#endif
