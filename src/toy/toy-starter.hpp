// toy-starter.hpp                                                    -*-C++-*-
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

#ifndef INCLUDED_TOY_STARTER
#define INCLUDED_TOY_STARTER

#include "toy-utility.hpp"
#include <utility>

namespace toy
{

// ----------------------------------------------------------------------------

template <typename Scheduler>
class starter
    : immovable
{
    struct job_base
        : immovable {
        Scheduler sched;
        job_base(Scheduler sched) : sched(sched) {}
        virtual ~job_base() = default;
    };

    struct receiver {
        job_base* job;
        friend Scheduler get_scheduler(receiver const& self) { return self.job->sched; }
        friend void set_value(receiver& self, auto) { delete self.job; }
        friend void set_error(receiver& self, auto) { delete self.job; }
    };

    template <typename S>
    struct job
        : job_base {
        decltype(connect(std::declval<S>(), std::declval<receiver>())) state;
        job(Scheduler sched, S&& s)
            : job_base(sched)
            , state(connect(std::move(s), receiver{this}))
        {
            start(state);
        }
    };

    Scheduler sched;

public:
    starter(Scheduler sched): sched(sched) {}
    void spawn(auto s) { new job<decltype(s)>(sched, std::move(s)); }
};

// ----------------------------------------------------------------------------

}

#endif
