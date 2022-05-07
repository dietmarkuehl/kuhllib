// nstd/execution/run_loop.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_RUN_LOOP
#define INCLUDED_NSTD_EXECUTION_RUN_LOOP

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/schedule.hpp"
#include "nstd/execution/scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    class run_loop;
}

namespace nstd::hidden_names::run_loop {
    class state_base;
    template <::nstd::execution::receiver> class state;
    class sender;
    class scheduler;
    auto tag_invoke(::nstd::execution::get_completion_scheduler_t<::nstd::execution::set_value_t>,
                    ::nstd::hidden_names::run_loop::sender const&) noexcept
        -> ::nstd::hidden_names::run_loop::scheduler;
}

// ----------------------------------------------------------------------------

class nstd::execution::run_loop
{
public:
    using state_base = ::nstd::hidden_names::run_loop::state_base;
    template <::nstd::execution::receiver Receiver>
    using state = ::nstd::hidden_names::run_loop::state<Receiver>; //-dk:TODO user receiver_of<???>
    using scheduler = ::nstd::hidden_names::run_loop::scheduler;
    using sender = ::nstd::hidden_names::run_loop::sender;

    run_loop() noexcept = default;
    run_loop(run_loop&&) = delete;
    ~run_loop() = default;

    auto get_scheduler() -> scheduler;
    auto run() -> void;
    auto finish() -> void;
};

// ----------------------------------------------------------------------------

class nstd::hidden_names::run_loop::sender
{
private:
    friend class ::nstd::hidden_names::run_loop::scheduler;

    ::nstd::execution::run_loop* d_loop;

    auto get_completion_scheduler() const noexcept -> ::nstd::hidden_names::run_loop::scheduler;

    sender(::nstd::execution::run_loop* loop): d_loop(loop) {}

public:
    using completion_signatures = ::nstd::execution::completion_signatures<
            ::nstd::execution::set_value_t()
        >;

    friend auto tag_invoke(::nstd::execution::get_completion_scheduler_t<::nstd::execution::set_value_t>,
                           ::nstd::hidden_names::run_loop::sender const&) noexcept
        -> ::nstd::execution::run_loop::scheduler;
    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&&, Receiver&& r)
        noexcept(noexcept(::nstd::type_traits::remove_cvref_t<Receiver>(::nstd::utility::forward<Receiver>(r))))
        -> ::nstd::execution::run_loop::state<Receiver> {
        return {};
    }
};

static_assert(::nstd::execution::sender<::nstd::execution::run_loop::sender>);

// ----------------------------------------------------------------------------

class nstd::hidden_names::run_loop::scheduler
{
private:
    friend class ::nstd::execution::run_loop;
    friend class ::nstd::hidden_names::run_loop::sender;

    ::nstd::execution::run_loop* d_loop;

    scheduler(::nstd::execution::run_loop*);
    auto schedule() const noexcept -> ::nstd::execution::run_loop::sender {
        return ::nstd::execution::run_loop::sender(this->d_loop);
    }
public:
    auto operator== (scheduler const&) const -> bool = default;
    friend auto tag_invoke(::nstd::execution::schedule_t, scheduler const& self) noexcept
        -> ::nstd::execution::run_loop::sender {
        return self.schedule();
    }
};

static_assert(::nstd::execution::scheduler<::nstd::execution::run_loop::scheduler>);

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver>
class nstd::hidden_names::run_loop::state
{
    friend auto tag_invoke(::nstd::execution::start_t, state&) noexcept -> void {
    }
};

// ----------------------------------------------------------------------------

#endif
