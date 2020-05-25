// include/execution/connect.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_CONNECT
#define INCLUDED_EXECUTION_CONNECT

#include <execution/as_invocable.hpp>
#include <execution/executor_of_impl.hpp>
#include <execution/operation_state.hpp>
#include <execution/receiver_of.hpp>
#include <execution/sender.hpp>

#include <utility>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename S, typename R>
        struct as_operation
        {
            std::remove_cvref_t<S> d_s;
            std::remove_cvref_t<R> d_r;

            void start() noexcept;
        };

        // --------------------------------------------------------------------

        template <typename S, typename R>
        concept has_member_connect
            =  requires(S&& s, R&& r){ std::forward<S>(s).connect(std::forward<R>(r)); }
            ;
        template <typename S, typename R>
        concept has_connect
            =  !has_member_connect<S, R>
            && requires(S&& s, R&& r){ connect(std::forward<S>(s), std::forward<R>(r)); }
            ;

        // --------------------------------------------------------------------

        void connect();
        struct connect_t
        {
            template <typename S, typename R>
                requires has_member_connect<S, R>
                      && execution::sender<S>
                      && execution::operation_state<decltype(std::declval<S>().connect(std::declval<R>()))>
            constexpr auto operator()(S&& s, R&& r) const
                noexcept(noexcept(std::forward<S>(s).connect(std::forward<R>(r))))
            {
                return std::forward<S>(s).connect(std::forward<R>(r));
            }
            template <typename S, typename R>
                requires has_connect<S, R>
                      && execution::sender<S>
                      && execution::operation_state<decltype(connect(std::declval<S>(), std::declval<R>()))>
            constexpr auto operator()(S&& s, R&& r) const
                noexcept(noexcept(connect(std::forward<S>(s), std::forward<R>(r))))
            {
                return connect(std::forward<S>(s), std::forward<R>(r));
            }
            template <typename S, typename R>
                requires receiver_of<R>
                      && execution::detail::executor_of_impl<std::remove_cvref_t<S>,
                                                             execution::detail::as_invocable<std::remove_cvref_t<R>, S>>
            constexpr auto operator()(S&& s, R&& r) const
                noexcept(noexcept(as_operation{std::forward<S>(s), std::forward<R>(r)}))
            {
                return as_operation{std::forward<S>(s), std::forward<R>(r)};
            }

            template <typename S, typename R>
            constexpr auto operator()(S&& has_no_suitable_connect, R&& r) const = delete;
        };
        inline constexpr connect_t connect_cp{};

        // --------------------------------------------------------------------

    }

    inline namespace customization_points
    {
        inline constexpr auto connect      = customization::connect_cp;
    }

    template<typename S, typename R>
    using connect_result_t = std::invoke_result_t<decltype(connect), S, R>;

}

// ----------------------------------------------------------------------------

#endif
