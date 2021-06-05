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
#include <functional>
#include <stdexcept>
#include <linux/io_uring.h> // requires a new enough kernel

#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>

namespace NET = ::nstd::net;

namespace
{
    int io_uring_setup(std::uint32_t size, io_uring_params* params)
    {
        return ::syscall(__NR_io_uring_setup, size, params);
    }
}

// ----------------------------------------------------------------------------

NET::io_context::io_context()
    : NET::io_context(NET::io_context::queue_size(1024)) //-dk:TODO remove/move to run
{
}

NET::io_context::io_context(NET::io_context::queue_size size)
{
    if (this->setup(size) < 0) {
        throw ::std::runtime_error("failed to create io_uring"); //-dk:TODO throw a better error?
    }
}

NET::io_context::~io_context()
{
}

// ----------------------------------------------------------------------------

auto NET::io_context::setup(NET::io_context::queue_size size) -> int
{
    io_uring_params params{};
    this->d_fd = ::nstd::file::descriptor(io_uring_setup(static_cast<int>(size), &params));
    if (this->d_fd < 0) {
        return this->d_fd;
    }

    return 0;
}

// ----------------------------------------------------------------------------

auto NET::io_context::run()
    -> NET::io_context::count_type
{
    //-dk:TODO work on task queue
    return 0;
}