// nstd/execution/work_source.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_WORK_SOURCE
#define INCLUDED_NSTD_EXECUTION_WORK_SOURCE

#include "nstd/thread/mutex.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace execution {
        class work_source;
    }
}

// ----------------------------------------------------------------------------

class ::nstd::execution::work_source
{
public:
    using unique_lock = ::nstd::thread::unique_lock<::nstd::thread::mutex>;

    work_source()                      = default;
    work_source(work_source const&)    = delete;
    void operator=(work_source const&) = delete;
    virtual ~work_source() = default;

    ::std::size_t size(unique_lock& kerberos) const  { return this->do_size(kerberos); }
    void          process_one(unique_lock& kerberos) { this->do_process_one(kerberos); }

    virtual ::std::size_t do_size(unique_lock& kerberos) const = 0;
    virtual void          do_process_one(unique_lock& kerberos) = 0;
};

// ----------------------------------------------------------------------------

#endif
