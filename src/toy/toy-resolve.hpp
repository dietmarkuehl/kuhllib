// toy-resolve.hpp                                                    -*-C++-*-
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

#ifndef INCLUDED_TOY_RESOLVE
#define INCLUDED_TOY_RESOLVE

#include "toy-networking.hpp"
#include "toy-sender.hpp"
#include "toy-task.hpp"
#include <ares.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <utility>

// ----------------------------------------------------------------------------

namespace toy {
    struct resolve {
        struct ares_init
            : toy::immovable
        {
            ares_init()
            {
                if (int rc{ares_library_init(ARES_LIB_INIT_ALL)}; rc != ARES_SUCCESS) {
                    throw std::runtime_error(std::string("failed to init ares library: ") + ares_strerror(rc));
                }
            }
            ~ares_init()
            {
                ares_library_cleanup();
            }
        };
        static void state_callback(void* data, int fd, int in, int out)
        {
            using event_kind = toy::hidden::io_operation::event_kind;
            event_kind events((in? event_kind::read: event_kind::none) | (out? event_kind::write: event_kind::none));
            if (bool(events)) {
                resolver& res(*static_cast<resolver*>(data));
                res.context->spawn([](void* data, int fd, event_kind events)->toy::task<toy::io_context::scheduler> {
                    resolver& res(*static_cast<resolver*>(data));
                    toy::socket sock(*res.context, fd);
                    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
                    do {
                        int rc = co_await toy::async_poll(sock, events);
                        (void)rc;
                        ares_process_fd(res.channel,
                                        //rc & POLL_IN? fd: ARES_SOCKET_BAD,
                                        //rc & POLL_OUT? fd: ARES_SOCKET_BAD);
                                        bool(events & event_kind::read)? fd: ARES_SOCKET_BAD,
                                        bool(events & event_kind::write)? fd: ARES_SOCKET_BAD);

                        int bits = ares_getsock(res.channel, socks, ARES_GETSOCK_MAXNUM);
                        events = event_kind::none;
                        if (bits) {
                            for (int i{}; i != ARES_GETSOCK_MAXNUM; ++i) {
                                if ((ARES_GETSOCK_READABLE(bits, i) || ARES_GETSOCK_WRITABLE(bits, i))
                                    && socks[i] == fd) {
                                    events = (ARES_GETSOCK_READABLE(bits, i)? event_kind::read: event_kind::none)
                                           | (ARES_GETSOCK_WRITABLE(bits, i)? event_kind::write: event_kind::none)
                                           ;
                                }
                            }
                        }
                    }
                    while (events != event_kind::none);
                }(data, fd, events));
            }
        }
        struct resolver {
            toy::io_context* context;
            ares_options     options;
            ares_channel     channel{};

            resolver(toy::io_context* context)
                : context(context)
                , options{ .sock_state_cb = state_callback, .sock_state_cb_data = this }
            {
                int mask(ARES_OPT_SOCK_STATE_CB);
                if (int rc{ares_init_options(&channel, &options, mask)}; rc != ARES_SUCCESS) {
                    throw std::runtime_error(std::string("failed to create ares channel: ") + ares_strerror(rc));
                }
            }
            ~resolver()
            {
                ares_destroy(channel);
            }
        };
        static resolver& get_resolver(auto scheduler)
        {
            static resolver rc(scheduler.context);
            return rc;
        }

        using result_t = toy::address;

        std::string host;
        int         family;
        template <typename Host>
        resolve(Host&& host, int family = AF_INET6)
            : host(std::forward<Host>(host))
            , family(family)
        {
        }

        template <typename Receiver>
        struct state {
            static void result_callback(void *data, int status, int, struct hostent *hosts)
            {
                state& self(*static_cast<state*>(data));
                if (hosts && status == ARES_SUCCESS) {
                    if (hosts->h_addrtype == AF_INET) {
                        sockaddr_in addr{};
                        addr.sin_family = AF_INET;
                        addr.sin_len = sizeof(sockaddr_in);
                        std::memcpy(&addr.sin_addr.s_addr, hosts->h_addr_list[0], sizeof(addr.sin_addr.s_addr));
                        set_value(self.receiver, toy::address(&addr, addr.sin_len));
                    }
                    else if (hosts->h_addrtype == AF_INET6) {
                        sockaddr_in6 addr{};
                        addr.sin6_family = AF_INET6;
                        addr.sin6_len = sizeof(sockaddr_in);
                        std::memcpy(&addr.sin6_addr, hosts->h_addr_list[0], sizeof(addr.sin6_addr));
                        set_value(self.receiver, toy::address(&addr, addr.sin6_len));
                    }
                    else {
                        set_error(self.receiver, std::make_exception_ptr(std::runtime_error("unsupported address family")));
                    }
                }
                else {
                    set_error(self.receiver, std::make_exception_ptr(std::runtime_error(std::string("host look-up failed: ") + ares_strerror(status))));
                }
            }
            Receiver    receiver;
            std::string host;
            int         family;
            friend void start(state& self) {
                auto scheduler(get_scheduler(self.receiver));
                resolver& res(get_resolver(scheduler));
                ares_gethostbyname(res.channel, self.host.c_str(), self.family, result_callback, &self);
                ares_process_fd(res.channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
            }
        };

        template <typename Receiver>
        friend state<Receiver> connect(resolve const& self, Receiver receiver) {
            static ares_init init;
            return state<Receiver>{receiver, self.host, self.family};
        }
    };
}

// ----------------------------------------------------------------------------

#endif
