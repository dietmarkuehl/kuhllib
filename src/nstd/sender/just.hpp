// nstd/sender/just.hpp                                               -*-C++-*-
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

#ifndef INCLUDED_NSTD_SENDER_JUST
#define INCLUDED_NSTD_SENDER_JUST

#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <exception>
#include <string_view>
#include <type_traits>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename Value> class just_sender;
    template <typename Value, typename Receiver> class just_state;

    template <typename Value> auto just(Value&& value) -> ::nstd::net::just_sender<Value>;
    template <::std::size_t N> auto just(char const (&value)[N]) -> ::nstd::net::just_sender<::std::string_view>;
}

template <typename Value, typename Receiver>
class nstd::net::just_state
{
private:
    Value    d_value;
    Receiver d_receiver;
    
public:
    just_state(auto&& value, Receiver receiver)
        : d_value(::nstd::utility::move(value))
        , d_receiver(::nstd::utility::forward<Receiver>(receiver)) {
    }
    void start() noexcept try {
        ::nstd::execution::set_value(::nstd::utility::move(this->d_receiver), ::nstd::utility::move(this->d_value));
    }
    catch (...) {
        ::nstd::execution::set_error(::nstd::utility::move(this->d_receiver), ::std::current_exception());
    }
};

template <typename Value>
class nstd::net::just_sender
{
private:
    ::std::remove_cvref_t<Value> d_value;

public:
    explicit just_sender(Value value): d_value(::nstd::utility::forward<Value>(value)) {}

    template <typename Receiver>
    just_state<Value, Receiver> connect(Receiver&& receiver) {
        return { ::nstd::utility::move(this->d_value), ::nstd::utility::forward<Receiver>(receiver) };
    }
};

template <typename Value>
auto nstd::net::just(Value&& value) -> ::nstd::net::just_sender<Value>
{
    return just_sender(::nstd::utility::forward<Value>(value));
}

template <::std::size_t N>
auto nstd::net::just(char const (&value)[N]) -> ::nstd::net::just_sender<::std::string_view>
{
    return ::nstd::net::just(std::string_view(value));
}

// ----------------------------------------------------------------------------

#endif
