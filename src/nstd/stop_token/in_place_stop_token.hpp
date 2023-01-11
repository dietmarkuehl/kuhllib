// nstd/stop_token/in_place_stop_token.hpp                            -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_IN_PLACE_STOP_TOKEN
#define INCLUDED_NSTD_STOP_TOKEN_IN_PLACE_STOP_TOKEN

#include "nstd/stop_token/stoppable_token.hpp"
#include "nstd/utility/move.hpp"
#include <atomic>

// ----------------------------------------------------------------------------

namespace nstd::stop_token_ns {
    class in_place_stop_callback_base;
    class in_place_stop_source;
    class in_place_stop_token;
    template <typename> struct in_place_stop_callback;
}

// ----------------------------------------------------------------------------

class nstd::stop_token_ns::in_place_stop_callback_base
{
private:
    virtual auto do_call() -> void {}
public:
    in_place_stop_callback_base() = default;
    in_place_stop_callback_base(in_place_stop_callback_base&&) = delete;
    virtual ~in_place_stop_callback_base() = default;
    in_place_stop_callback_base*      d_next = nullptr;
    ::std::atomic<bool> d_called = false;

    auto call() -> void { this->d_called = true; this->do_call(); }
};

// ----------------------------------------------------------------------------

class nstd::stop_token_ns::in_place_stop_source
{
private:
    in_place_stop_callback_base                 d_marker;
    in_place_stop_callback_base                 d_end;
    ::std::atomic<in_place_stop_callback_base*> d_list = &this->d_end;
    ::std::atomic<bool>           d_stop_requested{false};

public:
    friend class ::nstd::stop_token_ns::in_place_stop_token;

    in_place_stop_source() = default;
    in_place_stop_source(in_place_stop_source const&) = delete;
    auto operator= (in_place_stop_source const&) -> in_place_stop_source& = delete;
    ~in_place_stop_source() = default;

    auto stop_requested() const noexcept -> bool { return this->d_stop_requested; }
    auto token() -> ::nstd::stop_token_ns::in_place_stop_token;
    auto stop() -> void {
        bool expect{false};
        if (!this->d_stop_requested.compare_exchange_strong(expect, true)) {
            return;
        }
        in_place_stop_callback_base* head = this->d_list;
        // The marker is set while calling the callbacks to avoid registered
        // callbacks being destroyed after they realized that they didn't need
        // to do anything during remove() but while being called.
        while (head && (head == &this->d_marker || !this->d_list.compare_exchange_strong(head, &this->d_marker))) {
        }
        if (head) {
            for (auto node = head; node; ) {
                auto tmp = node->d_next;
                node->call();
                node = tmp;
            }
            this->d_list = nullptr;
        }
    }
    auto insert(in_place_stop_callback_base* callback) -> void {
        for (in_place_stop_callback_base* head = this->d_list; head; ) {
            callback->d_next = head;
            if (   head != &this->d_marker
                && this->d_list.compare_exchange_strong(head, callback)) {
                return;
            }
        }
        callback->call();
    }
    auto remove(in_place_stop_callback_base* callback) -> void {
        if (callback->d_called) {
            return;
        }
        in_place_stop_callback_base* head = this->d_list;
        for (; ; head = this->d_list) {
            if (head == nullptr) {
                return;
            }
            if (head == &this->d_marker) {
                continue;
            }
            if (this->d_list.compare_exchange_strong(head, &this->d_marker)) {
                break;
            }
        }
        if (head == callback) {
            head = head->d_next;
        }
        else {
            in_place_stop_callback_base* node = head;
            while (node && node->d_next != callback) {
                node = node->d_next;
            }
            if (node) {
                // node->d_next must be non-null because the last node is &this->d_end
                in_place_stop_callback_base* next = node->d_next;
                in_place_stop_callback_base* keep = next->d_next;
                node->d_next = keep;
            }
        }

        this->d_list = head;
    }
};

// ----------------------------------------------------------------------------

class nstd::stop_token_ns::in_place_stop_token
{
    friend class ::nstd::stop_token_ns::in_place_stop_source;
    template <typename Callback>
    friend struct ::nstd::stop_token_ns::in_place_stop_callback;

private:
    ::nstd::stop_token_ns::in_place_stop_source* d_state;

    explicit in_place_stop_token(::nstd::stop_token_ns::in_place_stop_source* state)
        : d_state(state)
    {
    }
public:
    template <typename Callable>
    struct callback_type
        : ::nstd::stop_token_ns::in_place_stop_callback<Callable> {
        callback_type(in_place_stop_token token, Callable&& callback)
            : ::nstd::stop_token_ns::in_place_stop_callback<Callable>(token, ::nstd::utility::move(callback)) {
        }
    };

    auto operator== (in_place_stop_token const&) const noexcept -> bool = default;
    auto stop_requested() const noexcept -> bool { return this->d_state->stop_requested(); }
    constexpr auto stop_possible() const noexcept -> bool { return true; }
};

static_assert(::nstd::stop_token_ns::stoppable_token<::nstd::stop_token_ns::in_place_stop_token>);

// ----------------------------------------------------------------------------

template <typename Callable>
struct nstd::stop_token_ns::in_place_stop_callback
    : ::nstd::stop_token_ns::in_place_stop_callback_base
{
    in_place_stop_source* d_state;
    Callable            d_callback;
    in_place_stop_callback(in_place_stop_token token, Callable callback)
        : d_state(token.d_state)
        , d_callback(::nstd::utility::move(callback)) {
        this->d_state->insert(this);
    }
    in_place_stop_callback(in_place_stop_callback const&) = delete;
    auto operator= (in_place_stop_callback const&) -> in_place_stop_callback& = delete;
    ~in_place_stop_callback() {
        this->d_state->remove(this);
    }
    auto do_call() ->void override { this->d_callback(); }
};

// ----------------------------------------------------------------------------

inline auto nstd::stop_token_ns::in_place_stop_source::token()
    -> ::nstd::stop_token_ns::in_place_stop_token
{
    return ::nstd::stop_token_ns::in_place_stop_token(this);
}

// ----------------------------------------------------------------------------

#endif
