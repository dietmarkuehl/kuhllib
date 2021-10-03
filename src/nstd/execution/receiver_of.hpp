// nstd/execution/receiver_of.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_RECEIVER_OF
#define INCLUDED_NSTD_EXECUTION_RECEIVER_OF

#include "nstd/execution/receiver.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/type_traits/remove_cvref.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    template <typename Receiver, typename... Args>
    concept receiver_of
        =  ::nstd::execution::receiver<Receiver>
        && requires(::nstd::type_traits::remove_cvref_t<Receiver>&& rec, Args&&... args) {
            ::nstd::execution::set_value(::nstd::utility::move(rec),
                                         ::nstd::utility::forward<Args>(args)...);
        }
        ;

    template <typename... Args>
    struct test_receiver_of;
}

// ----------------------------------------------------------------------------

template <typename... Args>
struct nstd::execution::test_receiver_of
{
    test_receiver_of(test_receiver_of&&) = default;

    friend auto tag_invoke(::nstd::execution::set_value_t, test_receiver_of&&, Args...) noexcept -> void {};
    template <typename Error>
    friend auto tag_invoke(::nstd::execution::set_error_t, test_receiver_of&&, Error&&) noexcept -> void {};
    friend auto tag_invoke(::nstd::execution::set_done_t, test_receiver_of&&) noexcept -> void {};
};

static_assert(::nstd::execution::receiver_of<::nstd::execution::test_receiver_of<int, bool&, char const>, int, bool&, char const>);

// ----------------------------------------------------------------------------

#endif
