// nstd/memory/uninitialized_default_construct.hpp                    -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_MEMORY_UNINITIALIZED_DEFAULT_CONSTRUCT
#define INCLUDED_NSTD_MEMORY_UNINITIALIZED_DEFAULT_CONSTRUCT

#include "nstd/memory/addressof.hpp"
#include "nstd/execution/execution.hpp"
#include "nstd/new/new.hpp"
#include "nstd/iterator/iterator_traits.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace memory {
        namespace detail {
            struct uninitialized_default_construct {
                constexpr uninitialized_default_construct() noexcept(true) {}
                template <typename ForwardIterator, typename EndPoint>
                auto operator()(ForwardIterator, EndPoint) const -> void;
                template <typename ExecutionPolicy, typename ForwardIterator, typename EndPoint>
                auto operator()(ExecutionPolicy&&, ForwardIterator, EndPoint) const -> void;
            };
        }
        constexpr nstd::memory::detail::uninitialized_default_construct
            uninitialized_default_construct{};
    }
}

// ----------------------------------------------------------------------------

template <typename ForwardIterator, typename EndPoint>
auto nstd::memory::detail::uninitialized_default_construct::operator()(ForwardIterator it,
                                                                       EndPoint        end) const
    -> void {
    using type = typename ::nstd::iterator_traits<ForwardIterator>::value_type;
    for (; it != end; ++it) {
        new(static_cast<void*>(::nstd::memory::addressof(*it))) type;
    }
}

template <typename ExecutionPolicy, typename ForwardIterator, typename EndPoint>
auto nstd::memory::detail::uninitialized_default_construct::operator()(ExecutionPolicy&& ep,
                                                                       ForwardIterator   it,
                                                                       EndPoint          end) const
    -> void {
    execute(::nstd::utility::forward<ExecutionPolicy>(ep), *this, it, end);
}

// ----------------------------------------------------------------------------

#endif
