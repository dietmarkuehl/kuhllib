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

#include "nstd/execution/work_source.hpp"
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
        class thread_pool;
    }
}

// ----------------------------------------------------------------------------

class nstd::execution::thread_pool {
public:
    using mutex              = ::nstd::thread::mutex;
    using thread             = ::nstd::thread::thread;
    using condition_variable = ::nstd::thread::condition_variable;
    using unique_lock        = ::nstd::thread::unique_lock<mutex>;
    using work_source        = ::nstd::execution::work_source;

private:

    mutex                                  d_mutex;
    condition_variable                     d_condition;
    ::nstd::container::deque<work_source*> d_queue;
    ::nstd::container::vector<thread>      d_threads;
    bool                                   d_stopped = true;

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
    bool add_work(work_source* source, unique_lock& kerberos);

    unique_lock get_lock() { return unique_lock(this->d_mutex); }
};

// ----------------------------------------------------------------------------

#endif
