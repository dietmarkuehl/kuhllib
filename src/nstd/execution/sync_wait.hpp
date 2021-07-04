// nstd/execution/sync_wait.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_SYNC_WAIT
#define INCLUDED_NSTD_EXECUTION_SYNC_WAIT

#include <exception>
#include <optional>
#include <mutex>
#include <condition_variable>
#include "nstd/execution/connect.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    template <typename Sender> void sync_wait(Sender&& s);
    struct sync_wait_receiver;
}

// ----------------------------------------------------------------------------

struct nstd::execution::sync_wait_receiver
{
public:
    template <typename Sender>
    friend void ::nstd::execution::sync_wait(Sender&& s);

    ::std::mutex*                          bottleneck;
    ::std::condition_variable*             condition;
    bool*                                  flag;
    ::std::optional<::std::exception_ptr>* exception;

    void complete() {
        {
            ::std::lock_guard kerberos(*this->bottleneck);
            *this->flag = true;
        }
        this->condition->notify_one();
    }

public:
    template <typename... A>
    auto set_value(A&&...) && noexcept -> void {
        this->complete();
    }
    auto set_error(::std::error_code const& error) && noexcept -> void {
        *this->exception = ::std::make_exception_ptr(::std::system_error(error.value(), error.category()));
        this->complete();
    }
    template <typename Exception>
    auto set_error(Exception const& error) && noexcept -> void {
        *this->exception = ::std::make_exception_ptr(error);
        this->complete();
    }
    auto set_error(::std::exception_ptr const& ex) && noexcept -> void {
        *this->exception = ex;
        this->complete();
    }
    friend auto tag_invoke(::nstd::execution::set_done_t, sync_wait_receiver&& r) noexcept -> void {
        r.complete();
    }
};

// ----------------------------------------------------------------------------

template <typename Sender>
auto nstd::execution::sync_wait(Sender&& s) -> void
{
    ::std::mutex                          bottleneck;
    ::std::condition_variable             condition;
    bool                                  flag(false);
    ::std::optional<::std::exception_ptr> exception;

    auto operation = ::nstd::execution::connect(::nstd::utility::forward<Sender>(s),
                                 ::nstd::execution::sync_wait_receiver{ &bottleneck, &condition, &flag, &exception });
    operation.start();

    ::std::unique_lock kerberos(bottleneck);
    condition.wait(kerberos, [&flag]{ return flag; });
    if (exception) {
        ::std::rethrow_exception(*exception);
    }
}

// ----------------------------------------------------------------------------

#endif
