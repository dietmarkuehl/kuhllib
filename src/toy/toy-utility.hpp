// toy-utility.hpp                                                    -*-C++-*-
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

#ifndef INCLUDED_TOY_UTILITY
#define INCLUDED_TOY_UTILITY

#include <algorithm>
#include <chrono>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace toy
{

// ----------------------------------------------------------------------------

struct none {};

struct immovable {
    immovable() = default;
    immovable(immovable&&) = delete;
};

template <typename S> using sender_result_t = typename std::decay_t<S>::result_t;

// ----------------------------------------------------------------------------

template <typename State>
struct connector {
    State state;
    connector(auto pair)
        : state(connect(std::move(std::get<0>(pair)), std::move(std::get<1>(pair)))) {
    }
    template <typename S, typename R>
    connector(S&& sender, R&& receiver)
        : state(connect(std::forward<S>(sender), std::forward<R>(receiver))) {
    }
    friend void start(connector& self) { start(self.state); }
};

// ----------------------------------------------------------------------------

template <typename V>
class timer_queue
{
private:
    using time_point_t = std::chrono::system_clock::time_point;
    using timer_t      = std::pair<time_point_t, V>;
    struct compare_t { bool operator()(auto&& a, auto&& b) { return a.first > b.first; } };
    using queue_t      = std::vector<timer_t>;

    queue_t times;

public:
    bool empty() const { return times.empty(); }
    void push(time_point_t time, V op) {
        auto it(std::lower_bound(times.begin(), times.end(), std::make_pair(time, op), compare_t()));
        times.insert(it, std::make_pair(time, op));
    }
    void pop() { times.erase(times.begin()); }
    timer_t const& top() { return times.front(); }
    void erase(V i) {
        auto it = std::find_if(times.begin(), times.end(), [i](auto&& p){ return p.second == i; });
        if (it != times.end()) {
            times.erase(it);
        }
    }
};

// ----------------------------------------------------------------------------

}

#endif
