// toy-networking.hpp                                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_TOY_NETWORKING
#define INCLUDED_TOY_NETWORKING

#if defined(TOY_HAS_POLL)
#    include "toy-networking-poll.hpp"
#endif
#if defined(TOY_HAS_IO_URING)
#    include "toy-networking-io_uring.hpp"
#endif
#if defined(TOY_HAS_EPOLL)
#    include "toy-networking-epoll.hpp"
#endif
#if defined(TOY_HAS_KQUEUE)
#   include "toy-networking-kqueue.hpp"
#endif
#if defined(TOY_HAS_IOCP)
#   include "toy-networking-iocp.hpp"
#endif
#include "toy-networking-sender.hpp"

namespace toy {
    using io_context = TOY_CONTEXT;
}

#if defined(TOY_HAS_WINSOCK2)
#    include "toy-networking-winsock2.hpp"
#else
#    include "toy-networking-posix.hpp"
#endif

// ----------------------------------------------------------------------------

#endif
