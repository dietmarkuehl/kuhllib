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
#include <iostream>

// ----------------------------------------------------------------------------

namespace nstd::stop_token {
    class in_place_stop_source;
    class in_place_stop_token;
}

// ----------------------------------------------------------------------------

class nstd::stop_token::in_place_stop_source
{
private:
    class callback_base
    {
    private:
        virtual auto do_call() -> void {}
    public:
        virtual ~callback_base() = default;
        callback_base*      d_next = nullptr;
        ::std::atomic<bool> d_called = false;

        auto call() -> void { this->d_called = true; this->do_call(); }
    };

    callback_base                 d_marker;
    callback_base                 d_end;
    ::std::atomic<callback_base*> d_list = &this->d_end;
    ::std::atomic<bool>           d_stop_requested{false};

public:
    friend class ::nstd::stop_token::in_place_stop_token;

    in_place_stop_source() = default;
    in_place_stop_source(in_place_stop_source const&) = delete;
    auto operator= (in_place_stop_source const&) -> in_place_stop_source& = delete;
    ~in_place_stop_source() = default;

    auto stop_requested() const noexcept -> bool { return this->d_stop_requested; }
    auto token() -> ::nstd::stop_token::in_place_stop_token;
    auto stop() -> void {
        ::std::cout << "stopping stop state\n";
        bool expect{false};
        if (!this->d_stop_requested.compare_exchange_strong(expect, true)) {
            ::std::cout << "stop state already stopped\n";
            return;
        }
        callback_base* head = this->d_list;
        // The marker is set while calling the callbacks to avoid registered
        // callbacks being destroyed after they realized that they didn't need
        // to do anything during remove() but while being called.
        while (head && (head == &this->d_marker || !this->d_list.compare_exchange_strong(head, &this->d_marker))) {
        }
        if (head) {
            for (auto node = head; node; node = node->d_next) {
                ::std::cout << "stop state: calling calback cllient\n";
                node->call();
            }
            this->d_list = nullptr;
        }
        ::std::cout << "stopping stop state: done\n";
    }
    auto insert(callback_base* callback) -> void {
        ::std::cout << "inserting callback\n";
        for (callback_base* head = this->d_list; head; ) {
            callback->d_next = head;
            if (   head != &this->d_marker
                && this->d_list.compare_exchange_strong(head, callback)) {
                ::std::cout << "inserting callback done\n";
                return;
            }
        }
        ::std::cout << "inserting callback into stopped state: calling callback instead\n";
        callback->call();
    }
    auto remove(callback_base* callback) -> void {
        ::std::cout << "removing callback\n";
        if (callback->d_called) {
            ::std::cout << "removing callback which was called: do nothing\n";
            return;
        }
        callback_base* head = this->d_list;
        for (; ; head = this->d_list) {
            if (head == nullptr) {
                ::std::cout << "removing callback from stopped list: done\n";
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
            callback_base* node = head;
            while (node && node->d_next != callback) {
                node = node->d_next;
            }
            if (node) {
                // node->d_next must be non-null because the last node is &this->d_end
                callback_base* next = node->d_next;
                callback_base* keep = next->d_next;
                node->d_next = keep;
            }
        }

        this->d_list = head;
        ::std::cout << "removing callback done\n";
    }
};

// ----------------------------------------------------------------------------

class nstd::stop_token::in_place_stop_token
{
    friend class ::nstd::stop_token::in_place_stop_source;
private:
    ::nstd::stop_token::in_place_stop_source* d_state;

    explicit in_place_stop_token(::nstd::stop_token::in_place_stop_source* state)
        : d_state(state)
    {
    }
public:
    template <typename Callable>
    struct callback_type
        : ::nstd::stop_token::in_place_stop_source::callback_base
    {
        in_place_stop_source* d_state;
        Callable            d_callback;
        callback_type(in_place_stop_token token, Callable callback)
            : d_state(token.d_state)
            , d_callback(::nstd::utility::move(callback)) {
            std::cout << "constructing callback_type\n";
            this->d_state->insert(this);
            std::cout << "constructing callback_type: done\n";
        }
        callback_type(callback_type const&) = delete;
        auto operator= (callback_type const&) -> callback_type = delete;
        ~callback_type() {
            std::cout << "destroying callback_type\n";
            this->d_state->remove(this);
            std::cout << "destroying callback_type: done\n";
        }
        auto do_call() ->void override { this->d_callback(); }
    };
    auto operator== (in_place_stop_token const&) const noexcept -> bool = default;
    auto stop_requested() const noexcept -> bool { return this->d_state->stop_requested(); }
    constexpr auto stop_possible() const noexcept -> bool { return true; }
};

static_assert(::nstd::stop_token::stoppable_token<::nstd::stop_token::in_place_stop_token>);

// ----------------------------------------------------------------------------

inline auto nstd::stop_token::in_place_stop_source::token()
    -> ::nstd::stop_token::in_place_stop_token
{
    return ::nstd::stop_token::in_place_stop_token(this);
}

// ----------------------------------------------------------------------------

#endif
