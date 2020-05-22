// include/execution/submit.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_SUBMIT
#define INCLUDED_EXECUTION_SUBMIT

#include <execution/connect.hpp>
#include <execution/receiver_of.hpp>
#include <execution/sender_to.hpp>
#include <execution/set_done.hpp>
#include <execution/set_error.hpp>
#include <execution/set_value.hpp>
#include <execution/start.hpp>

#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename S, typename R>
        concept has_member_submit
            =  requires(S&& s, R&& r)
               {
                   std::forward<S>(s).submit(std::forward<R>(r));
               }
            ;

        template <typename S, typename R>
        concept has_submit
            =  !has_member_submit<S, R>
            && requires(S&& s, R&& r)
               {
                   submit(std::forward<S>(s), std::forward<R>(r));
               }
            ;

        // --------------------------------------------------------------------

        template <typename S, typename R>
        struct submit_receiver
        {
            struct state
            {
                submit_receiver* d_sr;

                template <typename... A>
                    requires execution::receiver_of<R, A...>
                void set_value(A&&... a) && noexcept(execution::is_nothrow_receiver_of_v<R, A...>)
                {
                    execution::set_value(std::move(this->d_sr->d_r), std::forward<A>(a)...);
                    delete this->d_sr;
                }
                template <typename E>
                    requires execution::receiver<R, E>
                void set_error(E&& e) && noexcept
                {
                    execution::set_error(std::move(this->d_sr->d_r), std::forward<E>(e));
                    delete this->d_sr;
                }
                void set_done() && noexcept
                {
                    execution::set_done(std::move(this->d_sr->d_r));
                    delete this->d_sr;
                }
            };
            std::remove_cvref_t<R>                d_r;
            execution::connect_result_t<S, state> d_state;
            submit_receiver(S&& s, R&& r)
                : d_r(std::forward<R>(r))
                , d_state(execution::connect(std::forward<S>(s), state{this}))
            {
            }
        };

        // --------------------------------------------------------------------

        void submit();
        struct submit_t
        {
            template <typename S, typename R>
                requires execution::sender_to<S, R>
                      && has_member_submit<S, R>
            constexpr auto operator()(S&& s, R&& r) const
                noexcept(noexcept(std::forward<S>(s).submit(std::forward<R>(r))))
            {
                return std::forward<S>(s).submit(std::forward<R>(r));
            }
            template <typename S, typename R>
                requires execution::sender_to<S, R>
                      && has_submit<S, R>
            constexpr auto operator()(S&& s, R&& r) const
                noexcept(noexcept(submit(std::forward<S>(s), std::forward<R>(r))))
            {
                return submit(std::forward<S>(s), std::forward<R>(r));
            }
            
            template <typename S, typename R>
                requires execution::sender_to<S, R>
                      && (!has_member_submit<S, R>)
                      && (!has_submit<S, R>)
            constexpr auto operator()(S&& s, R&& r) const
            {
                return execution::start((new submit_receiver<S, R>(std::forward<S>(s), std::forward<R>(r)))->d_state);
            }
            
            template <typename S, typename R>
                requires (!execution::sender_to<S, R>)
            constexpr void operator()(S&&, R&&) const = delete;
        };
        inline constexpr submit_t submit_cp{};

        // --------------------------------------------------------------------
    }
    inline namespace customization_points
    {
        inline constexpr auto submit       = customization::submit_cp;
    }
}

// ----------------------------------------------------------------------------

#endif
