// nstd/file/file.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_FILE
#define INCLUDED_NSTD_FILE_FILE

#include "nstd/file/open_flags.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/sender/then.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include <string_view>
#include <exception>
#include <system_error>
#include <chrono> //-dk:TODO remove
#include <thread> //-dk:TODO remove
#include <linux/io_uring.h>
#include <fcntl.h>

// ----------------------------------------------------------------------------

namespace nstd::file {
    template <typename, typename> struct open_sender;

    template <typename Context>
    auto open(Context& context, ::nstd::file::open_flags flags) {
        return [ctxt=&context, flags](auto&& sender){
            return ::nstd::file::open_sender<Context, decltype(sender)>{
                    {}, std::forward<decltype(sender)>(sender), ctxt, flags
                };
        };
    }
    auto open_in(auto& context) { return ::nstd::file::open(context, ::nstd::file::open_flags::in); }
    auto open_out(auto& context) { return ::nstd::file::open(context, ::nstd::file::open_flags::out); }
    auto open_inout(auto& context) { return ::nstd::file::open(context, ::nstd::file::open_flags::inout); }
}

// ----------------------------------------------------------------------------

template <typename Context, typename Sender>
struct nstd::file::open_sender
    : ::nstd::execution::sender_base
{
    Sender                   d_sender;
    Context*                 d_context;
    ::nstd::file::open_flags d_flags;

    struct foo {};
    template <typename Receiver>
    struct receiver
        : ::nstd::net::io_context::io_base
    {
        Context*                 d_context;
        ::nstd::file::open_flags d_flags;
        Receiver                 d_receiver;
        ::std::string            d_name;

        template <typename R>
        receiver(Context* context, ::nstd::file::open_flags flags, R&& receiver)
            : ::nstd::net::io_context::io_base()
            , d_context(context)
            , d_flags(flags)
            , d_receiver(::nstd::utility::forward<R>(receiver))
            , d_name()
        {

        }

        auto do_result(::std::int32_t res, ::uint32_t) -> void override {
            if (-1 <  res) {
                ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver), res);
            }
            else {
                ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver),
                                             ::std::make_exception_ptr(::std::system_error(-res, ::std::system_category())));
            }
        }

        auto set_value(::std::string_view name) && noexcept -> void {
            this->d_name = name;
            this->d_context->submit([this](io_uring_sqe& elem){
                elem = io_uring_sqe{
                    .opcode     = IORING_OP_OPENAT,
                    .flags      = {},
                    .ioprio     = {},
                    .fd         = AT_FDCWD,
                    .off        = {},
                    .addr       = reinterpret_cast<decltype(elem.addr)>(this->d_name.c_str()),
                    .len        = 0,
                    .open_flags = ::nstd::file::to_system(this->d_flags),
                    .user_data  = reinterpret_cast<decltype(elem.user_data)>(static_cast<::nstd::net::io_context::io_base*>(this)),
                    .__pad2     = {}
                };

            });
        }
        template <typename Error>
        auto set_error(Error&& error) && noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver), ::nstd::utility::forward<Error>(error));
        }
        auto set_done() && noexcept -> void {
            ::nstd::execution::set_done(::nstd::utility::move(this->d_receiver));
        }
    };

    struct state {
        auto start() noexcept -> void;
    };

    template <typename Receiver>
    auto connect(Receiver&& r) noexcept {
        return ::nstd::execution::connect(::nstd::utility::move(this->d_sender),
                                          receiver<Receiver>(this->d_context, this->d_flags, ::nstd::utility::forward<Receiver>(r)));
    }
};

// ----------------------------------------------------------------------------

#endif
