// nstd/executor/associated_allocator.hpp                             -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTOR_ASSOCIATED_ALLOCATOR
#define INCLUDED_NSTD_EXECUTOR_ASSOCIATED_ALLOCATOR

#include <memory>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename T, class ProtoAllocator = ::std::allocator<void>>
    struct associated_allocator;

    template <typename T, class ProtoAllocator = ::std::allocator<void>>
    using associated_allocator_t = typename ::nstd::net::associated_allocator<T, ProtoAllocator>::type;

    template <typename T>
    auto get_associated_allocator(T const&) noexcept
        -> ::nstd::net::associated_allocator_t<T>;
    template <typename T, typename ProtoAllocator>
    auto get_associated_allocator(T const&, ProtoAllocator const&) noexcept
        -> ::nstd::net::associated_allocator_t<T, ProtoAllocator>;
}

// ----------------------------------------------------------------------------

template <typename T, class ProtoAllocator>
struct nstd::net::associated_allocator
{
    using type = ProtoAllocator;
    static type get(T const&, ProtoAllocator const& a = {}) noexcept{
        return a;
    }
};

template <typename T, class ProtoAllocator>
    requires requires{ typename T::allocator_type; }
struct nstd::net::associated_allocator<T, ProtoAllocator>
{
    using type = typename T::allocator_type;
    static type get(T const& t, ProtoAllocator const& = {}) noexcept{
        return t.get_allocator();
    }
};

// ----------------------------------------------------------------------------

template <typename T>
auto nstd::net::get_associated_allocator(T const& t) noexcept
    -> ::nstd::net::associated_allocator_t<T>
{
    return ::nstd::net::associated_allocator<T>::get(t);
}

template <typename T, typename ProtoAllocator>
auto nstd::net::get_associated_allocator(T const& t, ProtoAllocator const& allocator) noexcept
    -> ::nstd::net::associated_allocator_t<T, ProtoAllocator>
{
    return ::nstd::net::associated_allocator<T, ProtoAllocator>::get(t, allocator);
}

// ----------------------------------------------------------------------------

#endif
