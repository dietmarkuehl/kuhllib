// nstd/execution/run_loop.cpp                                        -*-C++-*-
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

#include <nstd/execution/run_loop.hpp>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    int run_loop_dummy{};
}

// ----------------------------------------------------------------------------

auto nstd::hidden_names::run_loop::sender::get_completion_scheduler() const noexcept
    -> ::nstd::hidden_names::run_loop::scheduler {
    return ::nstd::hidden_names::run_loop::scheduler(this->d_loop);
}

auto nstd::hidden_names::run_loop::tag_invoke(::nstd::execution::get_completion_scheduler_t<::nstd::execution::set_value_t>,
                                              ::nstd::execution::run_loop::sender const& self) noexcept
    -> ::nstd::hidden_names::run_loop::scheduler {
    return self.get_completion_scheduler();
}

// ----------------------------------------------------------------------------

nstd::execution::run_loop::scheduler::scheduler(::nstd::execution::run_loop* loop)
    : d_loop(loop)
{
}

// ----------------------------------------------------------------------------

auto nstd::execution::run_loop::get_scheduler()
    -> ::nstd::execution::run_loop::scheduler
{
    return ::nstd::execution::run_loop::scheduler(this);
}