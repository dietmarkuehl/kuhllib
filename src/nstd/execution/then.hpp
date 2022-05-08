// nstd/execution/then.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_THEN
#define INCLUDED_NSTD_EXECUTION_THEN

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

// ----------------------------------------------------------------------------
// [exec.then]

namespace nstd::hidden_names::then {
    struct cpo;

    template <typename Sender, typename Fun>
    concept has_custom_scheduler_then
        =  ::nstd::execution::sender<Sender>
        && requires (Sender&& sender, Fun&& fun) {
            {
                ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo const&>(),
                                   ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                   ::nstd::utility::forward<Sender>(sender),
                                   ::nstd::utility::forward<Fun>(fun))
            } -> nstd::execution::sender;
        }
        ;

    template <typename Sender, typename Fun>
    concept has_custom_then
        =  ::nstd::execution::sender<Sender>
        && requires (Sender&& sender, Fun&& fun) {
            {
                ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo const&>(),
                                   ::nstd::utility::forward<Sender>(sender),
                                   ::nstd::utility::forward<Fun>(fun))
            } -> nstd::execution::sender;
        }
        ;

    template <::nstd::execution::receiver Receiver, typename Fun>
    struct receiver {
        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        ::nstd::type_traits::remove_cvref_t<Fun>      d_fun;

        friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) -> receiver {
            return self;
        }
        template <typename... Args>
        friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, Args&&... args) noexcept -> void {
            try {
                if constexpr (::nstd::concepts::same_as<void, decltype(::std::invoke(self.d_fun, ::nstd::utility::forward<Args>(args)...))>) {
                    ::std::invoke(self.d_fun, ::nstd::utility::forward<Args>(args)...);
                    ::nstd::execution::set_value(::nstd::utility::move(self.d_receiver));
                }
                else {
                    ::nstd::execution::set_value(::nstd::utility::move(self.d_receiver),
                                                 ::std::invoke(self.d_fun, ::nstd::utility::forward<Args>(args)...));
                }
            }
            catch (...) {
                ::nstd::execution::set_error(::nstd::utility::move(self.d_receiver),
                                             ::std::current_exception());
            }
        }
        template <typename Error>
        friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, Error&& error) noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(self.d_receiver),
                                         ::nstd::utility::forward<Error>(error));
        }
        friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self) noexcept -> void {
            ::nstd::execution::set_stopped(::nstd::utility::move(self.d_receiver));
        }
    };

    template <::nstd::execution::sender Sender, typename Fun>
    struct sender: ::nstd::execution::sender_tag {
        using completion_signatures = ::nstd::execution::completion_signatures</*-dk:TODO sort of the completion signatures*/>;

        ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
        ::nstd::type_traits::remove_cvref_t<Fun>    d_fun;

        template <typename Tag>
        friend auto tag_invoke(::nstd::execution::get_completion_scheduler_t<Tag>, sender const& self) noexcept {
            return ::nstd::execution::get_completion_scheduler_t<Tag>(self.d_sender);
        }
        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver r) {
            return ::nstd::execution::connect(::nstd::utility::move(self.d_sender),
                                              ::nstd::hidden_names::then::receiver<Receiver, Fun>{
                                                  ::nstd::utility::forward<Receiver>(r),
                                                  ::nstd::utility::move(self.d_fun)
                                              });
        }
        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Receiver r) {
            return ::nstd::execution::connect(self.d_sender,
                                              ::nstd::hidden_names::then::receiver<Receiver, Fun>{
                                                  ::nstd::utility::forward<Receiver>(r),
                                                  self.d_fun
                                              });
        }
    };

    struct cpo {
        template <::nstd::execution::sender Sender, typename Fun>
            requires ::nstd::hidden_names::then::has_custom_scheduler_then<Sender, Fun>
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }
        template <::nstd::execution::sender Sender, typename Fun>
            requires (not ::nstd::hidden_names::then::has_custom_scheduler_then<Sender, Fun>)
                && ::nstd::hidden_names::then::has_custom_then<Sender, Fun>
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }

        template <::nstd::execution::sender Sender, typename Fun>
        auto operator()(Sender&& sender, Fun&& fun) const;

        template <typename Fun>
        auto operator()(Fun&& fun) const;
    };
}

// ----------------------------------------------------------------------------

namespace nstd::execution::inline customization_points {
    using then_t = ::nstd::hidden_names::then::cpo;
    inline constexpr then_t then{};
}

// ----------------------------------------------------------------------------

template <typename Fun>
auto nstd::hidden_names::then::cpo::operator()(Fun&& fun) const {
    return [fun = ::nstd::utility::forward<Fun>(fun)](::nstd::execution::sender auto&& s) mutable {
        return ::nstd::execution::then(::nstd::utility::forward<decltype(s)>(s),
                                       ::nstd::utility::move(fun));
    };
}

// ----------------------------------------------------------------------------

#endif
