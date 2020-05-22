// include/execution/execute.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_EXECUTE
#define INCLUDED_EXECUTION_EXECUTE

#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution {
    namespace customization
    {
        // --------------------------------------------------------------------

        template <typename E, typename F>
        concept has_member_execute
            = requires(E&& e, F&& f) {
                    std::forward<E>(e).execute(std::forward<F>(f));
                }
            ;
        template <typename E, typename F>
        concept has_execute
            =  !has_member_execute<E, F>
            && requires(E&& e, F&& f) {
                    execute(std::forward<E>(e), std::forward<F>(f));
                }
            ;

        // --------------------------------------------------------------------

        void execute();
        struct execute_t
        {
            template <typename E, typename F>
                requires has_member_execute<E, F>
            constexpr auto operator()(E&& e, F&& f) const
                noexcept(noexcept(std::forward<E>(e).execute(std::forward<F>(f))))
            {
                return std::forward<E>(e).execute(std::forward<F>(f));
            }
            template <typename E, typename F>
                requires has_execute<E, F>
            constexpr auto operator()(E&& e, F&& f) const
                noexcept(noexcept(execute(std::forward<E>(e), std::forward<F>(f))))
            {
                return execute(std::forward<E>(e), std::forward<F>(f));
            }
        };
        inline constexpr execute_t execute_cp{};

        // --------------------------------------------------------------------
    }

    inline namespace customization_points
    {
        inline constexpr auto execute      = customization::execute_cp;
    }
}

// ----------------------------------------------------------------------------

#endif
