// include/execution/start.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_START
#define INCLUDED_EXECUTION_START

#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename O>
        concept has_member_start
            = requires(O&& o) { std::forward<O>(o).start(); }
            ;

        template <typename O>
        concept has_start
            =  !has_member_start<O>
            && requires(O&& o) { start(std::forward<O>(o)); }
            ;

        // --------------------------------------------------------------------

        void start();
        struct start_t
        {
            template <typename O>
                requires has_member_start<O>
            constexpr auto operator()(O&& o) const
                noexcept(noexcept(std::forward<O>(o).start()))
            {
                return std::forward<O>(o).start();
            }
            template <typename O>
                requires has_start<O>
            constexpr auto operator()(O&& o) const
                noexcept(noexcept(start(std::forward<O>(o))))
            {
                return start(std::forward<O>(o));
            }
            template <typename O>
            constexpr auto operator()(O&& has_no_suitable_start) const = delete;
        };
        inline constexpr start_t start_cp{};

        // --------------------------------------------------------------------
    }

    inline namespace customization_points
    {
        inline constexpr auto start        = customization::start_cp;
    }
}

// ----------------------------------------------------------------------------

#endif
