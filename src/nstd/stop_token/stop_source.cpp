// nstd/stop_token/stop_source.cpp                                    -*-C++-*-
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

#include <nstd/stop_token/stop_source.hpp>

// ----------------------------------------------------------------------------

namespace nstd::stop_token_ns {
    int stop_source_dummy{};
}

// ----------------------------------------------------------------------------

nstd::stop_token_ns::stop_source::stop_source()
    : d_state(::std::make_shared<::nstd::stop_token_ns::hidden_names::stop_state>())
{
}

nstd::stop_token_ns::stop_source::stop_source(nstd::stop_token_ns::nostopstate_t) noexcept
    : d_state()
{
}

nstd::stop_token_ns::stop_source::~stop_source() = default;

// ----------------------------------------------------------------------------

auto nstd::stop_token_ns::stop_source::swap(nstd::stop_token_ns::stop_source& other) noexcept -> void
{
    this->d_state.swap(other.d_state);
}

// ----------------------------------------------------------------------------

auto nstd::stop_token_ns::stop_source::get_token() const noexcept -> ::nstd::stop_token_ns::stop_token
{
    return ::nstd::stop_token_ns::stop_token(this->d_state);
}

auto nstd::stop_token_ns::stop_source::stop_possible() const noexcept -> bool
{
    return this->d_state.get();
}

auto nstd::stop_token_ns::stop_source::stop_requested() const noexcept -> bool
{
    return this->d_state.get() && this->d_state->d_stop_requested;
}

auto nstd::stop_token_ns::stop_source::request_stop() noexcept -> bool
{
    if (!this->d_state) {
        return false;
    }
    bool rc = this->d_state->d_stop_requested.exchange(true);
    if (!rc) {
        this->d_state->call();
        return true;
    }
    return false;
}