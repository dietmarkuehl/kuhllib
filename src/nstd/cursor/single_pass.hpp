// nstd/cursor/single_pass.hpp                                        -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_CURSOR_SINGLE_PASS
#define INCLUDED_NSTD_CURSOR_SINGLE_PASS

#include <limits>
#include <utility>

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace cursor
    {
        namespace detail {
            template <typename T>
            auto cursor_step(T& cursor) -> void {
                ++cursor;
            }

            struct step {
                template <typename T>
                auto operator()(T& cursor) const -> void {
                    cursor_step(cursor);
                }
            };

            template <typename T>
            auto cursor_key(T&& cursor) -> decltype(*cursor) {
                return *cursor;
            }
            template <typename T>
            auto cursor_key(T cursor) -> typename std::enable_if<std::numeric_limits<T>::is_integer, T>::type {
                return cursor;
            }

            struct key {
                template <typename T>
                auto constexpr operator()(T&& cursor) const -> decltype(cursor_key(cursor)) {
                    return cursor_key(std::forward<T>(cursor));
                }
            };

            template <typename S, typename T>
            auto constexpr cursor_at_same_pos(S&& cursor0, T&& cursor1) -> bool {
                return cursor0 == cursor1;
            }

            struct at_same_pos {
                template <typename S, typename T>
                auto constexpr operator()(S&& cursor0, T&& cursor1) const -> bool {
                    return cursor_at_same_pos(std::forward<S>(cursor0), std::forward<T>(cursor1));
                }
            };
        }

        constexpr nstd::cursor::detail::step        step{};
        constexpr nstd::cursor::detail::key         key{};
        constexpr nstd::cursor::detail::at_same_pos at_same_pos{};
    }
}

// ----------------------------------------------------------------------------

#endif
