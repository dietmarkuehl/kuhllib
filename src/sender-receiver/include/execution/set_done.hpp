// include/execution/set_done.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_SET_DONE
#define INCLUDED_EXECUTION_SET_DONE

#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename R>
        concept has_member_set_done
            = requires(R&& r) { std::forward<R>(r).set_done(); }
            ;
        template <typename R>
        concept has_set_done
            =  !has_member_set_done<R>
            && requires(R&& r) { set_done(std::forward<R>(r)); }
            ;

        // --------------------------------------------------------------------

        void set_done();
        struct set_done_t
        {
            template <typename R> requires has_member_set_done<R>
            constexpr auto operator()(R&& r) const
                noexcept(noexcept(std::forward<R>(r).set_done()))
            {
                return std::forward<R>(r).set_done();
            }
            template <typename R> requires has_set_done<R>
            constexpr auto operator()(R&& r) const
                noexcept(noexcept(set_done(std::forward<R>(r))))
            {
                return set_done(std::forward<R>(r));
            }
            template <typename R>
            constexpr auto operator()(R&& has_no_suitable_set_done) const = delete;
        };
        inline constexpr set_done_t set_done_cp{};

        // --------------------------------------------------------------------
    }

    inline namespace customization_points
    {
        inline constexpr auto set_done     = customization::set_done_cp;
    }
}

// ----------------------------------------------------------------------------

#endif
