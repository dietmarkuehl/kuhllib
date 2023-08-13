// toy-networking-sender.hpp                                          -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_TOY_NETWORKING_SENDER
#define INCLUDED_TOY_NETWORKING_SENDER

#include "toy-utility.hpp"
#include <chrono>

// ----------------------------------------------------------------------------

namespace toy {
    template <typename Receiver>
    using scheduler_t = decltype(get_scheduler(std::declval<Receiver const&>()));
    template <typename Receiver>
    using io_base = typename scheduler_t<Receiver>::io_base;


// ----------------------------------------------------------------------------

namespace hidden::io_operation {
    template <typename Operation>
    struct sender {
        using result_t = typename Operation::result_t;

        toy::socket&  socket;
        Operation     op;

        template <typename R>
        using scheduler_t = decltype(get_scheduler(std::declval<R const&>()));
        template <typename R>
        using state_t     = scheduler_t<R>::template io_state<R, Operation>;

        template <typename R>
        friend state_t<R> connect(sender const& self, R&& receiver) {
            return state_t<R>(std::forward<R>(receiver), self.socket.fd(), self.op.event, self.op);
        }
        template <typename R>
        friend state_t<R> connect(sender&& self, R&& receiver) {
            return state_t<R>(std::forward<R>(receiver), self.socket, self.op.event, std::move(self.op));
        }
    };
}

hidden::io_operation::sender<toy::hidden::io_operation::poll_op>
async_poll(toy::socket& s, toy::event_kind events) {
    return {s, { events }};
}

hidden::io_operation::sender<toy::hidden::io_operation::connect_op>
async_connect(toy::socket& s, toy::address addr) {
    return {s, { addr }};
}

hidden::io_operation::sender<toy::hidden::io_operation::accept_op>
async_accept(toy::socket& s) {
    return {s, {}};
}

hidden::io_operation::sender<toy::hidden::io_operation::read_some_op>
async_read_some(toy::socket& s, char* buffer, std::size_t len) {
    return {s, {buffer, len}};
}

hidden::io_operation::sender<toy::hidden::io_operation::write_some_op>
async_write_some(toy::socket& s, char const* buffer, std::size_t len) {
    return {s, {buffer, len}};
}

template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::receive_op<MBS>>
async_receive(toy::socket& s, toy::message_flags f, const MBS& b) {
    return {s, {f, b}};
}
template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::receive_op<MBS>>
async_receive(toy::socket& s, const MBS& b) {
    return {s, {toy::message_flags{}, b}};
}

template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::receive_from_op<MBS>>
async_receive_from(toy::socket& s, const MBS& b, toy::address& addr, toy::message_flags f) {
    return {s, {b, addr, f}};
}
template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::receive_from_op<MBS>>
async_receive_from(toy::socket& s, const MBS& b, toy::address& addr) {
    return {s, {b, addr, toy::message_flags{}}};
}

template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::send_op<MBS>>
async_send(toy::socket& s, toy::message_flags f, const MBS& b) {
    return {s, {f, b}};
}
template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::send_op<MBS>>
async_send(toy::socket& s, const MBS& b) {
    return {s, {toy::message_flags{}, b}};
}

template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::send_to_op<MBS>>
async_send_to(toy::socket& s, const MBS& b, toy::address addr, toy::message_flags f) {
    return {s, {b, addr, f}};
}
template <typename MBS>
hidden::io_operation::sender<toy::hidden::io_operation::send_to_op<MBS>>
async_send_to(toy::socket& s, const MBS& b, toy::address addr) {
    return {s, {b, addr, toy::message_flags{}}};
}

// ----------------------------------------------------------------------------

namespace hidden::time_operation
{
    template <typename Operation>
    struct sender {
        using result_t = typename Operation::result_t;

        Operation     op;

        template <typename R>
        using scheduler_t = decltype(get_scheduler(std::declval<R const&>()));
        template <typename R>
        using state_t     = scheduler_t<R>::template time_state<R, Operation>;

        template <typename R>
        friend state_t<R> connect(sender const& self, R&& receiver) {
            return state_t<R>(std::forward<R>(receiver), self.op);
        }
        template <typename R>
        friend state_t<R> connect(sender&& self, R&& receiver) {
            return state_t<R>(std::forward<R>(receiver), std::move(self.op));
        }
    };

    struct sleep_for_op
    {
        using result_t = toy::none;

        std::chrono::milliseconds duration;

        std::chrono::time_point<std::chrono::system_clock> get_timepoint()
        {
            return std::chrono::system_clock::now() + duration;
        }
    };

    struct sleep_to_op
    {
        using result_t = toy::none;

        std::chrono::time_point<std::chrono::system_clock> time;

        std::chrono::time_point<std::chrono::system_clock> get_timepoint()
        {
            return time;
        }
    };
}

// ----------------------------------------------------------------------------

hidden::time_operation::sender<toy::hidden::time_operation::sleep_for_op>
async_sleep_for(std::chrono::milliseconds duration) {
    return { { duration }};
}

hidden::time_operation::sender<toy::hidden::time_operation::sleep_to_op>
async_sleep_to(std::chrono::time_point<std::chrono::system_clock> time) {
    return { { time }};
}

// ----------------------------------------------------------------------------

}

#endif
