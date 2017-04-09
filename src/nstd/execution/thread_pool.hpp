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

#include "nstd/algorithm/for_each.hpp"
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

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
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
    using task     = ::nstd::functional::function<void(executor&)>;
    using queue    = ::nstd::container::deque<task>;
    using guard    = ::nstd::thread::unique_lock<::nstd::thread::mutex>;

    friend class ::nstd::execution::thread_pool_executor;

private:
#if 0
    struct thread
        : public ::nstd::thread::thread
    {
        using ::nstd::thread::thread::thread;
        thread(::nstd::thread::thread&& t): ::nstd::thread::thread(::nstd::utility::move(t)) {}
        ~thread() { if (this->joinable()) { this->join(); } }
    };
#else
    using thread = ::nstd::thread::thread;
#endif
    ::nstd::thread::mutex               d_mutex;
    ::nstd::thread::condition_variable  d_condition;
    ::nstd::container::deque<executor*> d_queue;
    ::nstd::container::vector<thread>   d_threads;
    bool                                d_stopped;

public:
    explicit thread_pool()
        : d_stopped(false) {
    }
    ~thread_pool() {
        this->stop();
        ::nstd::algorithm::for_each(this->d_threads.begin(), this->d_threads.end(),
                                   [](auto& thread){ thread.join(); });
    }
    // maintaining threads in the pool
    void start(unsigned count = ::nstd::thread::thread::hardware_concurrency()) {
        while (this->d_threads.size() != count) {
            this->d_threads.emplace_back(&::nstd::execution::thread_pool::donate, this);
        }
    }
    void stop() {
        {
            guard kerberos(this->d_mutex);
            this->d_stopped = true;
        }
        this->d_condition.notify_all();
    }
    void donate();
};

// ----------------------------------------------------------------------------

class nstd::execution::thread_pool_executor {
private:
    using executor = ::nstd::execution::thread_pool_executor;
    using task     = ::nstd::functional::function<void(executor&)>;
    using queue    = ::nstd::container::deque<task>;
    using guard    = ::nstd::thread::unique_lock<::nstd::thread::mutex>;

    ::nstd::execution::thread_pool& d_pool;
    queue                           d_tasks;

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
public:
    // Use the default thread_pool.
    explicit thread_pool_executor();
    // Use the specified thread_pool.
    explicit thread_pool_executor(::nstd::execution::thread_pool& pool)
        : d_pool(pool) {
    }
    ~thread_pool_executor() {
        //-dk:TODO wait until all processing is completed
    }

    // job control
    template <typename Job> void add(Job&& job) {
        guard kerberos(this->d_pool.d_mutex);
        this->d_tasks.push_back(this->make_task(::nstd::utility::forward<Job>(job)));
        if (this->d_tasks.size() == 1u) {
            this->d_pool.d_queue.push_back(this);
        }
    }
    template <typename Job> void add_front(Job&& job) {
        guard kerberos(this->d_pool.d_mutex);
        this->d_tasks.push_front(this->make_task(::nstd::utility::forward<Job>(job)));
        if (this->d_tasks.size() == 1u) {
            this->d_pool.d_queue.push_back(this);
        }
    }
    void cancel_all() {
        guard kerberos(this->d_pool.d_mutex);
        this->d_tasks.clear();
        //-dk:TODO remove tasks from the thread pool
    }

    // job access
    bool empty() const { return this->d_tasks.empty(); }
    task get_task() {
        task job(::nstd::utility::move(this->d_tasks.front()));
        this->d_tasks.pop_front();
        return job;
    }

    // contribute to the processing instead of waiting
    bool process_one() {
        task job;
        {
            guard kerberos(this->d_pool.d_mutex);
            if (this->d_tasks.empty()) {
                return false;
            }
            job = this->get_task();
        }
        job(*this);
        return true;
    }
    void process() {
        while (process_one()) {
        }
    }
};

// ----------------------------------------------------------------------------

void nstd::execution::thread_pool::donate() { // donate the current thread to the pool
    task job;
    executor* e = nullptr;
    while (true) {
        {
            guard kerberos(this->d_mutex);
            this->d_condition.wait(kerberos,
                                   [this]{ return this->d_stopped
                                               || !this->d_queue.empty(); });
            if (this->d_stopped) {
                break;
            }
            while (!this->d_queue.empty() && this->d_queue.front()->empty()) {
                this->d_queue.pop_front();
            }
            if (this->d_queue.empty()) {
                continue;
            }
            e = this->d_queue.front();
            this->d_queue.pop_front();
            job = e->get_task();
            if (!e->empty()) {
                this->d_queue.push_back(e);
            }
        }
        job(*e);
    }
}

// ----------------------------------------------------------------------------

#endif
