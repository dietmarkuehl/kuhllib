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
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/make_completion_signatures.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/hidden_names/compl_sig.hpp"
#include "nstd/type_traits/conditional.hpp"
#include "nstd/type_traits/copy_cvref.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include <functional>

// ----------------------------------------------------------------------------
// [exec.then]

namespace nstd::hidden_names::then {
    template<typename> struct cpo;

    template <typename Tag, typename Sender, typename Fun>
    concept has_custom_scheduler_then
        =  ::nstd::execution::sender<Sender>
        && requires (Sender&& sender, Fun&& fun) {
            {
                ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo<Tag> const&>(),
                                   ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                   ::nstd::utility::forward<Sender>(sender),
                                   ::nstd::utility::forward<Fun>(fun))
            } -> nstd::execution::sender;
        }
        ;

    template <typename Tag, typename Sender, typename Fun>
    concept has_custom_then
        =  ::nstd::execution::sender<Sender>
        && requires (Sender&& sender, Fun&& fun) {
            {
                ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo<Tag> const&>(),
                                   ::nstd::utility::forward<Sender>(sender),
                                   ::nstd::utility::forward<Fun>(fun))
            } -> nstd::execution::sender;
        }
        ;

    template <typename Tag, ::nstd::execution::receiver Receiver, typename Fun>
    struct receiver {
        ::nstd::type_traits::remove_cvref_t<Receiver> d_receiver;
        ::nstd::type_traits::remove_cvref_t<Fun>      d_fun;

        friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) -> receiver {
            return self;
        }
        template <typename... Args>
        friend auto tag_invoke(Tag, receiver&& self, Args&&... args) noexcept -> void
#if 1 
            requires requires(Fun fun, Args&&... args) { //-dk:TODO this requires shouldn't be necessary!
                ::std::invoke(fun, ::nstd::utility::forward<Args>(args)...);
            }
#endif
        {
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
        template <typename... Args>
            requires (not ::std::is_same_v<Tag, ::nstd::execution::set_value_t>)
        friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, Args&&... args) noexcept
            -> void
        {
            ::nstd::execution::set_value(::nstd::utility::move(self.d_receiver),
                                         ::nstd::utility::forward<Args>(args)...);
        }
        template <typename Error>
            requires (not ::std::is_same_v<Tag, ::nstd::execution::set_error_t>)
        friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, Error&& error) noexcept -> void {
            ::nstd::execution::set_error(::nstd::utility::move(self.d_receiver),
                                         ::nstd::utility::forward<Error>(error));
        }
        friend
        auto tag_invoke(::nstd::type_traits::conditional_t<not ::std::is_same_v<Tag, ::nstd::execution::set_stopped_t>, int, ::nstd::execution::set_stopped_t>,
            receiver&& self) noexcept -> void {
            ::nstd::execution::set_stopped(::nstd::utility::move(self.d_receiver));
        }
    };

    template <typename Tag, ::nstd::execution::sender Sender, typename Fun>
    struct sender: ::nstd::execution::sender_tag {
        template <typename... A>
        using set_value_completions
            = ::nstd::execution::completion_signatures<
                ::nstd::hidden_names::compl_sig_t<::nstd::execution::set_value_t, ::std::invoke_result_t<Fun, A...>>
                >
            ;
        
        template <typename... A>
        using potentially_throwing
            = ::nstd::type_traits::bool_constant<!::std::is_nothrow_invocable_v<Fun, A...>>
            ;
        
        template <typename... B>
        using any_of
            = ::nstd::type_traits::bool_constant<(false || ... || B::value)>
            ;

        template <typename S, typename Env>
        friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, S&&, Env&&) noexcept
            requires ::std::is_same_v<sender, ::nstd::type_traits::remove_cvref_t<S>>
        {
            return ::nstd::execution::make_completion_signatures<
                ::nstd::type_traits::copy_cvref_t<S, Sender>,
                Env,
                ::nstd::type_traits::conditional_t<::nstd::execution::value_types_of_t<Sender, Env, sender::potentially_throwing, sender::any_of>::value,
                                                   ::nstd::execution::completion_signatures<::nstd::execution::set_error_t(::std::exception_ptr)>,
                                                   ::nstd::execution::completion_signatures<>>,
                sender::set_value_completions
                >{};
        }

        ::nstd::type_traits::remove_cvref_t<Sender> d_sender;
        ::nstd::type_traits::remove_cvref_t<Fun>    d_fun;

        template <typename T>
            requires requires(Sender&& sender) {
                ::nstd::execution::get_completion_scheduler<T>(sender);
            }
        friend auto tag_invoke(::nstd::execution::get_completion_scheduler_t<T>, sender const& self) noexcept {
            return ::nstd::execution::get_completion_scheduler<T>(self.d_sender);
        }
        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Receiver&& r)
        {
            return ::nstd::execution::connect(::nstd::utility::move(self.d_sender),
                                              ::nstd::hidden_names::then::receiver<Tag, Receiver, Fun>{
                                                  ::nstd::utility::forward<Receiver>(r),
                                                  ::nstd::utility::move(self.d_fun)
                                              });
        }
        template <::nstd::execution::receiver Receiver>
        friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Receiver r) {
            return ::nstd::execution::connect(self.d_sender,
                                              ::nstd::hidden_names::then::receiver<Tag, Receiver, Fun>{
                                                  ::nstd::utility::forward<Receiver>(r),
                                                  self.d_fun
                                              });
        }
    };

    template <typename Tag>
    struct cpo
        : ::nstd::execution::sender_adaptor_closure<cpo<Tag>>
    {
        using ::nstd::execution::sender_adaptor_closure<cpo<Tag>>::operator();

        template <::nstd::execution::sender Sender, typename Fun>
            requires ::nstd::hidden_names::then::has_custom_scheduler_then<Tag, Sender, Fun>
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }
        template <::nstd::execution::sender Sender, typename Fun>
            requires (not ::nstd::hidden_names::then::has_custom_scheduler_then<Tag, Sender, Fun>)
                && ::nstd::hidden_names::then::has_custom_then<Tag, Sender, Fun>
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }

        template <::nstd::execution::sender Sender, typename Fun>
        auto operator()(Sender&& sender, Fun&& fun) const;
    };
}

// ----------------------------------------------------------------------------

namespace nstd::execution::inline customization_points {
    using then_t = ::nstd::hidden_names::then::cpo<::nstd::execution::set_value_t>;
    inline constexpr then_t then{};
    using upon_error_t = ::nstd::hidden_names::then::cpo<::nstd::execution::set_error_t>;
    inline constexpr upon_error_t upon_error{};
    using upon_stopped_t = ::nstd::hidden_names::then::cpo<::nstd::execution::set_stopped_t>;
    inline constexpr upon_stopped_t upon_stopped{};
}

// ----------------------------------------------------------------------------

template <typename Tag>
    template <::nstd::execution::sender Sender, typename Fun>
auto nstd::hidden_names::then::cpo<Tag>::operator()(Sender&& sender, Fun&& fun) const {
    return nstd::hidden_names::then::sender<Tag, Sender, Fun>{{},
                                            ::nstd::utility::forward<Sender>(sender),
                                            ::nstd::utility::forward<Fun>(fun)
                                            };
}

// ----------------------------------------------------------------------------

#endif
