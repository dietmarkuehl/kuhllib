// nstd/execution/sender_adaptor_closure.hpp                          -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_EXECUTION_SENDER_ADAPTOR_CLOSURE
#define INCLUDED_NSTD_EXECUTION_SENDER_ADAPTOR_CLOSURE

#include "nstd/execution/sender.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/type_traits/decay.hpp"
#include <tuple>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::sender_adaptor_closure_ns
{
    template <typename C, typename... T>
    struct adaptor
    {
        ::std::tuple<::nstd::type_traits::decay_t<T>...> args;
        template <::nstd::execution::sender Sender>
        auto operator()(Sender&& sender) {
            return ::std::apply([&sender](auto&&... a){ return C()(::nstd::utility::forward<Sender>(sender), a...); }, this->args);
        }
    };

    template <typename C>
    struct sender_adaptor_closure {
        template <typename... A>
        auto operator()(A&&... a) const {
            return ::nstd::hidden_names::sender_adaptor_closure_ns::adaptor<C, A...>{{::nstd::utility::forward<A>(a)...}};
        }
    };

    template <::nstd::execution::sender Sender, typename... T>
    auto operator| (Sender&& sender, ::nstd::hidden_names::sender_adaptor_closure_ns::adaptor<T...>&& a) {
        return a(::nstd::utility::forward<Sender>(sender));
    }
}

namespace nstd::execution {
    using ::nstd::hidden_names::sender_adaptor_closure_ns::sender_adaptor_closure;
}

// ----------------------------------------------------------------------------

#endif
