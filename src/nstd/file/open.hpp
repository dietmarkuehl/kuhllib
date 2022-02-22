// nstd/file/open.hpp                                                 -*-C++-*-
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

#include "nstd/file/descriptor.hpp"
#include "nstd/file/open_flags.hpp"
#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/file/context.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include <string_view>
#include <exception>
#include <system_error>
#include <chrono> //-dk:TODO remove
#include <thread> //-dk:TODO remove
#include <fcntl.h>

#ifdef _MSC_VER
inline constexpr int AT_FDCWD = 0;
#endif

// ----------------------------------------------------------------------------

namespace nstd::file {
    template <typename> struct open_sender;

    template <typename Context>
    auto open(Context& context, ::nstd::file::open_flags flags) {
        return [ctxt=&context, flags](auto&& sender){
            return ::nstd::file::open_sender<decltype(sender)>{
                    {}, std::forward<decltype(sender)>(sender), ctxt->hidden_context(), flags
                };
        };
    }
    auto open_in(auto& context) { return ::nstd::file::open(context, ::nstd::file::open_flags::in); }
    auto open_out(auto& context) { return ::nstd::file::open(context, ::nstd::file::open_flags::out); }
    auto open_inout(auto& context) { return ::nstd::file::open(context, ::nstd::file::open_flags::inout); }
}

// ----------------------------------------------------------------------------

template <typename Sender>
struct nstd::file::open_sender
    : ::nstd::execution::sender_base
{
    using completion_signatures
        = ::nstd::execution::completion_signatures<
            ::nstd::execution::set_value_t(::nstd::file::descriptor),
            ::nstd::execution::set_error_t(::std::error_code)
            >;
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<::nstd::file::descriptor>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr, std::error_code>;
    static constexpr bool sends_done = true;

    ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
    ::nstd::file::context*                      d_context;
    ::nstd::file::open_flags                    d_flags;

    template <typename Receiver>
    struct receiver
        : ::nstd::file::context::io_base
    {
        ::nstd::file::context*                        d_context;
        ::nstd::file::open_flags                      d_flags;
        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        ::std::string                                 d_name;

        template <typename R>
        receiver(::nstd::file::context* context, ::nstd::file::open_flags flags, R&& receiver)
            : ::nstd::file::context::io_base()
            , d_context(context)
            , d_flags(flags)
            , d_receiver(::nstd::utility::forward<R>(receiver))
            , d_name()
        {
        }
        receiver(receiver const&) = delete;
        receiver(receiver&&) = default;

        auto do_result(::std::int32_t res, ::uint32_t) -> void override {
            if (-1 <  res) {
                ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver),
                                             ::nstd::file::descriptor(res));
            }
            else {
                ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver),
                                             ::std::error_code(-res, ::std::system_category()));
            }
        }

        friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& r, ::std::string_view name) noexcept -> void {
            r.d_name = name;
            r.d_context->open_at(AT_FDCWD, r.d_name.c_str(), ::nstd::file::to_system(r.d_flags), &r);
        }
        template <typename Error>
        friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& r, Error&& error) noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver), ::nstd::utility::forward<Error>(error));
        }
        friend auto tag_invoke(::nstd::execution::set_done_t, receiver&& r) noexcept -> void {
            ::nstd::execution::set_done(::nstd::utility::move(r.d_receiver));
        }
    };

    template <typename Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, open_sender&& sender, Receiver&& r) noexcept {
        return ::nstd::execution::connect(::nstd::utility::move(sender.d_sender),
                                          receiver<Receiver>(sender.d_context, sender.d_flags, ::nstd::utility::forward<Receiver>(r)));
    }
};

// ----------------------------------------------------------------------------

#endif
