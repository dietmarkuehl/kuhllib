// nstd/sender/just_error.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_NSTD_SENDER_JUST_ERROR
#define INCLUDED_NSTD_SENDER_JUST_ERROR

#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/utility/forward.hpp"
#include <cstddef>
#include <string_view>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename Value> class just_error_sender;
    template <typename Value, typename Receiver> struct just_error_state;

    template <typename Value>
    auto just_error(Value&& value)
        -> ::nstd::net::just_error_sender<Value>;
    template <::std::size_t N>
    auto just_error(char const (&value)[N])
        -> ::nstd::net::just_error_sender<::std::string_view>;
}

// ----------------------------------------------------------------------------

template <typename Value, typename Receiver>
struct nstd::net::just_error_state
{
    ::std::remove_cvref_t<Value>    d_value;
    ::std::remove_cvref_t<Receiver> d_receiver;

    auto start() noexcept -> void {
        ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver),
                                     ::nstd::utility::move(this->d_value));
    }
};

// ----------------------------------------------------------------------------

template <typename Value>
class nstd::net::just_error_sender
    : public ::nstd::execution::piped_sender_base
{
public:
    ::std::remove_cvref_t<Value> d_value;

    template <typename Receiver>
    auto connect(Receiver&& receiver) && -> ::nstd::net::just_error_state<Value, Receiver> {
        return ::nstd::net::just_error_state<Value, Receiver>{
            ::nstd::utility::move(this->d_value),
            ::nstd::utility::forward<Receiver>(receiver)
            };
    }
};

// ----------------------------------------------------------------------------

template <typename Value>
auto nstd::net::just_error(Value&& value)
    -> ::nstd::net::just_error_sender<Value>
{
    return just_error_sender<Value>{{}, ::nstd::utility::forward<Value>(value)};
}

template <::std::size_t N>
auto nstd::net::just_error(char const (&value)[N]) -> ::nstd::net::just_error_sender<::std::string_view>
{
    return ::nstd::net::just_error(::std::string_view(value));
}

// ----------------------------------------------------------------------------

#endif
