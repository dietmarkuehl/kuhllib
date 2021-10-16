// nstd/execution/start_detached.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_START_DETACHED
#define INCLUDED_NSTD_EXECUTION_START_DETACHED

#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <concepts>
#include <exception>
#include <memory>
#include <iostream>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct start_detached_t
    {
        struct holder_base;
        template <::nstd::execution::operation_state> struct holder;
        struct receiver;

        template <::nstd::execution::sender Sender>
            requires requires(Sender&& sender) {
                {
                    ::nstd::tag_invoke(::nstd::type_traits::declval<start_detached_t>(),
                                       ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                       ::nstd::utility::forward<Sender>(sender))
                } -> ::std::same_as<void>;
            }
        auto operator()(Sender&& sender) const -> void {
            auto scheduler{::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender)};
            ::nstd::tag_invoke(*this,
                               ::nstd::utility::move(scheduler),
                               ::nstd::utility::forward<Sender>(sender));
        }
        template <::nstd::execution::sender Sender>
            requires requires(Sender&& sender) {
                {
                    ::nstd::tag_invoke(::nstd::type_traits::declval<start_detached_t>(),
                                       ::nstd::utility::forward<Sender>(sender))
                } -> ::std::same_as<void>;
            }
            && (not requires(Sender&& sender) {
                {
                    ::nstd::tag_invoke(::nstd::type_traits::declval<start_detached_t>(),
                                       ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                       ::nstd::utility::forward<Sender>(sender))
                } -> ::std::same_as<void>;
            })
        auto operator()(Sender&& sender) const -> void {
            ::nstd::tag_invoke(*this, ::nstd::utility::forward<Sender>(sender));
        }

        template <::nstd::execution::sender Sender>
        auto operator()(Sender&& ) const -> void;
    } start_detached;
}

// ----------------------------------------------------------------------------

struct nstd::execution::start_detached_t::holder_base
{
    virtual ~holder_base() = default;
};

template <::nstd::execution::operation_state State>
struct nstd::execution::start_detached_t::holder
    : nstd::execution::start_detached_t::holder_base
{
    ::nstd::type_traits::remove_cvref_t<State> d_state;
    template <::nstd::execution::sender S, ::nstd::execution::receiver R>
    holder(S&& sender, R&& receiver)
        : d_state(::nstd::execution::connect(::nstd::utility::forward<S>(sender),
                                             ::nstd::utility::forward<R>(receiver)))
    {
    }
};

// ----------------------------------------------------------------------------

struct nstd::execution::start_detached_t::receiver
{
    ::std::unique_ptr<holder_base>**  d_backpointer;
    ::std::unique_ptr<holder_base>    d_state;

    receiver(::std::unique_ptr<holder_base>** backpointer): d_backpointer(backpointer) {} 
    receiver(receiver&& r)
        : d_backpointer(r.d_backpointer)
        , d_state(::nstd::utility::move(r.d_state))
        {
        if (!this->d_state) {
            *this->d_backpointer = &this->d_state;
        }
    } 

    friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& r, auto&&...) noexcept -> void{
        r.d_state.reset();
    }
    friend auto tag_invoke(::nstd::execution::set_done_t, receiver&& r) noexcept -> void {
        r.d_state.reset();
    }
    friend auto tag_invoke(::nstd::execution::set_error_t, receiver&&, auto&&) noexcept -> void {
        ::std::terminate();
    }
};

static_assert(::nstd::execution::receiver<nstd::execution::start_detached_t::receiver>);

// ----------------------------------------------------------------------------


template <::nstd::execution::sender Sender>
auto nstd::execution::start_detached_t::operator()(Sender&& sender) const -> void
{
    using State = decltype(::nstd::execution::connect(::nstd::utility::forward<Sender>(sender),
                                                      receiver(nullptr)));
    ::std::unique_ptr<holder_base>* state = nullptr;
    holder<State>* tmp(new holder<State>(::nstd::utility::forward<Sender>(sender),
                                       receiver(&state)));
    state->reset(tmp);
    ::nstd::execution::start(tmp->d_state);
}

// ----------------------------------------------------------------------------

#endif
