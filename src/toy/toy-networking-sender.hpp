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

#include "toy-socket.hpp"
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
    template <typename Operation, typename Stream = toy::socket>
    struct sender {
        using result_t = typename Operation::result_t;

        Stream&   stream;
        Operation op;

        template <typename R>
        using scheduler_t = decltype(get_scheduler(std::declval<R const&>()));
        template <typename R>
        using state_t     = typename scheduler_t<R>::template io_state<R, Operation, Stream>;

        template <typename R>
        friend state_t<R> connect(sender const& self, R&& receiver) {
            return state_t<R>(std::forward<R>(receiver), self.stream, self.op.event, self.op);
        }
        template <typename R>
        friend state_t<R> connect(sender&& self, R&& receiver) {
            return state_t<R>(std::forward<R>(receiver), self.stream, self.op.event, std::move(self.op));
        }
    };

    template <typename Operation, typename Stream = toy::socket>
    struct incomplete_sender
    {
        Stream&   stream;

        template <typename UpstreamSender>
        struct sender
        {
            using args_t = typename Operation::template args_t<typename UpstreamSender::result_t>;
            using result_t = typename args_t::result_t;

            template <typename Receiver>
            struct state
            {
                template <typename R>
                using scheduler_t = decltype(get_scheduler(std::declval<R const&>()));
                template <typename R>
                using state_t     = typename scheduler_t<R>::template io_state<R, args_t, Stream>;

                struct own_receiver
                {
                    using stop_token_t = decltype(get_stop_token(std::declval<Receiver>()));
                    state* d_state;
                    template <typename Value>
                    friend void set_value(own_receiver self, Value&& value) {
                        self.d_state->own_state.emplace(
                            std::move(self.d_state->receiver), self.d_state->stream, args_t::event,
                            Operation::make_args(value)
                            );
                        start(*self.d_state->own_state);
                    }
                    template <typename Error>
                    friend void set_error(own_receiver self, Error&& error) { set_error(std::move(self.receiver), std::forward<Error>(error)); }
                    template <typename Tag = bool>
                    friend void set_stopped(own_receiver self, Tag = true) { set_stopped(std::move(self.d_state->receiver)); }
                    template <typename Tag = bool>
                    friend stop_token_t get_stop_token(own_receiver const& self, Tag = true) { return get_stop_token(self.d_state->receiver); }
                };
                using upstream_state_t = decltype(connect(std::declval<UpstreamSender>(), std::declval<own_receiver>()));

                Stream&                          stream;
                Receiver                         receiver;
                upstream_state_t                 upstream_state;
                std::optional<args_t>            args;
                std::optional<state_t<Receiver>> own_state;
                template <typename Upstream>
                state(Stream& stream, Upstream&& upstream_sender, Receiver&& receiver)
                    : stream(stream)
                    , receiver(std::forward<Receiver>(receiver))
                    , upstream_state(connect(std::forward<Upstream>(upstream_sender), own_receiver(this)))
                {
                }
                friend void start(state& self) { start(self.upstream_state); }
            };
            Stream&        stream;
            UpstreamSender upstream_sender;

            template <typename Receiver>
            friend state<Receiver> connect(sender const& self, Receiver&& receiver)
            {
                return state<Receiver>(self.stream, self.upstream_sender, std::forward<Receiver>(receiver));
            }
            template <typename Receiver>
            friend state<Receiver> connect(sender&& self, Receiver&& receiver)
            {
                return state<Receiver>(self.stream, std::move(self.upstream_sender), std::forward<Receiver>(receiver));
            }
        };
        template <typename UpstreamSender>
        sender<UpstreamSender> operator()(UpstreamSender&& sender) { return { this->stream, std::forward<UpstreamSender>(sender) }; }
    };
}

// ----------------------------------------------------------------------------

namespace io
{
    struct poll_t
    {
        struct args
        {
            using result_t = toy::event_kind;
            toy::event_kind event{};
            static constexpr char const* name = "poll";
        };

        toy::hidden::io_operation::sender<toy::io::poll_t::args>
        inline operator()(toy::socket& s, toy::event_kind events) const
        {
            return {s, { events }};
        }
    };

    struct connect_t
    {
        struct args
        {
            using result_t = int;
            static constexpr toy::event_kind event = toy::event_kind::write;
            static constexpr char const* name = "connect";

            toy::address address;
        };

        inline hidden::io_operation::sender<toy::io::connect_t::args>
        operator()(toy::socket& s, toy::address addr) const
        {
            return {s, { addr }};
        }
    };

    struct accept_t
    {
        struct args
        {
            using result_t = toy::socket;
            static constexpr toy::event_kind event = toy::event_kind::read;
            static constexpr char const* name = "accept";
        };
        inline hidden::io_operation::sender<toy::io::accept_t::args>
        operator()(toy::socket& s) const
        {
            return {s, {}};
        }
    };

    struct read_some_t
    {
        struct args {
            using result_t = int;
            static constexpr toy::event_kind event = toy::event_kind::read;
            static constexpr char const* name = "read_some";

            char*       buffer;
            std::size_t len;
        };

        inline hidden::io_operation::sender<toy::io::read_some_t::args>
        operator()(toy::socket& s, char* buffer, std::size_t len) const
        {
            return {s, {buffer, len}};
        }
        inline hidden::io_operation::sender<toy::io::read_some_t::args, toy::file>
        operator()(toy::file& f, char* buffer, std::size_t len) const
        {
            return {f, {buffer, len}};
        }
    };

    struct write_some_t
    {
        struct args
        {
            using result_t = int;
            static constexpr toy::event_kind event = toy::event_kind::write;
            static constexpr char const* name = "write_some";

            char const* buffer;
            std::size_t len;
        };
        inline hidden::io_operation::sender<toy::io::write_some_t::args>
        operator()(toy::socket& s, char const* buffer, std::size_t len) const {
            return {s, {buffer, len}};
        }
    };

    struct receive_t
    {
        template <typename MBS>
        struct args
        {
            using result_t = int;
            static constexpr toy::event_kind event = toy::event_kind::read;
            static constexpr char const* name = "receive";

            toy::message_flags flags;
            MBS                buffer;
        };
        template <typename MBS>
        using args_t = args<MBS>;

        template <typename MBS>
        static args<MBS> make_args(const MBS& b) { return args<MBS>{ toy::message_flags(), b }; }

        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::receive_t::args<MBS>>
        operator()(toy::socket& s, toy::message_flags f, const MBS& b) const
        {
            return {s, {f, b}};
        }
        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::receive_t::args<MBS>>
        operator()(toy::socket& s, const MBS& b) const
        {
            return {s, {toy::message_flags{}, b}};
        }
        inline hidden::io_operation::incomplete_sender<toy::io::receive_t>
        operator()(toy::socket& s) const{
            return { s };
        }
    };

    struct receive_from_t
    {
        template <typename MBS>
        struct args
        {
            using result_t = std::size_t;
            static constexpr toy::event_kind event = toy::event_kind::read;
            static constexpr char const* name = "receive_from";

            MBS                buffer;
            toy::address&      addr;
            toy::message_flags flags;
        };
        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::receive_from_t::args<MBS>>
        operator()(toy::socket& s, const MBS& b, toy::address& addr, toy::message_flags f) const {
            return {s, {b, addr, f}};
        }
        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::receive_from_t::args<MBS>>
        operator()(toy::socket& s, const MBS& b, toy::address& addr) const {
            return {s, {b, addr, toy::message_flags{}}};
        }
    };

    struct send_t
    {
        template <typename MBS>
        struct args
        {
            using result_t = std::size_t;
            static constexpr toy::event_kind event = toy::event_kind::write;
            static constexpr char const* name = "send";

            toy::message_flags flags;
            MBS                buffer;
        };

        template <typename MBS>
        using args_t = args<MBS>;

        template <typename MBS>
        static args<MBS> make_args(const MBS& b) { return args<MBS>{ toy::message_flags(), b }; }

        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::send_t::args<MBS>>
        operator()(toy::socket& s, toy::message_flags f, const MBS& b) const {
            return {s, {f, b}};
        }
        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::send_t::args<MBS>>
        operator()(toy::socket& s, const MBS& b) const {
            return {s, {toy::message_flags{}, b}};
        }
        inline hidden::io_operation::incomplete_sender<toy::io::send_t>
        operator()(toy::socket& s) const{
            return { s };
        }
    };

    struct send_to_t
    {
        template <typename MBS>
        struct args
        {
            using result_t = std::size_t;
            static constexpr toy::event_kind event = toy::event_kind::write;
            static constexpr char const* name = "send_to";

            MBS                buffer;
            toy::address       address;
            toy::message_flags flags;
        };
        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::send_to_t::args<MBS>>
        operator()(toy::socket& s, const MBS& b, toy::address addr, toy::message_flags f) const {
            return {s, {b, addr, f}};
        }
        template <typename MBS>
        inline hidden::io_operation::sender<toy::io::send_to_t::args<MBS>>
        operator()(toy::socket& s, const MBS& b, toy::address addr) const {
            return {s, {b, addr, toy::message_flags{}}};
        }
    };
}

using poll_t         = toy::io::poll_t;
using connect_t      = toy::io::connect_t;
using accept_t       = toy::io::accept_t;
using read_some_t    = toy::io::read_some_t;
using write_some_t   = toy::io::write_some_t;
using receive_t      = toy::io::receive_t;
using receive_from_t = toy::io::receive_from_t;
using send_t         = toy::io::send_t;
using send_to_t      = toy::io::send_to_t;

inline constexpr toy::poll_t             async_poll{};
inline constexpr toy::io::connect_t      async_connect{};
inline constexpr toy::io::accept_t       async_accept{};
inline constexpr toy::io::read_some_t    async_read_some{};
inline constexpr toy::io::write_some_t   async_write_some{};
inline constexpr toy::io::receive_t      async_receive{};
inline constexpr toy::io::receive_from_t async_receive_from{};
inline constexpr toy::io::send_t         async_send{};
inline constexpr toy::io::send_to_t      async_send_to{};

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
        using state_t     = typename scheduler_t<R>::template time_state<R, Operation>;

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
