// include/execution/set_value.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_SET_VALUE
#define INCLUDED_EXECUTION_SET_VALUE

#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename R, typename... A>
        concept has_member_set_value
            =  requires(R&& r, A&&... a) {
                   std::forward<R>(r).set_value(std::forward<A>(a)...);
               }
            ;
        template <typename R, typename... A>
        concept has_set_value
            =  !has_member_set_value<R, A...>
            && requires(R&& r, A&&... a) {
                   set_value(std::forward<R>(r), std::forward<A>(a)...);
               }
            ;
        
        // --------------------------------------------------------------------

        void set_value();
        struct set_value_t
        {
            template <typename R, typename... A>
                requires has_member_set_value<R, A...>
            constexpr auto operator()(R&& r, A&&...a) const
                noexcept(noexcept(std::forward<R>(r).set_value(std::forward<A>(a)...)))
            { 
                return std::forward<R>(r).set_value(std::forward<A>(a)...);
            }

            template <typename R, typename... A>
                requires has_set_value<R, A...>
            constexpr auto operator()(R&& r, A&&...a) const
                noexcept(noexcept(set_value(std::forward<R>(r), std::forward<A>(a)...)))
            { 
                return set_value(std::forward<R>(r), std::forward<A>(a)...);
            }
            template <typename R, typename... A>
            constexpr auto operator()(R&& has_no_suitable_set_value, A&&...) const
                = delete;
        };
        inline constexpr set_value_t set_value_cp{};

        // --------------------------------------------------------------------
    }

    inline namespace customization_points
    {
        inline constexpr auto set_value    = customization::set_value_cp;
    }
}

// ----------------------------------------------------------------------------

#endif
