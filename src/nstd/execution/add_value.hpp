// nstd/execution/add_value.hpp                                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_EXECUTION_ADD_VALUE
#define INCLUDED_NSTD_EXECUTION_ADD_VALUE

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/make_completion_signatures.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/get_env.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/get_attrs.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::add_value {
    struct cpo {
        template <::nstd::execution::receiver Downstream, typename... Values>
        struct receiver
        {
            ::nstd::type_traits::remove_cvref_t<Downstream>              d_downstream;
            ::std::tuple<::nstd::type_traits::remove_cvref_t<Values>...> d_values;

            friend auto tag_invoke(::nstd::execution::get_env_t, receiver const& self) noexcept {
                return ::nstd::execution::get_env(self.d_downstream);
            }

            template <typename... Args>
            friend auto tag_invoke(::nstd::execution::set_value_t, receiver&& self, Args&&... args) noexcept {
                ::std::apply([&](auto&&... values){
                    ::nstd::execution::set_value(
                        ::nstd::utility::move(self.d_downstream),
                        ::nstd::utility::forward<Args>(args)...,
                        ::nstd::utility::move(values)...
                    );
                }, ::nstd::utility::move(self.d_values));
            }
            template <typename Error>
            friend auto tag_invoke(::nstd::execution::set_error_t, receiver&& self, Error&& error) noexcept {
                ::nstd::execution::set_error(
                    ::nstd::utility::move(self.d_downstream),
                    ::nstd::utility::forward<Error>(error)
                    );
            }
            template <typename Error>
            friend auto tag_invoke(::nstd::execution::set_stopped_t, receiver&& self) noexcept {
                ::nstd::execution::set_stopped(::nstd::utility::move(self.d_downstream));
            }
        };

        template <::nstd::execution::sender Upstream, typename... Values>
        struct sender
        {
            template <typename... A>
            using make_set_value = ::nstd::execution::completion_signatures<::nstd::execution::set_value_t(A..., Values...)>;

            ::nstd::type_traits::remove_cvref_t<Upstream>                d_upstream;
            ::std::tuple<::nstd::type_traits::remove_cvref_t<Values>...> d_values;

            friend auto tag_invoke(::nstd::execution::get_attrs_t, sender const& self) noexcept {
                return ::nstd::execution::get_attrs(self.d_upstream);
            };

            template <typename Env>
            friend auto tag_invoke(::nstd::execution::get_completion_signatures_t, sender const&, Env&&) noexcept {
                return ::nstd::execution::make_completion_signatures<
                    Upstream,
                    Env,
                    ::nstd::execution::completion_signatures<>,
                    make_set_value
                    >{};
            }

            template <::nstd::execution::receiver Rcvr>
            friend auto tag_invoke(::nstd::execution::connect_t, sender&& self, Rcvr&& rcvr)
            {
                return ::nstd::execution::connect(
                    ::nstd::utility::move(self.d_upstream),
                    receiver<Rcvr, Values...>{
                        ::nstd::utility::forward<Rcvr>(rcvr),
                        ::nstd::utility::move(self.d_values)
                    }
                );
            }
            template <::nstd::execution::receiver Rcvr>
            friend auto tag_invoke(::nstd::execution::connect_t, sender const& self, Rcvr&& rcvr)
            {
                return ::nstd::execution::connect(
                    self.d_upstream,
                    receiver<Rcvr, Values...>{ ::nstd::utility::forward<Rcvr>(rcvr), self.d_values }
                );
            }

        };
        template <typename Upstream, typename... Values>
        friend auto tag_invoke(::nstd::hidden_names::add_value::cpo, Upstream&& upstream, Values&&... values)
            -> ::nstd::hidden_names::add_value::cpo::sender<Upstream, Values...>
        {
            static_assert(::nstd::execution::sender<Upstream>);
            return {
                ::nstd::utility::forward<Upstream>(upstream),
                { ::nstd::utility::forward<Values>(values)... }
                };
        }

        template <::nstd::execution::sender Upstream, typename... Values>
        auto operator()(Upstream&& upstream, Values&&... values) const
        {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Upstream>(upstream),
                                      ::nstd::utility::forward<Values>(values)...);
        }

        template <typename... Values>
        struct closure
            : ::nstd::execution::sender_tag
        {
            ::std::tuple<::nstd::type_traits::remove_cvref_t<Values>...> d_values;
            template <::nstd::execution::sender Upstream>
            auto operator()(Upstream&& upstream) && {
                return ::std::apply([&](auto&&... values){
                    return cpo{}(
                        ::nstd::utility::forward<Upstream>(upstream),
                        ::nstd::utility::move(values)...
                    );
                }, ::nstd::utility::move(this->d_values));
            }
        };
        template <typename... Values>
        auto operator()(Values&&... values) const
            -> closure<Values...>
        {
            return { {}, { ::nstd::utility::forward<Values>(values)... } };
        }
    };
}

namespace nstd::execution {
    using add_value_t = ::nstd::hidden_names::add_value::cpo;
    inline constexpr add_value_t add_value{};
}

// ----------------------------------------------------------------------------

#endif
