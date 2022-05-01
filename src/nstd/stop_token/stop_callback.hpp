// nstd/stop_token/stop_callback.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_STOP_CALLBACK
#define INCLUDED_NSTD_STOP_TOKEN_STOP_CALLBACK

#include "nstd/stop_token/stop_callback_base.hpp"
#include "nstd/stop_token/stop_token.hpp"
#include "nstd/stop_token/stop_state.hpp"
#include "nstd/container/intrusive_list.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

#include <concepts>
#include <memory>

// ----------------------------------------------------------------------------
// [thread.stoptoken.syn]

namespace nstd::stop_token_ns {
    template <typename Callback>
        requires ::std::invocable<Callback> && ::std::destructible<Callback>
    class stop_callback;

    template <typename Callback>
    stop_callback(::nstd::stop_token_ns::stop_token, Callback)
        -> stop_callback<Callback>;
}

// ----------------------------------------------------------------------------

template <typename Callback>
    requires ::std::invocable<Callback> && ::std::destructible<Callback>
class nstd::stop_token_ns::stop_callback
    : public ::nstd::stop_token_ns::hidden_names::stop_callback_base
{
private:
    using state = ::nstd::stop_token_ns::hidden_names::stop_state;
    ::std::shared_ptr<state> d_state;
    state::list::iterator    d_iterator;
    Callback                 d_callback;

    auto register_callback() {
        if (!this->d_state) {
            return;
        }
        this->d_iterator = this->d_state->insert(this);
    }
protected:
    auto do_call() noexcept -> void override {
        ::nstd::utility::forward<Callback>(this->d_callback)();
    }

public:
    using callback_type = Callback;

    template <typename C>
        requires ::std::constructible_from<Callback, C>
    stop_callback(::nstd::stop_token_ns::stop_token const& token, C&& c)
        noexcept(::std::is_nothrow_constructible_v<Callback, C>)
        : d_state(token.d_state)
        , d_callback(::nstd::utility::forward<C>(c))
    {
        this->register_callback();
    }
    template <typename C>
        requires ::std::constructible_from<Callback, C>
    stop_callback(::nstd::stop_token_ns::stop_token&& token, C&& c)
        noexcept(::std::is_nothrow_constructible_v<Callback, C>)
        : d_state(::nstd::utility::move(token.d_state))
        , d_callback(::nstd::utility::forward<C>(c))
    {
        this->register_callback();
    }
    stop_callback(stop_callback&&) = delete;
    ~stop_callback()
    {
        if (this->d_iterator != state::list::iterator()) {
            this->d_state->erase(this->d_iterator);
        }
    }
};

// ----------------------------------------------------------------------------

#endif
