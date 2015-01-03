// nstd/utility/tuple.hpp                                             -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_UTILITY_TUPLE
#define INCLUDED_NSTD_UTILITY_TUPLE

#include "nstd/utility/integer_sequence.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"
#include "nstd/functional/reference_wrapper.hpp"
#include "nstd/type_traits/add_const.hpp"
#include "nstd/type_traits/add_volatile.hpp"
#include "nstd/type_traits/add_cv.hpp"
#include "nstd/type_traits/condition.hpp"
#include "nstd/type_traits/decay.hpp"
#include "nstd/cheaders/cstddef.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace utility {
        namespace detail {
            struct ignore {};

            template <nstd::size_t, typename...> struct tuple_type;
            template <nstd::size_t N, typename S, typename... T> struct tuple_type<N, S, T...>;
            template <typename S, typename... T> struct tuple_type<0u, S, T...>;

            template <nstd::size_t I, typename...T>
            using tuple_type_t = typename nstd::utility::detail::tuple_type<I, T...>::type;

            template <nstd::size_t I, typename T>
            struct tuple_field;

            template <typename...>
            struct tuple;
            template <nstd::size_t... I, typename... T>
            struct tuple<nstd::utility::index_sequence<I...>, T...>;

            template <typename T, bool = nstd::functional::is_reference_wrapper<nstd::type_traits::decay_t<T> >::value>
            struct make_tuple;
            template <typename T>
            struct make_tuple<T, true>;
            template <typename T>
            using make_tuple_t = typename make_tuple<T>::type;

            template <nstd::size_t I, nstd::size_t N> struct tuple_compare;
            template <nstd::size_t N> struct tuple_compare<N, N>;
        }

        template <typename...> class tuple;
        template <> class tuple<>;
        constexpr nstd::utility::detail::ignore ignore{};

        template <typename... T>
        auto constexpr make_tuple(T&&...)
            -> tuple<nstd::utility::detail::make_tuple_t<T>...>;
        template <typename... T>
        auto constexpr forward_as_tuple(T&&...) -> tuple<T&&...>;
        template <typename... T>
        auto constexpr tie(T&...) -> tuple<T&...>;

        //-dk:TODO tuple_cat()

        template <typename T> class tuple_size;
        template <typename T> class tuple_size<T const>;
        template <typename T> class tuple_size<T volatile>;
        template <typename T> class tuple_size<T const volatile>;
        template <typename... T> class tuple_size<nstd::utility::tuple<T...> >;

        template <nstd::size_t, typename T> class tuple_element;
        template <nstd::size_t I, typename T> class tuple_element<I, T const>;
        template <nstd::size_t I, typename T> class tuple_element<I, T volatile>;
        template <nstd::size_t I, typename T> class tuple_element<I, T const volatile>;
        template <nstd::size_t I, typename... T> class tuple_element<I, nstd::utility::tuple<T...> >;

        template <nstd::size_t I, typename T>
        using tuple_element_t = typename nstd::utility::tuple_element<I, T>::type;

        template <nstd::size_t I, typename... T>
        auto constexpr get(nstd::utility::tuple<T...>&) noexcept(true)
            -> nstd::utility::tuple_element_t<I, nstd::utility::tuple<T...> >&;
        template <nstd::size_t I, typename... T>
        auto constexpr get(nstd::utility::tuple<T...> const&) noexcept(true)
            -> nstd::utility::tuple_element_t<I, nstd::utility::tuple<T...> > const&;
        template <nstd::size_t I, typename... T>
        auto constexpr get(nstd::utility::tuple<T...>&&) noexcept(true)
            -> nstd::utility::tuple_element_t<I, nstd::utility::tuple<T...> >&&;

        template <typename... S, typename... T>
        auto operator== (nstd::utility::tuple<S...> const& t0,
                         nstd::utility::tuple<T...> const& t1) -> bool {
            return nstd::utility::detail::tuple_compare<0u, sizeof...(S)>::equal(t0, t1);
        }
        template <typename... S, typename... T>
        auto operator!= (nstd::utility::tuple<S...> const& t0,
                         nstd::utility::tuple<T...> const& t1) -> bool {
            return !(t0 == t1);
        }
    }
}

// ----------------------------------------------------------------------------

template <typename S, typename... T>
struct nstd::utility::detail::tuple_type<0u, S, T...> {
    using type = S;
};
template <nstd::size_t N, typename S, typename... T>
struct nstd::utility::detail::tuple_type<N, S, T...> {
    using type = nstd::utility::detail::tuple_type_t<N-1u, T...>;
};

// ----------------------------------------------------------------------------

template <typename... T>
class nstd::utility::tuple_size<nstd::utility::tuple<T...> >
    : public nstd::type_traits::integral_constant<nstd::size_t, sizeof...(T)> {
};
template <typename T>
class nstd::utility::tuple_size<T const>
    : public nstd::utility::tuple_size<T> {
};
template <typename T>
class nstd::utility::tuple_size<T volatile>
    : public nstd::utility::tuple_size<T> {
};
template <typename T>
class nstd::utility::tuple_size<T const volatile>
    : public nstd::utility::tuple_size<T> {
};

// ----------------------------------------------------------------------------

template <nstd::size_t I, typename... T>
class nstd::utility::tuple_element<I, nstd::utility::tuple<T...> > {
public:
    using type = nstd::utility::detail::tuple_type_t<I, T...>;
};
template <nstd::size_t I, typename T>
class nstd::utility::tuple_element<I, T const> {
public:
    using type = nstd::type_traits::add_const_t<nstd::utility::tuple_element_t<I, T> >;
};
template <nstd::size_t I, typename T>
class nstd::utility::tuple_element<I, T volatile> {
public:
    using type = nstd::type_traits::add_volatile_t<nstd::utility::tuple_element_t<I, T> >;
};
template <nstd::size_t I, typename T>
class nstd::utility::tuple_element<I, T const volatile> {
public:
    using type = nstd::type_traits::add_cv_t<nstd::utility::tuple_element_t<I, T> >;
};

// ----------------------------------------------------------------------------

template <nstd::size_t I, typename T>
struct nstd::utility::detail::tuple_field {
    using type = T;
    T value;
    constexpr tuple_field(): value() {}
    constexpr tuple_field(T const& value): value(value) {}
    template <typename S>
    constexpr tuple_field(S&& value): value(nstd::utility::forward<S>(value)) {}
    tuple_field(tuple_field &) = default;
    tuple_field(tuple_field const&) = default;
    tuple_field(tuple_field &&) = default;
};

template <nstd::size_t... I, typename... T>
struct nstd::utility::detail::tuple<nstd::utility::index_sequence<I...>, T...>
    : nstd::utility::detail::tuple_field<I, T>... {
    constexpr tuple() {}
    constexpr tuple(T const&... values)
        : nstd::utility::detail::tuple_field<I, T>(values)... {
    }
    template <typename... S>
    constexpr tuple(S&&... values)
        : nstd::utility::detail::tuple_field<I, T>(nstd::utility::forward<S>(values))... {
    }
    tuple(tuple &) = default;
    tuple(tuple const&) = default;
    tuple(tuple &&) = default;

    template <typename... S>
    constexpr tuple(tuple<nstd::utility::index_sequence<I...>, S...> & other)
        : nstd::utility::detail::tuple_field<I, T>(
            static_cast<nstd::utility::detail::tuple_field<I, S>&>(other).value)... {
    }
    template <typename... S>
    constexpr tuple(tuple<nstd::utility::index_sequence<I...>, S...> const& other)
        : nstd::utility::detail::tuple_field<I, T>(
            static_cast<nstd::utility::detail::tuple_field<I, S> const&>(other).value)... {
    }
    template <typename... S>
    constexpr tuple(tuple<nstd::utility::index_sequence<I...>, S...> && other)
        : nstd::utility::detail::tuple_field<I, T>(
            nstd::utility::move(static_cast<nstd::utility::detail::tuple_field<I, S>&>(other).value))... {
    }
};

// ----------------------------------------------------------------------------

template <typename... T>
class nstd::utility::tuple
    : public nstd::utility::detail::tuple<nstd::utility::make_index_sequence<sizeof...(T)>, T...> {
public:
    using sequence = nstd::utility::make_index_sequence<sizeof...(T)>;
    using base = nstd::utility::detail::tuple<sequence, T...>;
    using base::base;

    constexpr tuple();
    constexpr explicit tuple(T const&...);
    template <typename... S>
    constexpr explicit tuple(S&&...);
    tuple(tuple &) = default;
    tuple(tuple const&) = default;
    tuple(tuple &&) = default;

    template <typename... S>
    constexpr tuple(tuple<S...> &);
    template <typename... S>
    constexpr tuple(tuple<S...> const&);
    template <typename... S>
    constexpr tuple(tuple<S...> &&);

    //-dk:TODO pair constructors
    //-dk:TODO alloctaor constructors
    //-dk:TODO assignments
    //-dk:TODO swap
};

// ----------------------------------------------------------------------------

template <typename... T>
constexpr nstd::utility::tuple<T...>::tuple() {
}

template <typename... T>
constexpr nstd::utility::tuple<T...>::tuple(T const&... values)
    : base(values...) {
}

template <typename... T>
    template <typename... S>
constexpr nstd::utility::tuple<T...>::tuple(S&&... values)
    : base(nstd::utility::forward<S>(values)...) {
}

template <typename... T>
    template <typename... S>
constexpr nstd::utility::tuple<T...>::tuple(nstd::utility::tuple<S...> & other)
    : base(static_cast<nstd::utility::detail::tuple<sequence, S...>&>(other)) {
}
template <typename... T>
    template <typename... S>
constexpr nstd::utility::tuple<T...>::tuple(nstd::utility::tuple<S...> const& other)
    : base(static_cast<nstd::utility::detail::tuple<sequence, S...> const&>(other)) {
}
template <typename... T>
    template <typename... S>
constexpr nstd::utility::tuple<T...>::tuple(nstd::utility::tuple<S...> && other)
    : base(nstd::utility::move(static_cast<nstd::utility::detail::tuple<sequence, S...>&>(other))) {
}

// ----------------------------------------------------------------------------

template <>
class nstd::utility::tuple<> {
public:
    constexpr tuple() {}
    tuple(tuple const&) = default;
    tuple(tuple&&) = default;
};

// ----------------------------------------------------------------------------

template <nstd::size_t I, typename... T>
auto constexpr nstd::utility::get(nstd::utility::tuple<T...>& t) noexcept(true)
    -> nstd::utility::tuple_element_t<I, nstd::utility::tuple<T...> >& {
    return static_cast<nstd::utility::detail::tuple_field<I, 
        nstd::utility::detail::tuple_type_t<I, T...> >&>(t).value;
}

template <nstd::size_t I, typename... T>
auto constexpr nstd::utility::get(nstd::utility::tuple<T...> const& t) noexcept(true)
    -> nstd::utility::tuple_element_t<I, nstd::utility::tuple<T...> > const& {
    return static_cast<nstd::utility::detail::tuple_field<I, 
        nstd::utility::detail::tuple_type_t<I, T...> > const&>(t).value;
}

template <nstd::size_t I, typename... T>
auto constexpr nstd::utility::get(nstd::utility::tuple<T...>&& t) noexcept(true)
    -> nstd::utility::tuple_element_t<I, nstd::utility::tuple<T...> >&& {
    return static_cast<nstd::utility::detail::tuple_field<I, 
        nstd::utility::detail::tuple_type_t<I, T...> >&&>(t).value;
}

// ----------------------------------------------------------------------------

template <nstd::size_t N>
struct nstd::utility::detail::tuple_compare<N, N> {
    template <typename S, typename T>
    static bool equal(S const&, T const&) {
        return true;
    }
};
template <nstd::size_t I, nstd::size_t N>
struct nstd::utility::detail::tuple_compare{
    template <typename S, typename T>
    static bool equal(S const&s, T const& t) {
        return nstd::utility::get<I>(s) == nstd::utility::get<I>(t)
            && nstd::utility::detail::tuple_compare<I + 1u, N>::equal(s, t);
    }
};

// ----------------------------------------------------------------------------

template <typename T, bool>
struct nstd::utility::detail::make_tuple {
    using type = nstd::type_traits::decay_t<T>;
};
template <typename T>
struct nstd::utility::detail::make_tuple<T, true> {
    using type = typename T::type;
};


template <typename... T>
auto constexpr nstd::utility::make_tuple(T&&... values)
    -> nstd::utility::tuple<nstd::utility::detail::make_tuple_t<T>...> {
    return nstd::utility::tuple<nstd::utility::detail::make_tuple_t<T>...>(nstd::utility::forward<T>(values)...);
}

// ----------------------------------------------------------------------------

#endif
