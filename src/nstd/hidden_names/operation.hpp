// nstd/hidden_names/operation.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_OPERATION
#define INCLUDED_NSTD_HIDDEN_NAMES_OPERATION

#include "nstd/execution/connect.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_base.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/hidden_names/type_list.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <exception>
#include <tuple>

// ----------------------------------------------------------------------------
// The idea of operation is to factor out the creation of a pipeable
// sender and only provide the eventual operation. The class template
// operation does the necessary scaffolding. An example use could be
//
//    struct accept_t {};
//    inline constexpr
//    operation<accept_t,
//              std::variant<std::tuple<socket>>,
//              std::tuple<std::error_code>>
//        accept;

namespace nstd::hidden_names {
    template <typename Tag, typename Results, typename Errors>
    class operation;

    template <typename Tag, typename Receiver, typename... Args>
    struct operation_receiver;
}


template <typename Tag, typename Results, typename Errors>
class nstd::hidden_names::operation
{
public:
    template <typename Sender, typename... Args>
    struct sender;

    template <::nstd::execution::sender Sender, typename... Args>
    auto operator()(Sender&& s, Args&&... args) const
        -> sender<::nstd::type_traits::remove_cvref_t<Sender>,
                  ::nstd::type_traits::remove_cvref_t<Args>...>;
    template <typename... Args>
    auto operator()(Args&&... args) const
        -> sender<decltype(::nstd::execution::just()),
                  ::nstd::type_traits::remove_cvref_t<Args>...>;
};

// ----------------------------------------------------------------------------

template <typename Tag, typename Receiver, typename... Args>
struct nstd::hidden_names::operation_receiver {
    ::std::tuple<Args...>                         d_args;
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    template <typename... A>
    friend auto tag_invoke(::nstd::execution::set_value_t, operation_receiver&& r, A&&... a) noexcept {
        ::std::apply([&r, &a...](auto&&... args){
                ::nstd::tag_invoke(Tag(),
                ::nstd::utility::forward<Args>(args)...,
                ::nstd::utility::forward<A>(a)...,
                ::nstd::utility::move(r.d_receiver));
        }, ::nstd::utility::move(r.d_args));
    }
    friend auto tag_invoke(::nstd::execution::set_error_t, operation_receiver&& r, ::std::exception_ptr ex) noexcept {
        ::nstd::execution::set_error(::nstd::utility::move(r.d_receiver), ex);
    }
    friend auto tag_invoke(::nstd::execution::set_done_t, operation_receiver&& r) noexcept {
        ::nstd::execution::set_done(::nstd::utility::move(r.d_receiver));
    }
};

// ----------------------------------------------------------------------------

template <typename Tag, typename Results, typename Errors>
    template <typename Sender, typename... Args>
struct nstd::hidden_names::operation<Tag, Results, Errors>::sender
    : ::nstd::execution::piped_sender_base
{
    template <template <typename...> class V, template <typename...> class T>
    using value_types = typename ::nstd::hidden_names::type_list_nested_transform<V, T, Results>::types;
    template <template <typename...> class V>
    using error_types = typename ::nstd::hidden_names::type_list_transform<V, Errors>::types;
    static constexpr bool sends_done = false;

    Sender                 d_sender;
    ::std::tuple<Args...>  d_args;

    template <::nstd::execution::sender S>
    auto operator()(S&& s) && -> sender<::nstd::type_traits::remove_cvref_t<S>, Args...> {
        return { {}, ::nstd::utility::forward<S>(s), ::nstd::utility::move(this->d_args) };
    }
    template <::nstd::execution::sender S>
    auto operator()(S&& s) const& -> sender<::nstd::type_traits::remove_cvref_t<S>, Args...> {
        return { {}, ::nstd::utility::forward<S>(s), this->d_args };
    }
    template <typename Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& s, Receiver&& r) noexcept {
        return ::nstd::execution::connect(
                              ::nstd::utility::move(s.d_sender),
                              operation_receiver<Tag, Receiver, Args...>{
                                  ::nstd::utility::move(s.d_args),
                                  ::nstd::utility::forward<Receiver>(r)
                              });
    }
};

// ----------------------------------------------------------------------------

template <typename Tag, typename Results, typename Errors>
    template <::nstd::execution::sender Sender, typename... Args>
auto nstd::hidden_names::operation<Tag, Results, Errors>::operator()(Sender&& s, Args&&... args) const
        -> sender<::nstd::type_traits::remove_cvref_t<Sender>,
                  ::nstd::type_traits::remove_cvref_t<Args>...>
{
    return {
        {},
        ::nstd::utility::forward<Sender>(s),
        ::std::tuple<::nstd::type_traits::remove_cvref_t<Args>...>(::nstd::utility::forward<Args>(args)...)
        };
}

template <typename Tag, typename Results, typename Errors>
    template <typename... Args>
auto nstd::hidden_names::operation<Tag, Results, Errors>::operator()(Args&&... args) const
    -> sender<decltype(::nstd::execution::just()),
              ::nstd::type_traits::remove_cvref_t<Args>...> {
    return {
        {},
        ::nstd::execution::just(),
        ::std::tuple<::nstd::type_traits::remove_cvref_t<Args>...>(::nstd::utility::forward<Args>(args)...)
        };
}

// ----------------------------------------------------------------------------

#endif
