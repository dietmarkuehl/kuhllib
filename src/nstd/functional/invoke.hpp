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

#include "nstd/type_traits/add_lvalue_reference.hpp"
#include "nstd/type_traits/choose_type.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/decay.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/is_member_function_pointer.hpp"
#include "nstd/type_traits/is_pointer.hpp"
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

            template <bool, bool, typename, typename, typename...> struct invoke_member_type;
            template <typename Fun, typename Class, typename Obj, typename... Args>
                struct invoke_member_type<true,  true, Fun Class::*, Obj, Args...>;
            template <typename Fun, typename Class, typename Obj, typename... Args>
                struct invoke_member_type<true, false, Fun Class::*, Obj, Args...>;
            template <typename Type, typename Class, typename Obj>
                struct invoke_member_type<false,  true, Type Class::*, Obj>;
            template <typename Type, typename Class, typename Obj>
                struct invoke_member_type<false, false, Type Class::*, Obj>;
            template <bool, typename, typename...> struct invoke_member_fun;
            template <bool Ref, typename Fun, typename Class, typename Object, typename... Args>
                struct invoke_member_fun<Ref, Fun Class::*, Object, Args...>;

            template <typename, typename...> struct invoke;

            template <typename Fun, typename... Args>
            using invoke_type = ::nstd::type_traits::choose_type_t<
                typename ::nstd::functional::detail::invoke_member_fun<true,  ::nstd::type_traits::decay_t<Fun>, Args...>::choice_type,
                typename ::nstd::functional::detail::invoke_member_fun<false, ::nstd::type_traits::decay_t<Fun>, Args...>::choice_type,
                typename ::nstd::functional::detail::invoke_non_member<Fun, Args...>::choice_type,
                ::nstd::type_traits::choice<true, ::nstd::functional::detail::invoke_empty>
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
struct nstd::functional::detail::invoke_member_type {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};
template <typename Fun, typename Class, typename Obj, typename... Args>
struct nstd::functional::detail::invoke_member_type<true,  true, Fun Class::*, Obj, Args...> {
    template <typename F, typename = decltype((::nstd::type_traits::declval<Obj>().*nstd::type_traits::declval<F>())
                                              (::nstd::type_traits::declval<Args>()...))>
    static auto test(int) -> ::nstd::type_traits::true_type;
    template <typename>
    static auto test(...) -> ::nstd::type_traits::false_type;

    template <typename> struct result_type;
    template <typename RC, typename... A> struct result_type<RC(A...)> { using type = RC; };
    template <typename RC, typename... A> struct result_type<RC(A..., ...)> { using type = RC; };

    using type = typename result_type<Fun>::type;
    using choice_type = ::nstd::type_traits::choice<decltype(test<Fun Class::*>(0))::value,
        ::nstd::functional::detail::invoke_member_type<true, true, Fun Class::*, Obj, Args...>>;

    template <typename F, typename O, typename... A>
    static auto invoke(F&& fun, O&& object, A&&... args) -> type {
        return (::nstd::utility::forward<O>(object).*fun)(::nstd::utility::forward<A>(args)...);
    }
};
template <typename Fun, typename Class, typename Obj, typename... Args>
struct nstd::functional::detail::invoke_member_type<true,  false, Fun Class::*, Obj, Args...> {
    template <typename O, typename F, typename = decltype((*::nstd::type_traits::declval<O>().*nstd::type_traits::declval<F>())
                                              (::nstd::type_traits::declval<Args>()...))>
    static auto test(int) -> ::nstd::type_traits::true_type;
    template <typename, typename>
    static auto test(...) -> ::nstd::type_traits::false_type;

    template <typename> struct result_type;
    template <typename RC, typename... A> struct result_type<RC(A...)> { using type = RC; };
    template <typename RC, typename... A> struct result_type<RC(A..., ...)> { using type = RC; };

    using type = typename result_type<Fun>::type;
    using choice_type = ::nstd::type_traits::choice<decltype(test<Obj, Fun Class::*>(0))::value,
        ::nstd::functional::detail::invoke_member_type<true, false, Fun Class::*, Obj, Args...>>;

    template <typename F, typename O, typename... A>
    static auto invoke(F&& fun, O&& object, A&&... args) -> type {
        return ((*object).*fun)(::nstd::utility::forward<A>(args)...);
    }
};
template <typename Type, typename Class, typename Obj>
struct nstd::functional::detail::invoke_member_type<false,  true, Type Class::*, Obj> {
    template <typename O, typename F, typename = decltype(::nstd::type_traits::declval<O>().*nstd::type_traits::declval<F>())>
    static auto test(int) -> ::nstd::type_traits::true_type;
    template <typename, typename>
    static auto test(...) -> ::nstd::type_traits::false_type;

    template <bool, typename O, typename F> struct make_type;
    template <typename O, typename F>
    struct make_type<true, O, F> {
        using type = decltype(::nstd::type_traits::declval<O>().*nstd::type_traits::declval<F>());
    };
    template <typename O, typename F>
    struct make_type<false, O, F> {
        using type = void;
    };
    static constexpr bool test_result{decltype(test<Obj, Type Class::*>(0))::value};
    using type = typename make_type<test_result, Obj, Type Class::*>::type;
    using choice_type = ::nstd::type_traits::choice<test_result,
        ::nstd::functional::detail::invoke_member_type<false, true, Type Class::*, Obj>>;

    template <typename F, typename O>
    static auto invoke(F&& member, O&& object) -> type {
        return ::nstd::utility::forward<O>(object).*member;
    }
};
template <typename Type, typename Class, typename Obj>
struct nstd::functional::detail::invoke_member_type<false,  false, Type Class::*, Obj> {
    template <typename O, typename F, typename = decltype((*::nstd::type_traits::declval<O>()).*nstd::type_traits::declval<F>())>
    static auto test(int) -> ::nstd::type_traits::true_type;
    template <typename, typename>
    static auto test(...) -> ::nstd::type_traits::false_type;

    template <bool, typename O, typename F> struct make_type;
    template <typename O, typename F>
    struct make_type<true, O, F> {
        using type = decltype((*::nstd::type_traits::declval<O>()).*nstd::type_traits::declval<F>());
    };
    template <typename O, typename F>
    struct make_type<false, O, F> {
        using type = void;
    };
    static constexpr bool test_result{decltype(test<Obj, Type Class::*>(0))::value};
    using type = typename make_type<test_result, Obj, Type Class::*>::type;
    using choice_type = ::nstd::type_traits::choice<test_result,
        ::nstd::functional::detail::invoke_member_type<false, false, Type Class::*, Obj>>;

    template <typename F, typename O>
    static auto invoke(F&& member, O&& object) -> type {
        return (*object).*member;
    }
};

template <bool, typename, typename...>
struct nstd::functional::detail::invoke_member_fun {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};
template <bool Ref, typename Fun, typename Class, typename Object, typename... Args>
struct nstd::functional::detail::invoke_member_fun<Ref, Fun Class::*, Object, Args...> {
    static constexpr bool is_function = ::nstd::type_traits::is_member_function_pointer<Fun Class::*>::value;
    using choice_type
        = typename ::nstd::functional::detail::invoke_member_type<is_function,
                                                                  Ref && !nstd::type_traits::is_pointer<Object>::value,
                                                                  Fun Class::*, Object, Args...>::choice_type;
};

// ----------------------------------------------------------------------------

template <typename Fun, typename... Args>
struct nstd::functional::detail::invoke_non_member_type<true, Fun, Args...> {
    using type = decltype(::nstd::type_traits::declval<Fun>()(::nstd::type_traits::declval<Args>()...));
    using choice_type = ::nstd::type_traits::choice<true, ::nstd::functional::detail::invoke_non_member_type<true, Fun, Args...>>;

    template <typename F, typename... A>
    static auto invoke(F&& fun, A&&... args)
        -> decltype(::nstd::type_traits::declval<F>()(::nstd::type_traits::declval<A>()...)) {
        return ::nstd::utility::forward<F>(fun)(::nstd::utility::forward<A>(args)...);
    }
};
template <typename Fun, typename... Args>
struct nstd::functional::detail::invoke_non_member_type<false, Fun, Args...> {
    using type = void;
    using choice_type = ::nstd::type_traits::choice<false, ::nstd::functional::detail::invoke_empty>;
};

template <typename Fun, typename... Args>
struct nstd::functional::detail::invoke_non_member
{
    template <typename F, typename = decltype(::nstd::type_traits::declval<F>()(::nstd::type_traits::declval<Args>()...))>
    static auto test(int) -> ::nstd::type_traits::true_type;
    template <typename>
    static auto test(...) -> ::nstd::type_traits::false_type;

    using choice_type = typename nstd::functional::detail::invoke_non_member_type<decltype(test<Fun>(0))::value, Fun, Args...>::choice_type;
};

// ----------------------------------------------------------------------------

template <typename Fun, typename... Args>
struct nstd::functional::detail::invoke
    : ::nstd::functional::detail::invoke_type<Fun, Args...> {
};

// ----------------------------------------------------------------------------

#endif
