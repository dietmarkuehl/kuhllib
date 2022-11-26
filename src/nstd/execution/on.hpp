// nstd/execution/on.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_ON
#define INCLUDED_NSTD_EXECUTION_ON

#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    namespace hidden_names::on {
        template <::nstd::execution::sender Sender>
        struct sender {
            using completion_signatures = typename Sender::completion_signatures;
        };
        struct cpo {
            template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender>
                requires requires(cpo const& on, Scheduler&& scheduler, Sender&& sender) {
                    {
                        tag_invoke(on,
                                   ::nstd::utility::forward<Scheduler>(scheduler),
                                   ::nstd::utility::forward<Sender>(sender))
                    } -> ::nstd::execution::sender;
                }
            auto operator()(Scheduler&& scheduler, Sender&& sender) const {
                return nstd::tag_invoke(*this,
                                        nstd::utility::forward<Scheduler>(scheduler),
                                        nstd::utility::forward<Sender>(sender));
            }
            template <::nstd::execution::scheduler Scheduler, ::nstd::execution::sender Sender>
                requires (not requires(cpo const& on, Scheduler&& scheduler, Sender&& sender) {
                    {
                        tag_invoke(on,
                                   ::nstd::utility::forward<Scheduler>(scheduler),
                                   ::nstd::utility::forward<Sender>(sender))
                    } -> ::nstd::execution::sender;
                })
            auto operator()(Scheduler&&, Sender&&) const {
                return ::nstd::execution::hidden_names::on::sender<Sender>{};
            }
        };
    }
    using on_t = ::nstd::execution::hidden_names::on::cpo;
    inline constexpr on_t on{};
}

// ----------------------------------------------------------------------------

#endif
