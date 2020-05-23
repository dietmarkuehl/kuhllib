// include/execution/schedule.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_SCHEDULE
#define INCLUDED_EXECUTION_SCHEDULE

#include <execution/connect.hpp>
#include <execution/executor.hpp>
#include <execution/receiver_of.hpp>
#include <execution/sender.hpp>

#include <exception>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename S>
        concept has_member_schedule
            =  requires(S&& s)
               {
                   std::forward<S>(s).schedule();
               }
            && execution::sender<decltype(std::declval<S>().schedule())>
            ;
        template <typename S>
        concept has_schedule
            =  !has_member_schedule<S>
            && requires(S&& s)
               {
                   schedule(std::forward<S>(s));
               }
            && execution::sender<decltype(schedule(std::declval<S>()))>
            ;

        // --------------------------------------------------------------------

        template<typename E>
        struct as_sender {
        private:
            E d_e;
        public:
            template<template<typename...> class T,
                     template<typename...> class V>
            using value_types = V<T<>>;
            template<template<typename...> class V>
            using error_types = V<std::exception_ptr>;
            static constexpr bool sends_done = true;

            explicit as_sender(E e) noexcept
                : d_e(std::forward<E>(e))
            {
            }
            template<typename R>
                requires execution::receiver_of<R>
            execution::connect_result_t<E, R> connect(R&& r) &&
            {
                return execution::connect(std::forward<E>(d_e), std::forward<R>(r));
            }
            template<typename R>
            requires receiver_of<R>
            execution::connect_result_t<E const&, R> connect(R&& r) const&
            {
                return execution::connect(d_e, std::forward<R>(r));
            }
        };

        // --------------------------------------------------------------------

        void schedule();
        struct schedule_t
        {
            template <typename S>
                requires has_member_schedule<S>
            constexpr auto operator()(S&& s) const
                noexcept(noexcept(std::forward<S>(s).schedule()))
            {
                return std::forward<S>(s).schedule();
            }

            template <typename S>
                requires has_schedule<S>
            constexpr auto operator()(S&& s) const
                noexcept(noexcept(schedule(std::forward<S>(s))))
            {
                return schedule(std::forward<S>(s));
            }

            template <typename S>
            requires (!has_member_schedule<S>)
                  && (!has_schedule<S>)
                  && execution::executor<S>
            constexpr auto operator()(S&& s) const
            {
                return as_sender<std::remove_cvref_t<S>>{s};
            }

            template <typename S>
            constexpr void operator()(S&&) const = delete;
        };
        inline constexpr schedule_t schedule_cp{};

        // --------------------------------------------------------------------
    }
    
    inline namespace customization_points
    {
        inline constexpr auto schedule     = customization::schedule_cp;
    }

}

// ----------------------------------------------------------------------------

#endif
