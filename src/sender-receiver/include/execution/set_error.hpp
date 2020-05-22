// include/execution/set_error.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_SET_ERROR
#define INCLUDED_EXECUTION_SET_ERROR

#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename R, typename E>
        concept has_member_set_error
            =  requires(R&& r, E&& e) {
                   std::forward<R>(r).set_error(std::forward<E>(e));
               }
            ;
        template <typename R, typename E>
        concept has_set_error
            =  !has_member_set_error<R, E>
            && requires(R&& r, E&& e) {
                   set_error(std::forward<R>(r), std::forward<E>(e));
               }
            ;

        void set_error();
        struct set_error_t
        {
            template <typename R, typename E>
                requires has_member_set_error<R, E>
            constexpr auto operator()(R&& r, E&& e) const
                noexcept(noexcept(std::forward<R>(r).set_error(std::forward<E>(e))))
            {
                return std::forward<R>(r).set_error(std::forward<E>(e));
            }
            template <typename R, typename E>
                requires has_set_error<R, E>
            constexpr auto operator()(R&& r, E&& e) const
                noexcept(noexcept(set_error(std::forward<R>(r), std::forward<E>(e))))
            {
                return set_error(std::forward<R>(r), std::forward<E>(e));
            }
            template <typename R, typename E>
            constexpr auto operator()(R&& has_no_suitable_set_error, E&&) const = delete;
        };
        inline constexpr set_error_t set_error_cp{};

        // --------------------------------------------------------------------
    }

    inline namespace customization_points
    {
        inline constexpr auto set_error    = customization::set_error_cp;

    }
}

// ----------------------------------------------------------------------------

#endif
