// nstd/thread/thread_pool.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_THREAD_THREAD_POOL
#define INCLUDED_NSTD_THREAD_THREAD_POOL

#include <algorithm>
// #include "nstd/algorithm/for_each.hpp"
#include "nstd/functional/function.hpp"
#include "nstd/container/deque.hpp"
#include "nstd/container/vector.hpp"
#include "nstd/thread/thread.hpp"
#include "nstd/thread/mutex.hpp"
#include "nstd/thread/condition_variable.hpp"
#include "nstd/thread/unlock_guard.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <iostream>
#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
        class work_source;
        class thread_pool;
        class thread_pool_executor;

        template <typename Fun> struct test_thread_pool_executor_arg;
        template <typename Fun> struct has_thread_pool_executor_arg;
        template <typename Fun>
        constexpr bool has_thread_pool_executor_arg_v
            = ::nstd::execution::has_thread_pool_executor_arg<Fun>::value;
    }
}

// ----------------------------------------------------------------------------

class ::nstd::execution::work_source {
public:
    using unique_lock = ::nstd::thread::unique_lock<::nstd::thread::mutex>;

    virtual ::std::size_t size(unique_lock& kerberos) const = 0;
    virtual void          process_one(unique_lock& kerberos) = 0;
};

// ----------------------------------------------------------------------------

template <typename Fun>
struct nstd::execution::test_thread_pool_executor_arg {
    template <typename F>
    static char (&test(F* f, decltype((*f)(::nstd::type_traits::declval<::nstd::execution::thread_pool_executor&>()))*))[1];
    template <typename F>
    static char (&test(F* f, ...))[2];
};

template <typename Fun>
struct nstd::execution::has_thread_pool_executor_arg
    : ::nstd::type_traits::integral_constant<bool,
        1u == sizeof(::nstd::execution::test_thread_pool_executor_arg<Fun>::test(static_cast<Fun*>(0), 0))> {
};

// ----------------------------------------------------------------------------

class nstd::execution::thread_pool {
public:
    using executor = ::nstd::execution::thread_pool_executor;
    using unique_lock    = ::nstd::thread::unique_lock<::nstd::thread::mutex>;

    friend class ::nstd::execution::thread_pool_executor;

private:
    using thread = ::nstd::thread::thread;

    ::nstd::thread::mutex               d_mutex;
    ::nstd::thread::condition_variable  d_condition;
    ::nstd::container::deque<work_source*> d_queue;
    ::nstd::container::vector<thread>   d_threads;
    bool                                d_stopped;

public:
    static ::nstd::execution::thread_pool& default_pool();

    explicit thread_pool(unsigned count = 0u);
    thread_pool(thread_pool&)             = delete;
    thread_pool(thread_pool&&)            = default;
    thread_pool& operator=(thread_pool&)  = delete;
    thread_pool& operator=(thread_pool&&) = default;
    ~thread_pool();

    void start(unsigned count);
    void stop();
    void donate();
    void add_work(::nstd::execution::work_source* source, unique_lock& kerberos);

    inline void lock()   { this->d_mutex.lock(); }
    inline void unlock() { this->d_mutex.unlock(); }
};

// ----------------------------------------------------------------------------

class nstd::execution::thread_pool_executor
    : public ::nstd::execution::work_source {
private:
    using unique_lock = ::nstd::thread::unique_lock<::nstd::thread::mutex>;

    using executor = ::nstd::execution::thread_pool_executor;
    using task     = ::nstd::functional::function<void(executor&)>;
    using queue    = ::nstd::container::deque<task>;

    ::nstd::execution::thread_pool&     d_pool;
    queue                               d_tasks;
    ::nstd::thread::condition_variable  d_done;
    unsigned                            d_running = 0u;

    thread_pool_executor(thread_pool_executor&) = delete;
    void operator=(thread_pool_executor&) = delete;

    template <typename Job>
    auto make_task(Job&& job) -> ::nstd::type_traits::enable_if_t<
        nstd::execution::has_thread_pool_executor_arg_v<Job>, task> {
        return task(::nstd::utility::forward<Job>(job));
    }
    template <typename Job>
    auto make_task(Job&& job) -> ::nstd::type_traits::enable_if_t<
        !nstd::execution::has_thread_pool_executor_arg_v<Job>, task> {
        return task([job = ::nstd::utility::forward<Job>(job)](::nstd::execution::thread_pool_executor&) {
                job();
            });
    }

    void process_one(unique_lock& kerberos) override {
        this->process_one_locked(kerberos);
    }

public:
    // Use the specified thread_pool.
    explicit thread_pool_executor(::nstd::execution::thread_pool& pool
                                  = ::nstd::execution::thread_pool::default_pool())
        : d_pool(pool) {
    }
    ~thread_pool_executor() {
        unique_lock kerberos(this->d_pool.d_mutex);
        while (0u != this->d_running) {
            this->d_done.wait(kerberos);
            //-dk:TODO signal condition variable
        }
    }

    // job control
    template <typename Job> void add(Job&& job) {
        unique_lock kerberos(this->d_pool.d_mutex);
        this->d_tasks.push_back(this->make_task(::nstd::utility::forward<Job>(job)));
        if (this->d_tasks.size() == 1u) {
            this->d_pool.d_queue.push_back(this);
            //-dk:TODO signal condition variable
        }
    }
    template <typename Job> void add_front(Job&& job) {
        unique_lock kerberos(this->d_pool.d_mutex);
        this->d_tasks.push_front(this->make_task(::nstd::utility::forward<Job>(job)));
        if (this->d_tasks.size() == 1u) {
            this->d_pool.d_queue.push_back(this);
        }
    }
    void cancel_all() {
        unique_lock kerberos(this->d_pool.d_mutex);
        this->d_tasks.clear();
        this->d_pool.d_queue.erase(::std::find(this->d_pool.d_queue.begin(),
                                               this->d_pool.d_queue.end(),
                                               this),
                                   this->d_pool.d_queue.end());
    }

    // job access
    bool     empty(unique_lock&) const { return this->d_tasks.empty(); }
    ::std::size_t size(unique_lock& kerberos) const override {
        return this->d_tasks.size();
    }
    task get_task(unique_lock&) {
        task job(::nstd::utility::move(this->d_tasks.front()));
        this->d_tasks.pop_front();
        return job;
    }

    // contribute to the processing instead of waiting
    bool process_one() {
        unique_lock kerberos(this->d_pool.d_mutex);
        if (this->empty(kerberos)) {
            return false;
        }
        process_one_locked(kerberos);
        return !this->empty(kerberos);
    }
    void process_one_locked(unique_lock& kerberos) {
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
    void process() {
        unique_lock kerberos(this->d_pool.d_mutex);
        while (!this->empty(kerberos)) {
            process_one_locked(kerberos);
        }
    }
};

// ----------------------------------------------------------------------------

#endif
