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

#include "nstd/functional/tag_invoke.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/value_types_of_t.hpp"
#include "nstd/hidden_names/decayed_tuple.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/type_traits/type_identity.hpp"
#include <exception>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <tuple>
#include <variant>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename...> struct type_identity_or_monostate;
    template <> struct type_identity_or_monostate<> { using type = ::std::monostate; };
    template <typename T> struct type_identity_or_monostate<T> { using type = T; };

    template <typename... T>
    using type_identity_or_monostate_t = typename ::nstd::hidden_names::type_identity_or_monostate<T...>::type;

    template <::nstd::execution::sender S, typename sync_wait_env>
    using sync_wait_type = ::std::optional<
        ::nstd::execution::value_types_of_t<S, sync_wait_env, ::nstd::hidden_names::decayed_tuple, ::nstd::type_traits::type_identity_t>
        >;
}
namespace nstd::this_thread {
    inline constexpr struct sync_wait_t {
        template <::nstd::execution::sender Sender>
            requires requires(Sender&& s, nstd::this_thread::sync_wait_t const& sync_wait) {
                {
                    ::nstd::tag_invoke(sync_wait,
                                      ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(s),
                                      s)
                } //-dk:TODO -> ::nstd::hidden_names::sync_wait_type<Sender>
                ;
            }
        auto operator()(Sender&& s) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(s),
                                      s);
        }

        template <::nstd::execution::sender Sender>
            requires requires(Sender&& s, nstd::this_thread::sync_wait_t const& sync_wait) {
                {
                    ::nstd::tag_invoke(sync_wait, s)
                } //-dk:TODO -> ::nstd::hidden_names::sync_wait_type<Sender>
                ;
            }
        auto operator()(Sender&& s) const {
            return ::nstd::tag_invoke(*this, s);
        }

        template <typename Type>
        struct receiver {
            ::std::mutex*                           bottleneck;
            ::std::condition_variable*              condition;
            bool*                                   done;
            Type*                                   res;
            ::std::optional<::std::exception_ptr>*  ex;
            void complete() const {
                (::std::lock_guard<::std::mutex>(*bottleneck)), (*done = true);
                condition->notify_one();
            }

            friend auto tag_invoke(::nstd::execution::set_value_t, receiver&&  r) noexcept {
                (*r.res) = {};
                r.complete();
            }
            friend auto tag_invoke(::nstd::execution::set_value_t, receiver&&  r, Type const& t) noexcept {
                (*r.res) = t;
                r.complete();
            }
            friend auto tag_invoke(::nstd::execution::set_value_t, receiver&&  r, Type&& t) noexcept {
                (*r.res) = ::nstd::utility::move(t);
                r.complete();
            }
            friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& r, ::std::exception_ptr ex) noexcept {
                (*r.ex) = ex;
                r.complete();
            }
            template <typename E>
            friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& r, E ex) noexcept {
                try { throw ex; }
                catch (...) { (*r.ex) = ::std::current_exception(); }
                r.complete();
            }
            friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& r) noexcept {
                r.complete();
            }
        };

        template <::nstd::execution::sender Sender>
        auto operator()(Sender&& s) const -> ::nstd::hidden_names::sync_wait_type<Sender> {
            using type = ::nstd::hidden_names::sync_wait_type<Sender>;

            ::std::mutex                          bottleneck;
            ::std::condition_variable             condition;
            bool                                  done(false);
            type                                  res(std::nullopt);
            ::std::optional<::std::exception_ptr> ex;

            auto state = ::nstd::execution::connect(::nstd::utility::forward<Sender>(s),
                                                    receiver<type>{&bottleneck, &condition, &done, &res, &ex});
            ::nstd::execution::start(state);

            ::std::unique_lock kerberos(bottleneck);
            condition.wait(kerberos, [&done]{ return done; });
            if (ex) {
                ::std::rethrow_exception(*ex);
            }
            return res;
        }
    } sync_wait;
}

// ----------------------------------------------------------------------------

#endif
