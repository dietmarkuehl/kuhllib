// nstd/thread/mutex.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_THREAD_MUTEX
#define INCLUDED_NSTD_THREAD_MUTEX

#include <mutex>

// ----------------------------------------------------------------------------

namespace nstd {
    namespace thread {
        using ::std::mutex;
        using ::std::recursive_mutex;
        using ::std::timed_mutex;
        using ::std::recursive_timed_mutex;

        using ::std::defer_lock_t;
        using ::std::try_to_lock_t;
        using ::std::adopt_lock_t;

        using ::std::defer_lock;
        using ::std::try_to_lock;
        using ::std::adopt_lock;

        using ::std::lock_guard;
        using ::std::unique_lock;

        using ::std::try_lock;
        using ::std::lock;

        using ::std::once_flag;
        using ::std::call_once;
    }
}

// ----------------------------------------------------------------------------

#endif
