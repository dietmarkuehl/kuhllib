// include/socket/socket_base.hpp                                     -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_SOCKET_SOCKET_BASE
#define INCLUDED_SOCKET_SOCKET_BASE

#include <netfwd.hpp>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    class socket_base;
}

// ----------------------------------------------------------------------------

class cxxrt::net::socket_base
{
public:
    class broadcast;
    class debug;
    class do_not_route;
    class keep_alive;
    class linger;
    class out_of_band_inline;
    class receive_buffer_size;
    class receive_low_watermark;
    class reuse_address;
    class send_buffer_size;
    class send_low_watermark;

    using shutdown_type = int; //-dk:TODO 
    //-dk:TODO static constexpr shutdown_type shutdown_receive;
    //-dk:TODO static constexpr shutdown_type shutdown_send;
    //-dk:TODO static constexpr shutdown_type shutdown_both;

    using wait_type = int; //-dk:TODO 
    //-dk:TODO static constexpr wait_type wait_read;
    //-dk:TODO static constexpr wait_type wait_write;
    //-dk:TODO static constexpr wait_type wait_error;

    using message_flags = int; //-dk:TODO
    //-dk:TODO static constexpr message_flags message_peek;
    //-dk:TODO static constexpr message_flags message_out_of_band;
    //-dk:TODO static constexpr message_flags message_do_not_route;

    static const int max_listen_connections;

protected:
    socket_base();
    ~socket_base() = default;
};

// ----------------------------------------------------------------------------

#endif
