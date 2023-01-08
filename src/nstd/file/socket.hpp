// nstd/file/socket.hpp                                               -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_SOCKET
#define INCLUDED_NSTD_FILE_SOCKET

// ----------------------------------------------------------------------------

#include <cstddef>
#ifndef _MSC_VER
#    include <netinet/in.h>
#    include <sys/socket.h>
#    include <sys/types.h>
#    include <sys/uio.h>
#else
#    include <WinSock2.h>

using socklen_t = ::std::size_t;

struct iovec { //-dk:TODO abstract iovec nicer
    void* iov_base;
    ::std::size_t iov_len;
};

struct msghdr {
    void*          msg_name;
    ::socklen_t    msg_namelen;
    ::iovec*       msg_iov;
    ::std::size_t  msg_iovlen;
    void*          msg_control;
    ::std::size_t  msg_controllen;
    int            msg_flags;
};

struct in_addr_t {
    char buffer[4];
};

struct sockaddr_in6 {
    int sin6_family;
    short sin6_port;
    char sin6_addr[16];
};
#endif

// ----------------------------------------------------------------------------

#endif
