// nstd/net/socket_base.cpp                                           -*-C++-*-
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

#include "nstd/net/socket_base.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

// ----------------------------------------------------------------------------

auto nstd::net::socket_base::open(int domain, int type, int protocol) -> int
{
    this->d_descriptor = ::nstd::file::descriptor(::socket(domain, type, protocol));
#if 0
    int fd{this->d_descriptor.get()};
    if (0 <= fd) {
        this->d_flags = ::fcntl(fd, F_GETFL, 0);
    }
#endif
    return this->d_descriptor.get();
}

// ----------------------------------------------------------------------------

nstd::net::socket_base::socket_base(int domain, int type, int protocol)
    : d_flags()
{
    if (this->open(domain, type, protocol) < 0) {
        throw ::std::system_error(errno, ::std::system_category());
    }
}

// ----------------------------------------------------------------------------
// ::fcntl(fd, F_SETFL, this->d_flags)

auto nstd::net::socket_base::non_blocking() const noexcept -> bool
{
    return this->d_flags & O_NONBLOCK;
}

// ----------------------------------------------------------------------------

int socket_base_dummy = 0;
