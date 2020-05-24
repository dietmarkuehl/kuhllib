// include/internet/address.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_INTERNET_ADDRESS
#define INCLUDED_INTERNET_ADDRESS

#include <netfwd.hpp>
#include <internet/address_v4.hpp>

#include <memory>
#include <string>
#include <system_error>
#include <variant>

// ----------------------------------------------------------------------------

namespace cxxrt::net::ip
{
    class address_v6;
    class address;
}

// ----------------------------------------------------------------------------

class cxxrt::net::ip::address
{
private:
    struct empty {};
    std::variant<empty, address_v4/*, address_v6*/> d_addr;

public:
    constexpr address() noexcept;
    /*-dk:TODO constexpr*/ address(address const& a) noexcept = default;
    /*-dk:TODO constexpr*/ address(address_v4 const& a) noexcept;
    constexpr address(address_v6 const& a) noexcept;

    address& operator=(address const& a) noexcept;
    address& operator=(address_v4 const& a) noexcept;
    address& operator=(address_v6 const& a) noexcept;

    constexpr bool is_v4() const noexcept;
    constexpr bool is_v6() const noexcept;
    constexpr address_v4 to_v4() const;
    constexpr address_v6 to_v6() const;
    constexpr bool is_unspecified() const noexcept;
    constexpr bool is_loopback() const noexcept;
    constexpr bool is_multicast() const noexcept;
    template<typename Allocator = std::allocator<char>>
    std::basic_string<char, std::char_traits<char>, Allocator>
    to_string(Allocator const& a = Allocator()) const;

    bool operator== (address const&) const;
    bool operator<  (address const&) const;
    template <typename cT, typename Traits>
    friend std::basic_ostream<cT, Traits>&
    operator<< (std::basic_ostream<cT, Traits>& out, address const& addr)
    {
        return out << addr.to_string();
    }
};

// ----------------------------------------------------------------------------

/*-dk:TODO constexpr*/ cxxrt::net::ip::address::address(address_v4 const& a) noexcept
    : d_addr(a)
{
}

// ----------------------------------------------------------------------------

template<typename Allocator>
std::basic_string<char, std::char_traits<char>, Allocator>
cxxrt::net::ip::address::to_string(Allocator const& a) const
{
    switch (this->d_addr.index())
    {
    default: return std::basic_string<char, std::char_traits<char>, Allocator>(a);
    case 1:  return std::get<1>(this->d_addr).to_string(a);
        //-dk:TODO: case 2: return std::get<2>(this->d_addr).to_string(a);
    }
}

// ----------------------------------------------------------------------------

#endif
