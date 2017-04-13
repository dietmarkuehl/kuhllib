// nstd/execution/thread_pool_executor.hpp                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_THREAD_POOL_EXECUTOR
#define INCLUDED_NSTD_EXECUTION_THREAD_POOL_EXECUTOR

#include "nstd/execution/work_source.hpp"
#include "nstd/execution/thread_pool.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
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
        !nstd::execution::has_thread_pool_executor_arg<Job>::value, task> {
        return task([job = ::nstd::utility::forward<Job>(job)](::nstd::execution::thread_pool_executor&) {
                job();
            });
    }

    void do_process_one(unique_lock& kerberos) override;
    void register_work(unique_lock& kerberos) {
        if (this->d_pool.add_work(this, kerberos)) {
            ++this->d_running; // tasks registered with the pool are considered running
        }
    }

public:
    // Use the specified thread_pool.
    explicit thread_pool_executor(::nstd::execution::thread_pool& pool
                                  = ::nstd::execution::thread_pool::default_pool());
    ~thread_pool_executor();

    // job control
    template <typename Job> void add(Job&& job) {
        unique_lock kerberos(this->d_pool.get_lock());
        this->d_tasks.push_back(this->make_task(::nstd::utility::forward<Job>(job)));
        if (this->d_tasks.size() == 1u) {
            this->register_work(kerberos);
        }
    }
    template <typename Job> void add_front(Job&& job) {
        unique_lock kerberos(this->d_pool.get_lock());
        this->d_tasks.push_front(this->make_task(::nstd::utility::forward<Job>(job)));
        if (this->d_tasks.size() == 1u) {
            this->register_work(kerberos);
        }
    }
    void cancel_all();

    // job access
    bool     empty(unique_lock&) const { return this->d_tasks.empty(); }
    task get_task(unique_lock&);

    // contribute to the processing instead of waiting
    bool process_one();
    void process_one_locked(unique_lock& kerberos);
    void process();
};

// ----------------------------------------------------------------------------

#endif
