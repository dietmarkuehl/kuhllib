// toy-networking-io_uring.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_TOY_NETWORKING_IO_URING
#define INCLUDED_TOY_NETWORKING_IO_URING

#include "toy-networking-common.hpp"
#include "toy-networking-posix.hpp"
#include "toy-starter.hpp"
#include "toy-utility.hpp"

#include <chrono>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <liburing.h>

namespace toy::io_uring
{

class context;
class scheduler;

// ----------------------------------------------------------------------------

struct io_base
    : immovable
{
    virtual ~io_base() = default;
    virtual std::size_t complete(int) = 0;
    std::size_t nop() { return 0; }
};

struct cancel_state
    : toy::io_uring::io_base
{
    toy::io_uring::io_base* state{nullptr};
    std::optional<int>      rc;
    std::size_t complete(int) override final {
        rc
            ? std::exchange(state, nullptr)->complete(*rc)
            : std::exchange(state, nullptr)->nop()
            ;
        return 1;
    }
};

template <typename>
struct storage_type
{
    using type = toy::none;
};
template <typename Operation>
using storage_t = typename storage_type<Operation>::type;

inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::poll_op& op, toy::none)
{
    ::io_uring_prep_poll_add(sqe, fd,
                              (bool(op.event & toy::event_kind::read)? POLLIN: 0)
                            | (bool(op.event & toy::event_kind::write)? POLLOUT: 0));
}

inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::connect_op& op, toy::none const&)
{
    ::io_uring_prep_connect(sqe, fd, &op.address.as_addr(), op.address.size());
}

template <>
struct storage_type<toy::hidden::io_operation::accept_op> { using type = toy::address; };
inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::accept_op&, toy::address& address)
{
    ::io_uring_prep_accept(sqe, fd, &address.as_addr(), &(address.len = address.capacity()), 0);
}

inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::read_some_op& op, toy::none const&)
{
    ::io_uring_prep_read(sqe, fd, op.buffer, op.len, 0);
}

inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::write_some_op& op, toy::none const&)
{
    ::io_uring_prep_write(sqe, fd, op.buffer, op.len, 0);
}

template <typename MBS>
struct storage_type<toy::hidden::io_operation::receive_op<MBS>> { using type = ::msghdr; };
template <typename MBS>
inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::receive_op<MBS>& op, ::msghdr& header)
{
    header.msg_iov    = op.buffer.data();
    header.msg_iovlen = op.buffer.size();
    ::io_uring_prep_recvmsg(sqe, fd, &header, int(op.flags));
}

template <typename MBS>
struct storage_type<toy::hidden::io_operation::receive_from_op<MBS>> { using type = ::msghdr; };
template <typename MBS>
inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::receive_from_op<MBS>& op, ::msghdr& header)
{
    header.msg_name = &op.addr.as_addr();
    header.msg_namelen = op.addr.size();
    header.msg_iov = op.buffer.data();
    header.msg_iovlen = op.buffer.size();
    ::io_uring_prep_recvmsg(sqe, fd, &header, static_cast<unsigned int>(op.flags));
}
template <typename MBS>
inline void finalize(toy::hidden::io_operation::receive_from_op<MBS>& op, ::msghdr& header)
{
    op.addr.resize(header.msg_namelen);
}

template <typename MBS>
struct storage_type<toy::hidden::io_operation::send_op<MBS>> { using type = ::msghdr; };
template <typename MBS>
inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::send_op<MBS>& op, ::msghdr& header)
{
    header.msg_iov    = op.buffer.data();
    header.msg_iovlen = op.buffer.size();
    ::io_uring_prep_sendmsg(sqe, fd, &header, int(op.flags));
}

template <typename MBS>
struct storage_type<toy::hidden::io_operation::send_to_op<MBS>> { using type = ::msghdr; };
template <typename MBS>
inline void prepare(::io_uring_sqe* sqe, int fd, toy::hidden::io_operation::send_to_op<MBS>& op, ::msghdr& header)
{
    header.msg_name = &op.address.as_addr();
    header.msg_namelen = op.address.size();
    header.msg_iov = op.buffer.data();
    header.msg_iovlen = op.buffer.size();
    ::io_uring_prep_sendmsg(sqe, fd, &header, static_cast<unsigned int>(op.flags));
}

// ----------------------------------------------------------------------------

template <typename Receiver, typename Operation>
struct io_state
    : toy::io_uring::io_base
{
    using scheduler_t  = decltype(get_scheduler(std::declval<Receiver const&>()));
    using stop_token_t = decltype(get_stop_token(std::declval<Receiver const&>()));
    using result_t = Operation::result_t;

    struct cancel_callback
    {
        io_state& s;
        void operator()() {
            s.cancel.state = &s;
            scheduler_t scheduler = s.scheduler();
            
            ::io_uring_sqe* sqe{ scheduler.get_sqe() };
            io_uring_prep_cancel(sqe, &s, unsigned());
            sqe->user_data = reinterpret_cast<std::uint64_t>(&s.cancel);
            scheduler.submit();
        }
    };
    Receiver             receiver;
    toy::socket&         socket;
    Operation            operation;
    storage_t<Operation> storage{};
    cancel_state         cancel;
    std::optional<typename stop_token_t::template callback_type<cancel_callback>> cb;
    
    scheduler_t scheduler() { return get_scheduler(receiver); }

    io_state(Receiver receiver, toy::socket& socket, toy::event_kind, Operation operation)
        : io_base()
        , receiver(receiver)
        , socket(socket)
        , operation(operation)
    {
    }

    friend void start(io_state& self)
    {
        self.cb.emplace(get_stop_token(self.receiver), cancel_callback{self});
        ::io_uring_sqe* sqe{self.scheduler().get_sqe()};
        toy::io_uring::prepare(sqe, self.socket.fd(), self.operation, self.storage);
        sqe->user_data = reinterpret_cast<std::uint64_t>(&self);
        self.scheduler().submit();
    }
    std::size_t complete(int res) override
    {
        cb.reset();
        if (cancel.state) {
            cancel.rc = res;
            return 0u;
        }
        else if (0 <= res) {
            using event_kind = toy::event_kind;
            if constexpr (requires(Operation const& op, toy::io_uring::storage_t<Operation>& s) { finalize(op, s); })
            {
                finalize(operation, storage);
            }
            if constexpr (std::same_as<event_kind, result_t>) {
                event_kind event(std::invoke([res]{
                    switch (res & (POLLIN | POLLOUT)) {
                        default: return event_kind::none;
                        case POLLIN: return event_kind::read;
                        case POLLOUT: return event_kind::write;
                        case POLLIN | POLLOUT: return event_kind::both;
                    }
                }));
                set_value(std::move(receiver), event);
            }
            else if constexpr (requires(toy::io_context_base& context, decltype(res) res){ result_t(context, res); }) {
                set_value(std::move(receiver), result_t(scheduler().base(), res));
            }
            else {
                set_value(std::move(receiver), result_t(res));
            }
        }
        else if (-res == ECANCELED) {
            set_stopped(std::move(receiver));
        }
        else  {
            set_error(std::move(receiver), std::make_exception_ptr(std::system_error(-res, std::system_category())));
        }
        return 1;
    }
};

template <typename Receiver, typename Operation>
struct time_state
    : toy::io_uring::io_base
{
    using scheduler_t  = decltype(get_scheduler(std::declval<Receiver const&>()));
    using stop_token_t = decltype(get_stop_token(std::declval<Receiver const&>()));
    using result_t = Operation::result_t;

    struct cancel_callback
    {
        time_state& s;
        void operator()() {
            s.cancel.state = &s;
            scheduler_t scheduler = s.scheduler();
            
            ::io_uring_sqe* sqe{ scheduler.get_sqe() };
            io_uring_prep_cancel(sqe, &s, unsigned());
            sqe->user_data = reinterpret_cast<std::uint64_t>(&s.cancel);
            scheduler.submit();
        }
    };
    Receiver          receiver;
    Operation         operation;
    __kernel_timespec spec;
    cancel_state      cancel;
    std::optional<typename stop_token_t::template callback_type<cancel_callback>> cb;
    
    scheduler_t scheduler() { return get_scheduler(receiver); }

    time_state(Receiver receiver, Operation operation)
        : io_base()
        , receiver(receiver)
        , operation(operation)
    {
    }

    friend void start(time_state& self)
    {
        std::chrono::milliseconds duration(std::chrono::duration_cast<std::chrono::milliseconds>(self.operation.get_timepoint() - std::chrono::system_clock::now()));
        self.cb.emplace(get_stop_token(self.receiver), cancel_callback{self});
        ::io_uring_sqe* sqe{self.scheduler().get_sqe()};
        self.spec.tv_sec = duration.count() / 1000;
        self.spec.tv_nsec = (duration.count() % 1000) * 1000000;
        io_uring_prep_timeout(sqe, &self.spec, ~unsigned(), unsigned());
        sqe->user_data = reinterpret_cast<std::uint64_t>(&self);
        self.scheduler().submit();
    }
    std::size_t complete(int res) override
    {
        cb.reset();
        if (cancel.state) {
            cancel.rc = res;
            return 0u;
        }
        else if (-res == ECANCELED) {
            set_stopped(std::move(receiver));
        }
        else {
            set_value(std::move(receiver), none{});
        }
        return 1u;
    }
};

class scheduler {
private:
    friend class context;

    toy::io_uring::context* context;

    explicit scheduler(toy::io_uring::context* context): context(context) {}
public:
    template <typename Receiver, typename Operation>
    using io_state = toy::io_uring::io_state<Receiver, Operation>;
    template <typename Receiver, typename Operation>
    using time_state = toy::io_uring::time_state<Receiver, Operation>;

    toy::io_context_base& base() const;
    ::io_uring_sqe* get_sqe();
    void submit();

    template <typename Sender>
    void spawn(Sender&& sender);
};

class context
    : public starter<scheduler>
    , public toy::io_context_base
{
public:
    using scheduler = toy::io_uring::scheduler;
    scheduler get_scheduler() { return scheduler(this); }

    static constexpr std::size_t entries{256};

    ::io_uring  ring;
    std::size_t outstanding{};

    context()
        : starter(scheduler{this}) {
        ::io_uring_queue_init(entries, &ring, 0);
    }
    ~context() { ::io_uring_queue_exit(&ring); }

    io_uring_sqe* get_sqe() { return ::io_uring_get_sqe(&ring); }
    void          submit() {
        ::io_uring_submit(&ring); ++outstanding;
    }

    void run() {
        while (outstanding) {
            ::io_uring_cqe* cqe;
            if (::io_uring_wait_cqe(&ring, &cqe))
                throw std::runtime_error("io_uring_wait_cqe");
            outstanding -= reinterpret_cast<io_base*>(cqe->user_data)->complete(cqe->res);
            ::io_uring_cqe_seen(&ring, cqe);
        }
    }
};

inline toy::io_context_base& scheduler::base() const
{
    return *context;
}

inline ::io_uring_sqe* scheduler::get_sqe() { return context->get_sqe(); }
inline void scheduler::submit() { context->submit(); }

template <typename Sender>
inline void scheduler::spawn(Sender&& sender) { context->spawn(std::forward<Sender>(sender)); }

// ----------------------------------------------------------------------------

}

#endif
