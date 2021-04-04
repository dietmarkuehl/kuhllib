// buffer/mutable_buffer.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_BUFFER_MUTABLE_BUFFER
#define INCLUDED_BUFFER_MUTABLE_BUFFER

#include <array>
#include <string>
#include <vector>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    class mutable_buffer;

    template <typename> struct is_mutable_buffer_sequence;
    template <typename T>
    inline constexpr bool is_mutable_buffer_sequence_v
        = is_mutable_buffer_sequence<T>::value;

    mutable_buffer const*
    buffer_sequence_begin(mutable_buffer const&) noexcept;
    mutable_buffer const*
    buffer_sequence_end(mutable_buffer const&) noexcept;

    mutable_buffer operator+ (mutable_buffer const&, std::size_t) noexcept;
    mutable_buffer operator+ (std::size_t, mutable_buffer const&) noexcept;

    mutable_buffer buffer(void*, std::size_t) noexcept;
    mutable_buffer buffer(mutable_buffer const&) noexcept;
    mutable_buffer buffer(mutable_buffer const&, std::size_t) noexcept;

    template <typename T, std::size_t N>
    mutable_buffer buffer(T (&)[N]) noexcept;
    template <typename T, std::size_t N>
    mutable_buffer buffer(std::array<T, N>&) noexcept;
    template <typename T, typename A>
    mutable_buffer buffer(std::vector<T, A>&) noexcept;
    template <typename C, typename T, typename A>
    mutable_buffer buffer(std::basic_string<C, T, A>&) noexcept;

    template <typename T, std::size_t N>
    mutable_buffer buffer(T (&)[N], std::size_t) noexcept;
    template <typename T, std::size_t N>
    mutable_buffer buffer(std::array<T, N>&, std::size_t) noexcept;
    template <typename T, typename A>
    mutable_buffer buffer(std::vector<T, A>&, std::size_t) noexcept;
    template <typename C, typename T, typename A>
    mutable_buffer buffer(std::basic_string<C, T, A>&, std::size_t) noexcept;
}

// ----------------------------------------------------------------------------

class cxxrt::net::mutable_buffer
{
private:
    void*       d_data = nullptr;
    std::size_t d_size{};
public:
    mutable_buffer() noexcept = default;
    mutable_buffer(void*, std::size_t) noexcept;
    
    void*           data() const noexcept { return this->d_data; }
    std::size_t     size() const noexcept { return this->d_size; }
    mutable_buffer& operator+= (std::size_t n) noexcept;
};

// ----------------------------------------------------------------------------

template <typename T, std::size_t N>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(T (&b)[N]) noexcept
{
    return buffer(std::begin(b) != std::end(b)
                  ? std::addressof(*std::begin(b))
                  : nullptr,
                  std::size(b) * sizeof(*std::begin(b)));
}

template <typename T, std::size_t N>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(std::array<T, N>& b) noexcept
{
    return buffer(std::begin(b) != std::end(b)
                  ? std::addressof(*std::begin(b))
                  : nullptr,
                  std::size(b) * sizeof(*std::begin(b)));
}

template <typename T, typename A>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(std::vector<T, A>& b) noexcept
{
    return buffer(std::begin(b) != std::end(b)
                  ? std::addressof(*std::begin(b))
                  : nullptr,
                  std::size(b) * sizeof(*std::begin(b)));
}

template <typename C, typename T, typename A>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(std::basic_string<C, T, A>& b) noexcept
{
    return buffer(std::begin(b) != std::end(b)
                  ? std::addressof(*std::begin(b))
                  : nullptr,
                  std::size(b) * sizeof(*std::begin(b)));
}

// ----------------------------------------------------------------------------

template <typename T, std::size_t N>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(T (&b)[N], std::size_t s) noexcept
{
    return buffer(buffer(b), s);
}

template <typename T, std::size_t N>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(std::array<T, N>& b, std::size_t s) noexcept
{
    return buffer(buffer(b), s);
}

template <typename T, typename A>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(std::vector<T, A>& b, std::size_t s) noexcept
{
    return buffer(buffer(b), s);
}

template <typename C, typename T, typename A>
cxxrt::net::mutable_buffer
cxxrt::net::buffer(std::basic_string<C, T, A>& b, std::size_t s) noexcept
{
    return buffer(buffer(b), s);
}


// ----------------------------------------------------------------------------

#endif
