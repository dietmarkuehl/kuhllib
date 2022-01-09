// nstd/file/mapped_memory.cpp                                        -*-C++-*-
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
//  OTHER DEALINGS IN THE SOFTW
// ARE. 
// ----------------------------------------------------------------------------

#include "nstd/file/mapped_memory.hpp"
#include "nstd/utility/exchange.hpp"
#ifdef _MSC_VER
#    include <cstddef>

namespace {
    void* mmap(void*, ::std::size_t, int, int, int, ::std::ptrdiff_t) { return nullptr; }
    int munmap(void*, ::std::size_t) { return 0; }
    constexpr int PROT_READ = 0;
    constexpr int PROT_WRITE = 0;
    constexpr int MAP_SHARED = 0;
    constexpr void* MAP_FAILED = nullptr;
}
#else
#    include <unistd.h>
#    include <sys/mman.h>
#endif

// ----------------------------------------------------------------------------

nstd::file::mapped_memory::mapped_memory(mapped_memory && other)
    : d_base(::nstd::utility::exchange(other.d_base, nullptr))
{
}

nstd::file::mapped_memory::~mapped_memory()
{
    if (this->d_base != nullptr) {
        ::munmap(this->d_base, this->d_length);
    }
}

auto nstd::file::mapped_memory::map(::std::size_t length,
                                    int           fd,
                                    ::std::ptrdiff_t offset)
    -> bool
{
    this->d_length = length;
    (void)fd;
    (void)offset;
    this->d_base = ::mmap(nullptr, length,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED
#ifdef NSTD_HAS_MAP_POPULATE
                          | MAP_POPULATE
#endif
                          ,
                          fd, offset);
    if (this->d_base == MAP_FAILED) {
        this->d_base = nullptr;
        return false;
    }
    return true;
}
