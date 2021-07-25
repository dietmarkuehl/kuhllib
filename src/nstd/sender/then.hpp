// nstd/sender/then.hpp                                               -*-C++-*-
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

#ifndef INCLUDED_NSTD_SENDER_THEN
#define INCLUDED_NSTD_SENDER_THEN

#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <exception>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template < typename Fun, typename Receiver> class then_receiver;
    template <typename Sender, typename Fun> class then_sender;

    template <typename Sender, typename Fun>
    auto then(Sender&&, Fun&&)
        -> ::nstd::net::then_sender<Sender, Fun>;
    
    template <typename Fun>
    auto then(Fun&& fun) {
        return [fun = ::nstd::utility::forward<Fun>(fun)](auto&& sender){
            return ::nstd::net::then(::nstd::utility::forward<decltype(sender)>(sender),
                                     ::nstd::utility::move(fun));
             };
    }
}

// ----------------------------------------------------------------------------

template <typename Fun, typename Receiver>
class nstd::net::then_receiver
{
public:
    ::nstd::type_traits::remove_cvref_t<Fun>       d_fun;
    ::nstd::type_traits::remove_cvref_t<Receiver>  d_receiver;

public:
    then_receiver(auto&& fun, Receiver&& receiver)
        : d_fun(::nstd::utility::move(fun))
        , d_receiver(::nstd::utility::forward<Receiver>(receiver)) {

    }
    friend auto tag_invoke(::nstd::execution::set_value_t ,
                           then_receiver&&                r,
                           auto&&...                      args) ->void try {
        if constexpr (::std::is_same_v<void, decltype(r.d_fun(::nstd::utility::forward<decltype(args)>(args)...))>) {
            r.d_fun(::nstd::utility::forward<decltype(args)>(args)...);
            ::nstd::execution::set_value(::nstd::utility::move(r.d_receiver));
        }
        else {
            ::nstd::execution::set_value(::nstd::utility::move(r.d_receiver), r.d_fun(::nstd::utility::forward<decltype(args)>(args)...));
        }
    }
    catch (...) {
        ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver), ::std::current_exception());
    }
    friend auto tag_invoke(::nstd::execution::set_error_t ,
                           then_receiver&&                r,
                           auto&&                         arg) noexcept ->void {
        ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver), ::nstd::utility::forward<decltype(arg)>(arg));
    }
    friend auto tag_invoke(::nstd::execution::set_done_t, then_receiver&& r) noexcept -> void {
        ::nstd::execution::set_done(::nstd::utility::move(r.d_receiver));
    }
};

// ----------------------------------------------------------------------------

template <typename Sender, typename Fun>
class nstd::net::then_sender
    : public ::nstd::execution::sender_base
{
private:
    ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
    ::nstd::type_traits::remove_cvref_t<Fun>    d_fun;

public:
    then_sender(Sender sender, Fun fun)
        : d_sender(::nstd::utility::forward<Sender>(sender))
        , d_fun(::nstd::utility::forward<Fun>(fun)) {
    }

    template <typename Receiver>
    auto friend tag_invoke(::nstd::execution::connect_t, then_sender&& sender, Receiver&& receiver) {
        return ::nstd::execution::connect(
            ::nstd::utility::move(sender.d_sender),
            ::nstd::net::then_receiver<Fun, Receiver>(::nstd::utility::move(sender.d_fun),
                                                      ::nstd::utility::forward<Receiver>(receiver)));
    }
};

// ----------------------------------------------------------------------------

template <typename Sender, typename Fun>
auto nstd::net::then(Sender&& sender, Fun&& fun)
    -> ::nstd::net::then_sender<Sender, Fun>
{
    return { ::nstd::utility::forward<Sender>(sender), ::nstd::utility::forward<Fun>(fun) };
}

// ----------------------------------------------------------------------------

#endif
