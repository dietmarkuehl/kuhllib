// include/io_ring.hpp                                                    -*-C++-*-
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

#ifndef INCLUDED_INCLUDE_IO_RING
#define INCLUDED_INCLUDE_IO_RING

#include <cstddef>
#include <sys/mman.h>
#include <linux/io_uring.h>

// ----------------------------------------------------------------------------
//-dk:TODO properly package this stuff:

struct app_io_sq_ring {
    unsigned *head;
    unsigned *tail;
    unsigned *ring_mask;
    //-dk:TODO needed? unsigned *ring_entries;
    //-dk:TODO needed? unsigned *flags;
    unsigned *array;
};
struct app_io_cq_ring {
    unsigned *head;
    unsigned *tail;
    unsigned *ring_mask;
    //-dk:TODO needed? unsigned *ring_entries;
    io_uring_cqe *cqes;
};

// ----------------------------------------------------------------------------

class io_ring
{
public:
    struct ring {
        int d_fd = -1;
        ~ring();
        int operator*() const { return this->d_fd; }
    };
    class mapped {
    private:
        void*       d_base;
        std::size_t d_length = 0u;

    public:
        mapped(): d_base(MAP_FAILED) {}
        mapped(mapped &&) = default;
        mapped(mapped const&) = delete;
        void operator=(mapped const&) = delete;
        ~mapped() {
            if (this->d_base != MAP_FAILED) {
                munmap(this->d_base, this->d_length);
            }
        }
        bool map(std::size_t length, int fd, off_t offset) {
            this->d_length = length;
            this->d_base = ::mmap(nullptr, length,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_POPULATE,
                                  fd, offset);
            return this->d_base != MAP_FAILED;
        }
        template <typename T>
        T* at_offset(std::size_t off) const {
            void* tmp(static_cast<char*>(this->d_base) + off);
            return static_cast<T*>(tmp);
        }
    };
    ring d_ring;
    mapped d_sbase;
    mapped d_cbase;
    mapped d_sqbase;

    app_io_sq_ring d_sring;
    io_uring_sqe*  d_sqes;
    app_io_cq_ring d_cring;

public:
    io_ring() = default;
    explicit io_ring(int queue_size);
    io_ring(io_ring &&) = default;
    io_ring(io_ring const&) = delete;
    void operator=(io_ring const&) = delete;
    ~io_ring() = default;

    int setup(int queue_size); // precondition: not set up
    void teardown();
    explicit operator bool() const { return *this->d_ring != -1; }

    unsigned int chead();
};

// ----------------------------------------------------------------------------

#endif
