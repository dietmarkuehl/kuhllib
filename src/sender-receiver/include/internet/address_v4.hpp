// include/internet/address_v4.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_INTERNET_ADDRESS_V4
#define INCLUDED_INTERNET_ADDRESS_V4

#include <netfwd.hpp>

#include <memory>
#include <string>
#include <system_error>
#include <cstdint>

// ----------------------------------------------------------------------------

namespace cxxrt::net::ip
{
    class address_v4;
    class v4_mapped_t;

    using uint_type = uint_least32_t;
    struct bytes_type { std::byte bytes[4]; };

    constexpr address_v4 make_address_v4(bytes_type const& bytes);
    constexpr address_v4 make_address_v4(uint_type val);
    constexpr address_v4 make_address_v4(v4_mapped_t, address_v6 const& a);
    address_v4 make_address_v4(char const* str);
    address_v4 make_address_v4(char const* str, std::error_code& ec) noexcept;
    address_v4 make_address_v4(std::string const& str);
    address_v4 make_address_v4(std::string const& str, std::error_code& ec) noexcept;
    address_v4 make_address_v4(std::string_view str);
    address_v4 make_address_v4(std::string_view str, std::error_code& ec) noexcept;
}

// ----------------------------------------------------------------------------

class cxxrt::net::ip::address_v4
{
public:
    using uint_type = uint_least32_t;
    struct bytes_type;

private:
    uint_type d_addr{};
    
    template <std::size_t S>
    char const* to_string(char (&a)[S]) const { return this->to_string(a, S); }
    char const* to_string(char* buffer, std::size_t size) const;

public:
    constexpr address_v4() noexcept = default;
    constexpr address_v4(const address_v4& a) noexcept = default;
    constexpr address_v4(const bytes_type&);
    explicit constexpr address_v4(uint_type a): d_addr(a) {};

    address_v4& operator=(const address_v4&) noexcept = default;

    constexpr bool is_unspecified() const noexcept;
    constexpr bool is_loopback() const noexcept;
    constexpr bool is_multicast() const noexcept;
    constexpr bytes_type to_bytes() const noexcept;
    constexpr uint_type to_uint() const noexcept;
    template<typename Allocator = std::allocator<char>>
    std::basic_string<char, std::char_traits<char>, Allocator>
    to_string(Allocator const& a = Allocator()) const;

    static constexpr address_v4 any() noexcept;
    static constexpr address_v4 loopback() noexcept;
    static constexpr address_v4 broadcast() noexcept;

    bool operator== (address_v4 const&) const;
    bool operator<  (address_v4 const&) const;
    template <typename cT, typename Traits>
    friend std::basic_ostream<cT, Traits>&
    operator<< (std::basic_ostream<cT, Traits>&, address_v4 const&);
};

// ----------------------------------------------------------------------------

template<typename Allocator>
std::basic_string<char, std::char_traits<char>, Allocator>
cxxrt::net::ip::address_v4::to_string(Allocator const& a) const
{
    char buffer[32];
    return std::basic_string<char, std::char_traits<char>, Allocator>(
        this->to_string(buffer), a);
}

// ----------------------------------------------------------------------------

#endif
