// nstd/file/read.hpp                                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_READ
#define INCLUDED_NSTD_FILE_READ

#include "nstd/file/descriptor.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/file/context.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <system_error>
#include <cstddef>
#include <cstdint>
#ifdef _MSC_VER
int read(int, char*, int) { return 0; }
#else
#    include <unistd.h>
#    include <sys/uio.h>
#endif
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace nstd::file {
    template <typename Buffer, typename Context, typename Receiver>
    struct read_receiver
        : ::nstd::file::context::io_base
    {
        Buffer                          d_buffer;
        ::iovec                         d_iovec[1];
        Context*                        d_context;
        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        ::nstd::file::descriptor        d_fd;

        read_receiver(Context* context, Receiver&& receiver)
            : ::nstd::file::context::io_base()
            , d_iovec()
            , d_context(context)
            , d_receiver(::nstd::utility::forward<Receiver>(receiver)) {
        }
        read_receiver(read_receiver const&) = delete;
        read_receiver(read_receiver&&) = default;

        auto do_result(::std::int32_t res, ::uint32_t) -> void override {
            ::std::cout << "read::do_result(res=" << res << ", ...)\n";
            ::std::cout.write(reinterpret_cast<char const*>(+this->d_buffer), res);
            ::std::cout << "\n";
            if (-1 <  res) {
                ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                             this->d_buffer,
                                             ::std::size_t(res));
            }
            else {
                ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver),
                                             ::std::error_code(-res, ::std::system_category()));
            }
        }

        friend auto tag_invoke(::nstd::execution::set_value_t, read_receiver&& r, ::nstd::file::descriptor&& fd) noexcept -> void {
            ::std::cout << "read: rvalue set_value\n";
            r.d_fd = ::nstd::utility::move(fd);
            r.submit(r.d_fd.get());
        }
        friend auto tag_invoke(::nstd::execution::set_value_t, read_receiver&& r, ::nstd::file::descriptor const& fd) noexcept -> void {
            ::std::cout << "read: lvalue set_value\n";
            r.submit(fd.get());
        }
        auto submit(int fd) {
            this->d_context->read(fd, this->d_iovec, 1u, this);
            this->d_iovec[0] = ::iovec{ .iov_base = this->d_buffer, .iov_len = sizeof(this->d_buffer) };
            ::std::cout << "buffer=" << +this->d_buffer << " "
                        << "iovec=" << this->d_iovec[0].iov_base << "/" << this->d_iovec[0].iov_len << "\n";
        }
        friend auto tag_invoke(::nstd::execution::set_error_t, read_receiver&& r, auto&& err) noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver),
                                         ::nstd::utility::forward<decltype(err)>(err));
        }
        friend auto tag_invoke(::nstd::execution::set_stopped_t, read_receiver&& r) noexcept -> void {
            ::nstd::execution::set_stopped(::nstd::utility::move(r.d_receiver));
        }
    };

    template <typename Buffer, typename Context, typename Sender>
    struct read_sender
    {
    using completion_signatures
        = ::nstd::execution::completion_signatures<
            ::nstd::execution::set_value_t(::nstd::file::descriptor, ::std::size_t)
            >;
        template <template <typename...> class T, template <typename...> class V>
        using value_types = V<T<::nstd::file::descriptor, ::std::size_t>>;
        template <template <typename...> class V>
        using error_types = V<::std::exception_ptr>;
        static constexpr bool sends_done = true;

        Context*                       d_context;
        ::nstd::type_traits::remove_cvref_t<Sender>  d_sender;

        template <typename Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, read_sender&& sender, Receiver&& receiver) noexcept {
            return ::nstd::execution::connect(::nstd::utility::move(sender.d_sender),
                                              ::nstd::file::read_receiver<Buffer, Context, Receiver>(
                                                sender.d_context,
                                                ::nstd::utility::forward<Receiver>(receiver)
                                              ));
        }
    };

    template <::std::size_t N, typename Context>
    auto read(Context& ctxt)
    {
        return [&ctxt](auto&& sender){
            return ::nstd::file::read_sender<::std::byte[N], Context, decltype(sender)>{
                {}, &ctxt, ::nstd::utility::forward<decltype(sender)>(sender)
            };
        };
    }
}

// ----------------------------------------------------------------------------

#endif
