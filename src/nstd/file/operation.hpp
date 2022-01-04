// nstd/file/operation.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_OPERATION
#define INCLUDED_NSTD_FILE_OPERATION

#include "nstd/file/io_base.hpp"
#include "nstd/hidden_names/message_flags.hpp"
#include <sys/types.h>
#include <sys/socket.h>

// ----------------------------------------------------------------------------

namespace nstd::file {
    struct operation;
    struct operation_read;
    struct operation_read_some;
    struct operation_receive;
    template <typename> struct operation_send;
    struct operation_write;
    struct operation_write_some;
}

// ----------------------------------------------------------------------------

struct nstd::file::operation
    : ::nstd::file::io_base
{
    virtual auto submit() -> void = 0;
};

// ----------------------------------------------------------------------------

template <typename ConstantBufferSequence>
struct nstd::file::operation_send
    : ::nstd::file::operation
{
    ConstantBufferSequence              d_buffers;
    ::nstd::hidden_names::message_flags d_flags;
    ::msghdr                            d_msgheader{};

    operation_send(ConstantBufferSequence const&       buffers,
                   ::nstd::hidden_names::message_flags flags)
        : d_buffers(buffers)
        , d_flags(flags)
    {
    }
};

// ----------------------------------------------------------------------------

#endif
