// src/internet/address_v4.cpp                                        -*-C++-*-
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

#include <internet/address_v4.hpp>

#include <arpa/inet.h>
#include <iostream> //-dk:TODO remove
#include <cerrno> //-dk:TODO remove
#include <cstring> //-dk:TODO remove

// ----------------------------------------------------------------------------

int internet_address_v4 = 0;

// ----------------------------------------------------------------------------

char const*
cxxrt::net::ip::address_v4::to_string(char* buffer, std::size_t size) const
{
    return inet_ntop(AF_INET, &this->d_addr, buffer, size)? buffer: "";
}


// ----------------------------------------------------------------------------

auto cxxrt::net::ip::make_address_v4(char const* str) -> address_v4
{
    uint_type addr{};
    return ::inet_pton(AF_INET, str, &addr)? address_v4(addr): address_v4();
}

auto cxxrt::net::ip::make_address_v4(std::string const& str) -> address_v4
{
    return make_address_v4(str.c_str());
}
auto cxxrt::net::ip::make_address_v4(std::string_view str) -> address_v4
{
    return make_address_v4(std::string(str));
}

