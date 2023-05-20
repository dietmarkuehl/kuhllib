// nstd/functional/tag_invoke.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_TAG_INVOKE
#define INCLUDED_NSTD_FUNCTIONAL_TAG_INVOKE

#include "nstd/type_traits/decay.hpp"
#include "nstd/utility/forward.hpp"
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace nstd_tag_invoke_detail {
    void tag_invoke();

    struct tag_invoke_cpo{
        template <typename Tag, typename... Args>
            requires requires(Tag tag, Args&&... args) {
                tag_invoke(tag, ::nstd::utility::forward<Args>(args)...);
            }
        decltype(auto) operator()(Tag tag, Args&&... args) const
            noexcept(noexcept(tag_invoke(tag, ::nstd::utility::forward<Args>(args)...)))
        {
            return tag_invoke(tag, ::nstd::utility::forward<Args>(args)...);
        }
    };
}
namespace nstd {
    inline namespace tag_invoke_namespace {
        inline constexpr ::nstd_tag_invoke_detail::tag_invoke_cpo tag_invoke{};
    }

    template <auto& Tag>
    using tag_t = ::nstd::type_traits::decay_t<decltype(Tag)>;

    template <typename Fun, typename... Args>
    concept invocable
        = requires(Fun fun, Args&&... args) {
            fun(::nstd::utility::forward<Args>(args)...);
        }
        ;

    template <typename Tag, typename... Args>
    concept tag_invocable
        = ::nstd::invocable<decltype(::nstd::tag_invoke), Tag, Args...>
        ;

    template <typename Tag, typename... Args>
    concept nothrow_tag_invocable
        =  ::nstd::tag_invocable<Tag, Args...>
        && ::std::is_nothrow_invocable_v<decltype(::nstd::tag_invoke), Tag, Args...>
        ;

    template <typename Tag, typename... Args>
    using tag_invoke_result = ::std::invoke_result<decltype(::nstd::tag_invoke), Tag, Args...>;

    template <typename Tag, typename... Args>
    using tag_invoke_result_t = ::std::invoke_result_t<decltype(::nstd::tag_invoke), Tag, Args...>;
}

// ----------------------------------------------------------------------------

#endif
