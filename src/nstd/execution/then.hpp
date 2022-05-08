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
#include "nstd/execution/get_completion_scheduler.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::then {
    struct sender {
        using completion_signatures = ::nstd::execution::completion_signatures<>;
    };
    struct cpo {
        template <::nstd::execution::sender Sender, typename Fun>
            requires requires (Sender&& sender, Fun&& fun) {
                {
                    ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo const&>(),
                                       ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                       ::nstd::utility::forward<Sender>(sender),
                                       ::nstd::utility::forward<Fun>(fun))
                } -> nstd::execution::sender;
            }
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }
        template <::nstd::execution::sender Sender, typename Fun>
            requires
                (not requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo const&>(),
                                           ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> nstd::execution::sender;
                })
                && requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo const&>(),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> nstd::execution::sender;
                }
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::tag_invoke(*this,
                                      ::nstd::utility::forward<Sender>(sender),
                                      ::nstd::utility::forward<Fun>(fun));
        }
        template <::nstd::execution::sender Sender, typename Fun>
            requires
                (not requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo const&>(),
                                           ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> nstd::execution::sender;
                })
                && (not requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::hidden_names::then::cpo const&>(),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> nstd::execution::sender;
                })
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::hidden_names::then::sender(
                    ::nstd::utility::forward<Sender>(sender),
                    ::nstd::utility::forward<Fun>(fun)
                );
        }
        template <typename Fun>
        auto operator()(Fun&& fun) const;
    };
}

// ----------------------------------------------------------------------------

namespace nstd::execution {
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
