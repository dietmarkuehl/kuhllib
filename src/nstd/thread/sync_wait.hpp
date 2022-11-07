// nstd/thread/sync_wait.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_THREAD_SYNC_WAIT
#define INCLUDED_NSTD_THREAD_SYNC_WAIT

#include "nstd/concepts/same_as.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/get_scheduler.hpp"
#include "nstd/execution/get_delegatee_scheduler.hpp"
#include "nstd/execution/run_loop.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/value_types_of_t.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/hidden_names/decayed_tuple.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/type_identity.hpp"

#include <exception>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::sync_wait {
    struct env {
        using scheduler_type = decltype(::nstd::execution::run_loop().get_scheduler());
        scheduler_type d_scheduler;

        env(): d_scheduler(::nstd::execution::run_loop().get_scheduler()) {}
        friend auto tag_invoke(::nstd::execution::get_scheduler_t, env const& self) noexcept
            -> scheduler_type { return self.d_scheduler; }
        friend auto tag_invoke(::nstd::execution::get_delegatee_scheduler_t, env const& self) noexcept
            -> scheduler_type { return self.d_scheduler; }
    };

    struct receiver {
        ::nstd::hidden_names::sync_wait::env d_env;
        friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self)
            -> ::nstd::hidden_names::sync_wait::env { return self.d_env; }
    };

    template <typename...> struct type_identity_or_monostate;
    template <> struct type_identity_or_monostate<> { using type = ::std::monostate; };
    template <typename T> struct type_identity_or_monostate<T> { using type = T; };
    template <typename... T>
    using type_identity_or_monostate_t = typename ::nstd::hidden_names::sync_wait::type_identity_or_monostate<T...>::type;

    // template <::nstd::execution::sender<::nstd::hidden_names::sync_wait::env> Sender>
    template <::nstd::execution::sender Sender>
    using result_type
        = ::nstd::execution::value_types_of_t<
            Sender,
            ::nstd::hidden_names::sync_wait::env,
            ::nstd::hidden_names::decayed_tuple,
            ::nstd::hidden_names::sync_wait::type_identity_or_monostate_t>;
    template <::nstd::execution::sender<::nstd::hidden_names::sync_wait::env> Sender>
    using return_type = ::std::optional<result_type<Sender>>;

    struct cpo;

    template <typename Sender>
    concept has_sync_wait_with_scheduler
        =  ::nstd::execution::sender<Sender>
        && requires(Sender&& s, nstd::hidden_names::sync_wait::cpo const& sync_wait) {
            {
                ::nstd::tag_invoke(sync_wait,
                                  ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(s),
                                  ::nstd::utility::forward<Sender>(s))
            } -> nstd::concepts::same_as<::nstd::hidden_names::sync_wait::return_type<Sender>>
            ;
        }
        ;

    template <typename Sender>
    concept has_sync_wait_with_sender
        =  ::nstd::execution::sender<Sender>
        && requires(Sender&& s, nstd::hidden_names::sync_wait::cpo const& sync_wait) {
            {
                ::nstd::tag_invoke(sync_wait, ::nstd::utility::forward<Sender>(s))
            } -> nstd::concepts::same_as<::nstd::hidden_names::sync_wait::return_type<Sender>>
            ;
        }
        ;

    struct cpo {
        template <::nstd::execution::sender Sender>
            requires has_sync_wait_with_scheduler<Sender>
        auto operator()(Sender&& s) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(s),
                                      ::nstd::utility::forward<Sender>(s));
        }

        template <::nstd::execution::sender Sender>
            requires has_sync_wait_with_sender<Sender> &&  (not has_sync_wait_with_scheduler<Sender>)
        auto operator()(Sender&& s) const {
            return ::nstd::tag_invoke(*this, ::nstd::utility::forward<Sender>(s));
        }

        template <typename Type>
        struct receiver_impl {
            struct hidden {
                ::std::mutex*               bottleneck;
                ::std::condition_variable*  condition;
                bool*                       done;
                ::std::optional<Type>*      res;
                ::std::exception_ptr*       ex;
                void complete() const {
                    (::std::lock_guard<::std::mutex>(*bottleneck)), (*done = true);
                    condition->notify_one();
                }

                friend auto tag_invoke(::nstd::execution::get_env_t, hidden const& ) noexcept
                    -> ::nstd::hidden_names::sync_wait::env {
                    return {};
                }
                friend auto tag_invoke(::nstd::execution::set_value_t, hidden&&  self) noexcept {
                    (*self.res) = {};
                    self.complete();
                }
                template <typename... Args>
                    requires ::std::is_assignable_v<::std::optional<Type>&, ::std::tuple<Args...>>
                friend auto tag_invoke(::nstd::execution::set_value_t, hidden&&  self, Args&&... args) noexcept {
                    self.res->emplace(::nstd::utility::forward<Args>(args)...);
                    self.complete();
                }
                friend auto tag_invoke(::nstd::execution::set_error_t, hidden&& self, ::std::exception_ptr ex) noexcept {
                    (*self.ex) = ex;
                    self.complete();
                }
                template <typename E>
                friend auto tag_invoke(::nstd::execution::set_error_t, hidden&& self, E ex) noexcept {
                    *self.ex = ::std::make_exception_ptr(ex);
                    self.complete();
                }
                friend auto tag_invoke(::nstd::execution::set_stopped_t, hidden&& self) noexcept {
                    self.complete();
                }
            };
        };
        template <typename Type>
        using receiver = typename receiver_impl<Type>::hidden;

        template <::nstd::execution::sender Sender>
        auto operator()(Sender&& s) const -> ::nstd::hidden_names::sync_wait::return_type<Sender> {
            using type = ::nstd::hidden_names::sync_wait::result_type<Sender>;

            ::nstd::hidden_names::sync_wait::return_type<Sender> res(std::nullopt);
            // ::nstd::execution::run_loop loop;
            // auto                        sender(::nstd::execution::schedule(loop.get_scheduler()));

            ::std::mutex                 bottleneck;
            ::std::condition_variable    condition;
            bool                         done(false);
            ::std::exception_ptr         ex;

            auto state = ::nstd::execution::connect(::nstd::utility::forward<Sender>(s),
                                                    receiver<type>{&bottleneck, &condition, &done, &res, &ex});
            ::nstd::execution::start(state);

            ::std::unique_lock kerberos(bottleneck);
            condition.wait(kerberos, [&done]{ return done; });
            if (ex) {
                ::std::rethrow_exception(ex);
            }
            return res;
        }
    };
}

namespace nstd::this_thread::inline customization_points {
    using sync_wait_t = nstd::hidden_names::sync_wait::cpo;
    inline constexpr sync_wait_t sync_wait{};
}

// ----------------------------------------------------------------------------

#endif
