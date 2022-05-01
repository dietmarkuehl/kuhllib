// nstd/stop_token/stop_source.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_STOP_SOURCE
#define INCLUDED_NSTD_STOP_TOKEN_STOP_SOURCE

#include "nstd/stop_token/nostopstate.hpp"
#include "nstd/stop_token/stop_state.hpp"
#include "nstd/stop_token/stop_token.hpp"
#include <memory>

// ----------------------------------------------------------------------------
// [thread.stoptoken.syn]

namespace nstd::stop_token_ns {
    class stop_source;
}

// ----------------------------------------------------------------------------

class nstd::stop_token_ns::stop_source
{
private:
    ::std::shared_ptr<::nstd::stop_token_ns::hidden_names::stop_state> d_state;

public:
    stop_source();
    explicit stop_source(::nstd::stop_token_ns::nostopstate_t) noexcept;
    stop_source(stop_source const&) noexcept = default;
    stop_source(stop_source&&) noexcept = default;
    ~stop_source();
    auto operator=(stop_source const&) noexcept -> stop_source& = default;
    auto operator=(stop_source&&) noexcept -> stop_source& = default;
    auto swap(stop_source&) noexcept -> void;

    // [stopsource.mem], stop handling
    [[nodiscard]] auto get_token() const noexcept -> ::nstd::stop_token_ns::stop_token;
    [[nodiscard]] auto stop_possible() const noexcept -> bool;
    [[nodiscard]] auto stop_requested() const noexcept -> bool;
    auto request_stop() noexcept -> bool;

    /*-dk:TODO [[nodiscard]]*/ friend bool operator==(stop_source const& lhs, stop_source const& rhs) noexcept {
        return lhs.d_state.get() == rhs.d_state.get();
    }
    friend auto swap(stop_source& lhs, stop_source& rhs) noexcept -> void {
        lhs.swap(rhs);
    }
};

// ----------------------------------------------------------------------------

#endif
