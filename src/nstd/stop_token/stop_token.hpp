// nstd/stop_token/stop_token.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_STOP_TOKEN
#define INCLUDED_NSTD_STOP_TOKEN_STOP_TOKEN

#include "nstd/stop_token/stop_state.hpp"
#include <concepts>
#include <memory>

// ----------------------------------------------------------------------------
// [thread.stoptoken.syn]

namespace nstd::stop_token_ns {
    class stop_source;
    class stop_token;
    template <typename Callback>
        requires ::std::invocable<Callback> && ::std::destructible<Callback>
    class stop_callback;
}

// ----------------------------------------------------------------------------
// [stoptoken.general]

class nstd::stop_token_ns::stop_token
{
    friend class ::nstd::stop_token_ns::stop_source;
    template <typename Callback>
        requires ::std::invocable<Callback> && ::std::destructible<Callback>
    friend class ::nstd::stop_token_ns::stop_callback;

private:
    ::std::shared_ptr<::nstd::stop_token_ns::hidden_names::stop_state> d_state;

    explicit stop_token(::std::shared_ptr<::nstd::stop_token_ns::hidden_names::stop_state> const&) noexcept;

public:
    stop_token() noexcept = default;

    stop_token(stop_token const&) noexcept = default;
    stop_token(stop_token&&) noexcept = default;
    ~stop_token() = default;
    auto operator=(stop_token const&) noexcept -> stop_token& = default;
    auto operator=(stop_token&&) noexcept -> stop_token& = default;
    auto swap(stop_token&) noexcept -> void;

    [[nodiscard]] auto stop_requested() const noexcept -> bool;
    [[nodiscard]] auto stop_possible() const noexcept -> bool;

    friend /*-dk:TODO [[nodiscard]] */ auto operator==(stop_token const& lhs, stop_token const& rhs) noexcept -> bool {
        return lhs.d_state.get() == rhs.d_state.get();
    }
    friend auto swap(stop_token& lhs, stop_token& rhs) noexcept -> void {
        return lhs.swap(rhs);
    }
};

// ----------------------------------------------------------------------------

inline auto nstd::stop_token_ns::stop_token::stop_requested() const noexcept -> bool
{
    return this->d_state && this->d_state->d_stop_requested;
}

inline auto nstd::stop_token_ns::stop_token::stop_possible() const noexcept -> bool
{
    return this->d_state.get();
}

// ----------------------------------------------------------------------------

#endif
