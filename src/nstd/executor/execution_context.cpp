// nstd/executor/execution_context.cpp                                -*-C++-*-
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

#include "nstd/executor/execution_context.hpp"
#include <algorithm>
#include <atomic>

namespace NET = ::nstd::net;

// ----------------------------------------------------------------------------

NET::execution_context::execution_context() = default;

NET::execution_context::~execution_context()
{
    this->shutdown();
    this->destroy();
}

// ----------------------------------------------------------------------------

auto NET::execution_context::notify_fork(NET::fork_event ev) -> void
{
    ::std::lock_guard kerberos(this->d_bottleneck);
    if (NET::fork_event::prepare == ev) {
        ::std::for_each(this->d_services.begin(), this->d_services.end(),
                        [ev](auto svc){ svc->notify_fork(ev); });
    }
    else {
        ::std::for_each(this->d_services.rbegin(), this->d_services.rend(),
                        [ev](auto svc){ svc->notify_fork(ev); });
    }
}

// ----------------------------------------------------------------------------

auto NET::execution_context::shutdown() noexcept -> void
{
    ::std::lock_guard kerberos(this->d_bottleneck);
    if (this->d_running) {
        ::std::for_each(this->d_services.rbegin(), this->d_services.rend(),
                        [](auto svc){ svc->shutdown(); });
        this->d_running = false;
    }
}

auto NET::execution_context::destroy() noexcept -> void
{
    ::std::lock_guard kerberos(this->d_bottleneck);
    ::std::for_each(this->d_services.rbegin(), this->d_services.rend(),
                    [](auto& svc){ delete svc; });
    this->d_services.clear();
}

auto NET::execution_context::next_key() -> ::std::size_t
{
    static ::std::atomic<::std::size_t> rc{0};
    return rc++;
}

// ----------------------------------------------------------------------------

NET::execution_context::service::service(NET::execution_context& ctxt)
    : d_context(ctxt)
{
}

NET::execution_context::service::~service() = default;

// ----------------------------------------------------------------------------

auto NET::execution_context::service::context() noexcept
    -> NET::execution_context&
{
    return this->d_context;
}

auto NET::execution_context::service::notify_fork(NET::fork_event) -> void
{
}