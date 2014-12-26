// nstd/functional/invoke.hpp                                         -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_INVOKE
#define INCLUDED_NSTD_FUNCTIONAL_INVOKE

#include "nstd/functional/mem_fn.hpp"
#include "nstd/type_traits/choose_type.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/decay.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/is_member_pointer.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        namespace detail {
            struct invoke_empty {};
            template <bool, typename, typename...> struct invoke_non_member_type;
            template <typename Fun, typename... Args> struct invoke_non_member_type<true, Fun, Args...>;
            template <typename Fun, typename... Args> struct invoke_non_member_type<false, Fun, Args...>;
            template <typename, typename...> struct invoke_non_member;

            template <bool, bool, typename, typename, typename...> struct invoke_member_fun_type;
            template <typename Fun, typename Class, typename Obj, typename... Args>
                struct invoke_member_fun_type<true,  true, Fun Class::*, Obj, Args...>;
            template <typename Fun, typename Obj, typename... Args>
                struct invoke_member_fun_type<true, false, Fun, Obj, Args...>;
            template <typename, typename...> struct invoke_member_fun;
            template <typename Fun, typename Class, typename Object, typename... Args>
                struct invoke_member_fun<Fun Class::*, Object, Args...>;

#if 0
            template <typename, typename...> struct invoke_member;
            template <typename Fun, typename Type, typename Object, typename... Args>
            struct invoke_member<Fun Type::*, Object, Args...>;
#endif

            template <typename, typename...> struct invoke;

            template <typename Fun, typename... Args>
            using invoke_type = ::nstd::type_traits::choose_type_t<
                typename ::nstd::functional::detail::invoke_member_fun<Fun, Args...>::choice_type,
                typename ::nstd::functional::detail::invoke_non_member<Fun, Args...>::choice_type
                >;
        }
        template <typename Fun, typename... Args>
        auto invoke(Fun&& fun, Args&&...args)
            -> typename ::nstd::functional::detail::invoke<Fun, Args...>::type {
            using invoke_type = ::nstd::functional::detail::invoke_type<Fun, Args...>;
            return invoke_type::invoke(::nstd::utility::forward<Fun>(fun), ::nstd::utility::forward<Args>(args)...);
        }
    }
}

// ----------------------------------------------------------------------------

template <bool, bool, typename, typename, typename...>
struct ::nstd::functional::detail::invoke_member_fun_type {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};
template <typename Fun, typename Class, typename Obj, typename... Args>
struct ::nstd::functional::detail::invoke_member_fun_type<true,  true, Fun Class::*, Obj, Args...> {
    template <typename> struct result_type;
    template <typename RC, typename... A> struct result_type<RC(A...)> { using type = RC; };

    using type = typename result_type<Fun>::type;
    using choice_type = ::nstd::type_traits::choice<true,
        ::nstd::functional::detail::invoke_member_fun_type<true, true, Fun Class::*, Obj, Args...>>;

    template <typename F, typename O, typename... A>
    static auto invoke(F&& fun, O&& object, A&&... args) -> type {
        return (::nstd::utility::forward<O>(object).*fun)(::nstd::utility::forward<A>(args)...);
    }
};
template <typename Fun, typename Obj, typename... Args>
struct ::nstd::functional::detail::invoke_member_fun_type<true, false, Fun, Obj, Args...> {
};

template <typename, typename...>
struct ::nstd::functional::detail::invoke_member_fun {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};
template <typename Fun, typename Class, typename Object, typename... Args>
struct ::nstd::functional::detail::invoke_member_fun<Fun Class::*, Object, Args...> {
    using choice_type
        = typename ::nstd::functional::detail::invoke_member_fun_type<true, true, Fun Class::*, Object, Args...>::choice_type;
};

// ----------------------------------------------------------------------------

#if 0
template <typename Fun, typename Obj, typename... Args>
struct ::nstd::functional::detail::invoke_member_type<true, Fun, Obj, Args...> {
    using type = decltype((::nstd::type_traits::declval<Obj>().*::nstd::type_traits::declval<Fun>())(::nstd::type_traits::declval<Args>()...), 0);
    using choice_type = ::nstd::type_traits::choice<true, ::nstd::functional::detail::invoke_member_type<true, Fun, Obj, Args...>>;
};
template <typename Fun, typename Obj, typename... Args>
struct ::nstd::functional::detail::invoke_member_type<false, Fun, Obj, Args...> {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};
template <typename Fun, typename Type, typename Object, typename... Args>
struct ::nstd::functional::detail::invoke_member<Fun Type::*, Object, Args...>
{
    template <typename F, typename T, typename O,
              typename = decltype((::nstd::type_traits::declval<O>().*::nstd::type_traits::declval<F T::*>())(::nstd::type_traits::declval<Args>()...), 0)>
    static auto test(int) -> ::nstd::type_traits::true_type;
    template <typename>
    static auto test(...) -> ::nstd::type_traits::false_type;

    using choice_type = typename nstd::functional::detail::invoke_member_type<decltype(test<Fun>(0))::value, Fun, Args...>::choice_type;
};
template <typename, typename...>
struct ::nstd::functional::detail::invoke_member {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};
#endif

// ----------------------------------------------------------------------------

template <typename Fun, typename... Args>
struct ::nstd::functional::detail::invoke_non_member_type<true, Fun, Args...> {
    using type = decltype(::nstd::type_traits::declval<Fun>()(::nstd::type_traits::declval<Args>()...));
    using choice_type = ::nstd::type_traits::choice<true, ::nstd::functional::detail::invoke_non_member_type<true, Fun, Args...>>;
    
    template <typename F, typename... A>
    static auto invoke(F&& fun, A&&... args)
        -> decltype(::nstd::type_traits::declval<F>()(::nstd::type_traits::declval<A>()...)) {
        return ::nstd::utility::forward<F>(fun)(::nstd::utility::forward<A>(args)...);
    }
};
template <typename Fun, typename... Args>
struct ::nstd::functional::detail::invoke_non_member_type<false, Fun, Args...> {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};

template <typename Fun, typename... Args>
struct ::nstd::functional::detail::invoke_non_member
{
    template <typename F, typename = decltype(::nstd::type_traits::declval<F>()(::nstd::type_traits::declval<Args>()...), 0)>
    static auto test(int) -> ::nstd::type_traits::true_type;
    template <typename>
    static auto test(...) -> ::nstd::type_traits::false_type;

    using choice_type = typename nstd::functional::detail::invoke_non_member_type<decltype(test<Fun>(0))::value, Fun, Args...>::choice_type;
};

// ----------------------------------------------------------------------------

template <typename Fun, typename... Args>
struct ::nstd::functional::detail::invoke
    : ::nstd::functional::detail::invoke_type<Fun, Args...> {
};

// ----------------------------------------------------------------------------

#endif
