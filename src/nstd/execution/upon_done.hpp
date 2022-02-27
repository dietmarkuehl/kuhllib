// nstd/execution/upon_done.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_UPON_DONE
#define INCLUDED_NSTD_EXECUTION_UPON_DONE

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/conditional.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/is_void.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <exception>
#include <functional>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct upon_done_t {
        template <::nstd::execution::receiver, typename> struct receiver;
        template <::nstd::execution::sender, typename> struct sender;

        template <::nstd::execution::sender Sender, typename Fun>
        auto operator()(Sender&&, Fun&&) const -> sender<Sender, Fun>;
        template <typename Fun>
        auto operator()(Fun&&) const;
    } upon_done;
}

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver, typename Fun>
struct nstd::execution::upon_done_t::receiver
{
    ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
    ::nstd::type_traits::remove_cvref_t<Fun>      d_fun;

    template <typename Tag, typename... Args>
    friend auto tag_invoke(Tag tag, receiver&& self, Args&&... args) noexcept {
        return ::nstd::tag_invoke(tag, ::nstd::utility::move(self.d_receiver), ::nstd::utility::forward<Args>(args)...);
    }
    friend auto tag_invoke(::nstd::execution::set_done_t, receiver&& self) noexcept -> void {
        try {
            if constexpr (::nstd::type_traits::is_void_v<decltype(::nstd::utility::move(self.d_fun)())>) {
                ::nstd::utility::move(self.d_fun)();
                ::nstd::execution::set_value(::nstd::utility::move(self.d_receiver));
            }
            else {
                ::nstd::execution::set_value(::nstd::utility::move(self.d_receiver), ::nstd::utility::move(self.d_fun)());
            }
        }
        catch (...) {
            ::nstd::execution::set_error(::nstd::utility::move(self.d_receiver), ::std::current_exception());
        }
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender, typename Fun>
struct nstd::execution::upon_done_t::sender
{
    using result_type = decltype(::std::invoke(::nstd::type_traits::declval<Fun>()));
    using non_void_type = ::nstd::type_traits::conditional_t<::nstd::type_traits::is_void_v<result_type>, int, result_type>;
    using completion_signatures = ::nstd::execution::completion_signatures<
            ::nstd::type_traits::conditional_t<::nstd::type_traits::is_void_v<result_type>,
                                               ::nstd::execution::set_value_t(),
                                               ::nstd::execution::set_value_t(non_void_type)>
        >;
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<decltype(::std::invoke(::nstd::type_traits::declval<Fun>()))>>;
    template <template <typename...> class V>
    using error_types = V<::std::exception_ptr>;
    static constexpr bool sends_done = false;

    ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
    ::nstd::type_traits::remove_cvref_t<Fun>    d_fun;

    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& rceiver) noexcept
    {
        return ::nstd::execution::connect(::nstd::utility::move(self.d_sender),
                                          ::nstd::execution::upon_done_t::receiver<Receiver, Fun>{ 
                                              ::nstd::utility::forward<Receiver>(rceiver),
                                              ::nstd::utility::move(self.d_fun)
                                          });
    }
};

// ----------------------------------------------------------------------------

template <::nstd::execution::sender Sender, typename Fun>
auto nstd::execution::upon_done_t::operator()(Sender&& sndr, Fun&& fun) const
    -> nstd::execution::upon_done_t::sender<Sender, Fun>
{
    return { ::nstd::utility::forward<Sender>(sndr), ::nstd::utility::forward<Fun>(fun) };
}

template <typename Fun>
auto nstd::execution::upon_done_t::operator()(Fun&& fun) const
{
    return [fun = ::nstd::utility::forward<Fun>(fun)](::nstd::execution::sender auto&& sndr) mutable {
        return nstd::execution::upon_done(::nstd::utility::forward<decltype(sndr)>(sndr),
                                          ::nstd::utility::move(fun));
    };
}

// ----------------------------------------------------------------------------

#endif
