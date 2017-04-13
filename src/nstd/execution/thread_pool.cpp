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
    this->start(count);
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
    while (this->d_threads.size() != count) {
        this->d_threads.emplace_back(&NE::thread_pool::donate, this);
    }
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

void NE::thread_pool::add_work(NE::work_source* source, unique_lock& kerberos) {
    this->d_queue.push_back(source);
    this->d_condition.notify_one();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void NE::thread_pool_executor::do_process_one(unique_lock& kerberos) {
    switch (this->d_tasks.size()) {
    default:
        this->d_pool.add_work(this, kerberos);
        this->process_one_locked(kerberos);
        break;
    case 1u:
        --this->d_running;
        this->process_one_locked(kerberos);
        break;
    case 0u:
        if (0u == --this->d_running) {
            this->d_done.notify_one();
        }
        // there is nothing to do
        break;
    }
}

NE::thread_pool_executor::thread_pool_executor(::nstd::execution::thread_pool& pool)
    : d_pool(pool) {
}
NE::thread_pool_executor::~thread_pool_executor() {
    unique_lock kerberos(this->d_pool.get_lock());
    while (0u != this->d_running) {
        this->d_done.wait(kerberos);
        //-dk:TODO signal condition variable
    }
}

void NE::thread_pool_executor::cancel_all() {
    unique_lock kerberos(this->d_pool.d_mutex);
    this->d_tasks.clear();
    this->d_pool.d_queue.erase(::std::find(this->d_pool.d_queue.begin(),
                                           this->d_pool.d_queue.end(),
                                           this),
                               this->d_pool.d_queue.end());
}

NE::thread_pool_executor::task NE::thread_pool_executor::get_task(unique_lock&) {
    task job(::nstd::utility::move(this->d_tasks.front()));
    this->d_tasks.pop_front();
    return job;
}

    // contribute to the processing instead of waiting
bool NE::thread_pool_executor::process_one() {
    unique_lock kerberos(this->d_pool.d_mutex);
    if (this->empty(kerberos)) {
        return false;
    }
    process_one_locked(kerberos);
    return !this->empty(kerberos);
}
void NE::thread_pool_executor::process_one_locked(unique_lock& kerberos) {
    assert(!this->empty(kerberos));
    task job(this->get_task(kerberos));
    ++this->d_running;
    {
        ::nstd::thread::unlock_guard<unique_lock> release(kerberos);
        job(*this);
    }
    if (0u == --this->d_running && this->d_tasks.empty()) {
        this->d_done.notify_all();
    }
}
void NE::thread_pool_executor::process() {
    unique_lock kerberos(this->d_pool.d_mutex);
    while (!this->empty(kerberos)) {
        process_one_locked(kerberos);
    }
}
