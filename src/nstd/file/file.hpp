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
#include "nstd/sender/then.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include <string_view>
#include <chrono> //-dk:TODO remove
#include <thread> //-dk:TODO remove

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

    template <typename Receiver>
    struct receiver {
        Receiver d_receiver;

        auto set_value(::std::string_view&&...) && noexcept -> void {
            ::std::thread thread([this]{
                using namespace ::std::chrono_literals;
                ::std::this_thread::sleep_for(2000ms);
                ::nstd::utility::move(this->d_receiver).set_value();
            });
            thread.detach();
        }
        template <typename Error>
        auto set_error(Error&& error) && noexcept -> void {
            ::nstd::utility::move(this->d_receiver).set_error(::nstd::utility::forward<Error>(error));
        }
        auto set_done() && noexcept -> void {
            ::nstd::utility::move(this->d_receiver).set_done();
        }
    };

    struct state {
        auto start() noexcept -> void;
    };

    template <typename Receiver>
    auto connect(Receiver&& r) noexcept {
        return ::nstd::execution::connect(::nstd::utility::move(this->d_sender),
                                          receiver{::nstd::utility::forward<Receiver>(r)});
    }
};

// ----------------------------------------------------------------------------

#endif
