// nstd/hidden_names/boolean_socket_option.hpp                        -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_BOOLEAN_SOCKET_OPTION
#define INCLUDED_NSTD_HIDDEN_NAMES_BOOLEAN_SOCKET_OPTION

#include "nstd/file/socket.hpp"
#include "nstd/concepts/same_as.hpp"
#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename T>
    concept boolean_socket_option 
        = requires(T t) {
            { T() } noexcept;
            { T(true) } noexcept;
            { t.value() } noexcept -> ::nstd::concepts::same_as<bool>;
            { static_cast<bool>(t) } noexcept;
            { !t } noexcept -> nstd::concepts::same_as<bool>;
        }
        ;

    template <int> class boolean_socket_option_t;
}

// ----------------------------------------------------------------------------

template <int Name>
class nstd::hidden_names::boolean_socket_option_t
{
private:
    int d_value{};

public:
    using type = int;

    constexpr boolean_socket_option_t() noexcept = default;
    explicit constexpr boolean_socket_option_t(bool value) noexcept: d_value(value) {}

    constexpr auto operator= (bool value) noexcept -> boolean_socket_option_t& {
        this->d_value = value;
        return *this;
    }

    constexpr auto level(auto) const noexcept -> int { return SOL_SOCKET; }
    constexpr auto name(auto) const noexcept -> int { return Name; }
    constexpr auto data(auto) noexcept -> void* { return &this->d_value; }
    constexpr auto data(auto) const noexcept -> void const* { return &this->d_value; }
    constexpr auto size(auto) const noexcept -> ::std::size_t { return sizeof this->d_value; }

    constexpr auto value() const noexcept -> bool { return this->d_value; }
    constexpr auto operator not() const noexcept -> bool { return not this->value(); }
    explicit constexpr operator bool() const noexcept { return this->value(); }
};

    // ----------------------------------------------------------------------------

#endif
