// nstd/file/ring_context.cpp                                         -*-C++-*-
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

#include "nstd/file/ring_context.hpp"
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iostream> //-dk:TODO
#include <cstring> //-dk:TODO
#include <cerrno> //-dk:TODO
#include <unistd.h> // requires a new enough kernel
#include <linux/io_uring.h> // requires a new enough kernel

#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>

namespace NF = ::nstd::file;

namespace
{
    int io_uring_setup(std::uint32_t size, io_uring_params* params)
    {
        return ::syscall(__NR_io_uring_setup, size, params);
    }

    int io_uring_enter(int fd, unsigned int count, unsigned int min_complete, unsigned int flags, sigset_t)
    {
        //-dk:TODO do something with the sigset?
        int rc(syscall(__NR_io_uring_enter, fd, count, min_complete, flags, NULL, 0));
        return rc;
    }
}

// ----------------------------------------------------------------------------

NF::ring_context::ring_context()
    : NF::ring_context(NF::ring_context::queue_size(1024)) //-dk:TODO remove/move to run
{
}

NF::ring_context::ring_context(NF::ring_context::queue_size size)
{
    if (this->setup(size) < 0) {
        throw ::std::runtime_error("failed to create io_uring"); //-dk:TODO throw a better error?
    }
}

NF::ring_context::~ring_context()
{
}

// ----------------------------------------------------------------------------

auto NF::ring_context::setup(NF::ring_context::queue_size size) -> int
{
    io_uring_params params{};
    this->d_fd = ::nstd::file::descriptor(io_uring_setup(static_cast<int>(size), &params));
    if (not this->d_fd) {
        return this->d_fd.get();
    }

    ::std::size_t const smap(params.sq_off.array + params.sq_entries * sizeof(::std::uint32_t));
    ::std::size_t const cmap(params.cq_off.cqes + params.cq_entries * sizeof(::io_uring_cqe));
    ::std::size_t const sqmap(params.sq_entries * sizeof(::io_uring_sqe));
    bool const          single_mmap(params.features & IORING_FEAT_SINGLE_MMAP);

    if (!this->d_smem.map(single_mmap? ::std::max(smap, cmap): smap,
                          this->d_fd.get(), IORING_OFF_SQ_RING)
        || (!single_mmap && !this->d_cmem.map(cmap, this->d_fd.get(), IORING_OFF_CQ_RING))
        || !this->d_emem.map(sqmap, this->d_fd.get(), IORING_OFF_SQES)
        ) {
            ::std::cout << "mmap error=" << ::std::strerror(errno) << "\n";
        return -1;
    }
    
    this->d_submission = ::nstd::file::ring<unsigned int>(this->d_smem,
                                                          params.sq_off.head,
                                                          params.sq_off.tail,
                                                          params.sq_off.ring_mask,
                                                          params.sq_off.array);
    this->d_completion = ::nstd::file::ring<::io_uring_cqe>(single_mmap? this->d_smem: this->d_cmem,
                                                            params.cq_off.head,
                                                            params.cq_off.tail,
                                                            params.cq_off.ring_mask,
                                                            params.cq_off.cqes);
    this->d_submission_elements = this->d_emem.at_offset<::io_uring_sqe>(0u);

    return this->d_fd.get();
}

// ----------------------------------------------------------------------------

auto NF::ring_context::process_result() -> unsigned int
{
    auto head(this->d_completion.head());
    auto tail(this->d_completion.tail());
    if (head != tail) {
        io_uring_cqe result{this->d_completion.get(head)};
        this->d_completion.advance_head();
        --this->d_outstanding;
        reinterpret_cast<NF::ring_context::io_base*>(result.user_data)->result(result.res, result.flags);
        return 1u;
    }
    return 0u;
}

// ----------------------------------------------------------------------------

auto NF::ring_context::intern_submit(::std::size_t) -> void
{
    io_uring_enter(this->d_fd.get(), 1u, 0u, 0u, ::sigset_t{});
}

auto NF::ring_context::run_one() -> NF::ring_context::count_type
{
    io_uring_enter(this->d_fd.get(), 0u, 1u, IORING_ENTER_GETEVENTS, ::sigset_t{});
    return process_result();
}

auto NF::ring_context::run()
    -> NF::ring_context::count_type
{
    NF::ring_context::count_type rc{};

    do
    {
        rc += this->run_one();
    }
    while (0u != this->d_outstanding);
    
    return rc;
}

// ----------------------------------------------------------------------------

auto NF::ring_context::accept(int      fd,
                              void*    addr,
                              void*    len,
                              int      flags,
                              io_base* continuation) -> void
{
    this->submit([=](::io_uring_sqe& element){
        element = ::io_uring_sqe{};
        element.opcode    = IORING_OP_ACCEPT;
        element.fd        = fd;
        element.flags     = flags;
        element.addr      = reinterpret_cast<::std::uint64_t>(addr);
        element.addr2     = reinterpret_cast<::std::uint64_t>(len);
        element.user_data = reinterpret_cast<::std::uint64_t>(continuation);
    });
}