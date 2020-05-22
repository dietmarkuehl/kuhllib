// include/execution/sender_traits.hpp                                -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_INCLUDE_EXECUTION_SENDER_TRAITS
#define INCLUDED_INCLUDE_EXECUTION_SENDER_TRAITS

#include <execution/as_invocable.hpp>
#include <execution/executor_of_impl.hpp>
#include <execution/sender_base.hpp>

#include <concepts>
#include <exception>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace sender_traits_detail
    {
        // --------------------------------------------------------------------

        template <template <template <typename...> class T,
                            template <typename...> class V> class>
        struct has_value_types;
        template <template <template <typename...> class V> class>
        struct has_error_types;

        template <typename S>
        concept has_sender_types
            = requires {
                typename has_value_types<S::template value_types>;
                typename has_error_types<S::template error_types>;
                typename std::bool_constant<S::sends_done>;
            }
            ;

        // --------------------------------------------------------------------

        struct void_receiver
        {
            void set_value() noexcept;
            void set_error(std::exception_ptr) noexcept;
            void set_done() noexcept;
        };

        // --------------------------------------------------------------------

        template <typename S,
                  bool = has_sender_types<S>,
                  bool = execution::detail::executor_of_impl<S, execution::detail::as_invocable<void_receiver, S>>,
                  bool = std::derived_from<S, sender_base>>
        struct base;

        template <typename S, bool E, bool B>
        struct base<S, true, E, B>;

        template <typename S, bool B>
        struct base<S, false, true, B>;

        template <typename S>
        struct base<S, false, false, true>;

        // --------------------------------------------------------------------
    }

    template<class S>
    struct sender_traits;
}

// ----------------------------------------------------------------------------
// the primary template, indicating that it didn't get specialized.

template <typename, bool, bool, bool>
struct cxxrt::execution::sender_traits_detail::base
{
    using __unspecialized = void;
};

// ----------------------------------------------------------------------------
// The sender has nested value and error types defined

template <typename S, bool E, bool B>
struct cxxrt::execution::sender_traits_detail::base<S, true, E, B>
{
    template <template <typename...> class T,
              template <typename...> class V>
    using value_types = typename S::template value_types<T, V>;
    template <template <typename...> class V>
    using error_types = typename S::template error_types<V>;

    static constexpr bool sends_done = S::sends_done;
};

// ----------------------------------------------------------------------------
// The sender can use an invocable

template <typename S, bool B>
struct cxxrt::execution::sender_traits_detail::base<S, false, true, B>
{
    template <template <typename...> class T,
              template <typename...> class V>
    using value_types = V<T<>>;
    template <template <typename...> class V>
    using error_types = V<std::exception_ptr>;

    static constexpr bool sends_done = true;
};

// ----------------------------------------------------------------------------
// The sender is derived from sender_base.

template <typename S>
struct cxxrt::execution::sender_traits_detail::base<S, false, false, true>
{
};

// ----------------------------------------------------------------------------

template<class S>
struct cxxrt::execution::sender_traits
    : cxxrt::execution::sender_traits_detail::base<S>
{
};

// ----------------------------------------------------------------------------

#endif
