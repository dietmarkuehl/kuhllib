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

#include <execution/executor.hpp>
#include <execution/sender.hpp>

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
#if 0
Otherwise, as-sender<remove_cvref_t<S>>{s} if S satisfies executor, where as-sender is an implementation-defined class template equivalent to

  template<class E>
  struct as-sender {
  private:
    E ex_;
  public:
    template<template<class...> class Tuple, template<class...> class Variant>
      using value_types = Variant<Tuple<>>;
    template<template<class...> class Variant>
      using error_types = Variant<std::exception_ptr>;
    static constexpr bool sends_done = true;

    explicit as-sender(E e) noexcept
      : ex_((E&&) e) {}
    template<class R>
      requires receiver_of<R>
    connect_result_t<E, R> connect(R&& r) && {
      return execution::connect((E&&) ex_, (R&&) r);
    }
    template<class R>
      requires receiver_of<R>
    connect_result_t<const E &, R> connect(R&& r) const & {
      return execution::connect(ex_, (R&&) r);
    }
  };

#endif
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
            //-dk:TODO add the as-sender schedule

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
