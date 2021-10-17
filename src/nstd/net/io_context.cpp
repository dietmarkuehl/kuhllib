// nstd/net/io_context.cpp                                            -*-C++-*-
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

#include "nstd/net/io_context.hpp"
#include <cassert>

namespace nstd::net {
    int io_context_dummy = 0;
}

// ----------------------------------------------------------------------------

nstd::net::io_context::io_context()
#if NSTD_HAS_LINUX_IO_URING
    : nstd::net::io_context(nstd::file::ring_context::queue_size(1024))
#else
    : d_context(nullptr)
#endif
{
    assert(this->d_context && "install poll_context if ring_context can't be used");
}

nstd::net::io_context::io_context(::nstd::file::ring_context::queue_size size)
#if NSTD_HAS_LINUX_IO_URING
    : d_context(new ::nstd::file::ring_context(size))
#else
    : d_context(nullptr)
#endif
{
    (void)size;
}

auto nstd::net::io_context::run_one() -> nstd::net::io_context::count_type
{
    return this->d_context->run_one();
}

auto nstd::net::io_context::run() -> nstd::net::io_context::count_type
{
    count_type count{};
    while (1u == this->d_context->run_one()) {
        ++count;
    }
    return count;
}
