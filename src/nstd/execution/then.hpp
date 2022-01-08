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

#include "nstd/execution/sender.hpp"
#include "nstd/execution/lazy_then.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct then_t {
        template <::nstd::execution::sender Sender, typename Fun>
            requires requires (Sender&& sender, Fun&& fun) {
                {
                    ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::then_t const&>(),
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
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::then_t const&>(),
                                           ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> nstd::execution::sender;
                })
                && requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::then_t const&>(),
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
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::then_t const&>(),
                                           ::nstd::execution::get_completion_scheduler<::nstd::execution::set_value_t>(sender),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> nstd::execution::sender;
                })
                && (not requires (Sender&& sender, Fun&& fun) {
                    {
                        ::nstd::tag_invoke(::nstd::type_traits::declval<::nstd::execution::then_t const&>(),
                                           ::nstd::utility::forward<Sender>(sender),
                                           ::nstd::utility::forward<Fun>(fun))
                    } -> nstd::execution::sender;
                })
        auto operator()(Sender&& sender, Fun&& fun) const {
            return ::nstd::execution::lazy_then(
                    ::nstd::utility::forward<Sender>(sender),
                    ::nstd::utility::forward<Fun>(fun)
                );
        }
        template <typename Fun>
        auto operator()(Fun&& fun) const;
    } then;
}

// ----------------------------------------------------------------------------

template <typename Fun>
auto nstd::execution::then_t::operator()(Fun&& fun) const {
    return [fun = ::nstd::utility::forward<Fun>(fun)](::nstd::execution::sender auto&& s) mutable {
        return ::nstd::execution::then(::nstd::utility::forward<decltype(s)>(s),
                                       ::nstd::utility::move(fun));
    };
}

// ----------------------------------------------------------------------------

#endif
