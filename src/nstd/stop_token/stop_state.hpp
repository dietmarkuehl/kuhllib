// nstd/stop_token/stop_state.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_STOP_STATE
#define INCLUDED_NSTD_STOP_TOKEN_STOP_STATE

#include "nstd/stop_token/stop_callback_base.hpp"
#include "nstd/container/intrusive_list.hpp"
#include <atomic>
#include <mutex>

// ----------------------------------------------------------------------------

namespace nstd::stop_token_ns::hidden_names {
    struct stop_state;
}

// ----------------------------------------------------------------------------

struct nstd::stop_token_ns::hidden_names::stop_state {
    //-dk:TODO verify/fix the stop_state concurrency
    using list = ::nstd::container::intrusive_list<::nstd::stop_token_ns::hidden_names::stop_callback_base>;
    list                d_list;
    ::std::atomic<bool> d_stop_requested{false};
    ::std::atomic<bool> d_busy{false};

    auto insert(::nstd::stop_token_ns::hidden_names::stop_callback_base* callback) -> list::iterator {
        while (this->d_busy.exchange(true)) {
        }
        auto rc = this->d_list.insert(this->d_list.end(), *callback);
        if (this->d_stop_requested) {
            callback->call();
        }
        this->d_busy = false;
        return rc;
    }
    auto erase(list::iterator it) -> void {
        while (this->d_busy.exchange(true)) {
        }
        this->d_list.erase(it);
        this->d_busy = false;
    }
    auto call() -> void {
        while (this->d_busy.exchange(true)) {
        }
        while (!this->d_list.empty()) {
            this->d_list.front().call();
            this->d_list.pop_front();
        }
        this->d_busy = false;
    }
};

// ----------------------------------------------------------------------------

#endif
