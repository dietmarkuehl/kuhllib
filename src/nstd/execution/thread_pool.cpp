// nstd/execution/thread_pool.cpp                                     -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2016 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/execution/thread_pool.hpp"
#include "nstd/algorithm/max.hpp"

namespace NA = ::nstd::algorithm;
namespace NE = ::nstd::execution;

// ----------------------------------------------------------------------------

NE::thread_pool&
NE::thread_pool::default_pool() {
    static NE::thread_pool rc(::nstd::thread::thread::hardware_concurrency());
    return rc;
}

// ----------------------------------------------------------------------------

NE::thread_pool::thread_pool(unsigned count)
    : d_stopped(false) {
    this->start(NA::max(count, 1u));
}

NE::thread_pool::~thread_pool() {
    this->stop();
    for (thread& t: this->d_threads) { //-dk:TODO use an algorithm
        t.join();
    }
    //::nstd::algorithm::for_each(this->d_threads.begin(), this->d_threads.end(),
    //                           [](auto& thread){ thread.join(); });
}

// ----------------------------------------------------------------------------
void
NE::thread_pool::start(unsigned count) {
    {
        unique_lock kerberos(this->d_mutex);
        this->d_stopped = false;
    }
    while (this->d_threads.size() != count) {
        this->d_threads.emplace_back(&NE::thread_pool::donate, this);
    }
    assert(!this->d_threads.empty());
}

void NE::thread_pool::stop() {
        {
            unique_lock kerberos(this->d_mutex);
            this->d_stopped = true;
        }
        this->d_condition.notify_all();
    }

void
nstd::execution::thread_pool::donate() {
    unique_lock kerberos(this->d_mutex);
    while (!this->d_stopped) {
        if (this->d_queue.empty()) {
            this->d_condition.wait(kerberos,
                                   [this]{ return this->d_stopped
                                           || !this->d_queue.empty(); });
        }
        while (!this->d_queue.empty()) {
            work_source* ws = this->d_queue.front();
            this->d_queue.pop_front();
            ws->process_one(kerberos);
        }
    }
}

// ----------------------------------------------------------------------------

bool NE::thread_pool::add_work(NE::work_source* source, unique_lock& kerberos) {
    if (this->d_stopped) {
        return false;
    }
    this->d_queue.push_back(source);
    this->d_condition.notify_one();
    return true;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
