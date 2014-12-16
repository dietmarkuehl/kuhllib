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

#include "nstd/type_traits/declval.hpp"
#include "nstd/type_traits/enable_if.hpp"
#include "nstd/utility/forward.hpp"
#include <limits>

// ----------------------------------------------------------------------------
// To implement a single_pass cursor C provide the following operations:
// - cursor_step(C&)              move to the next element
// - cursor_key(C)                access the key at the given position
// - cursor_at_same_pas(C c, E e) return true if c and e are at the same position

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
            template <typename T, typename = nstd::type_traits::enable_if_t<std::numeric_limits<T>::is_integer>>
            auto cursor_key(T cursor) -> T {
                return cursor;
            }

            struct key {
                template <typename T>
                auto operator()(T&& cursor) const -> decltype(cursor_key(cursor)) {
                    return cursor_key(nstd::utility::forward<T>(cursor));
                }
            };

            template <typename S, typename T>
            auto cursor_at_same_pos(S const& cursor0, T const& cursor1) -> bool {
                return cursor0 == cursor1;
            }

            struct at_same_pos {
                template <typename S, typename T>
                auto operator()(S&& cursor0, T&& cursor1) const -> bool {
                    using detail::cursor_at_same_pos;
                    return cursor_at_same_pos(nstd::utility::forward<S>(cursor0), nstd::utility::forward<T>(cursor1));
                }
            };

            struct distance_type_delegate {
                template <typename T>
                static auto size(T const&) -> typename T::distance_type;
                static auto size(...) -> std::size_t;
            };

            template <typename T>
            auto cursor_distance_type(T cursor) -> decltype(distance_type_delegate::size(cursor));

            template <typename T>
            auto distance_type(T cursor) -> decltype(cursor_distance_type(cursor));
        }

        constexpr nstd::cursor::detail::step        step{};
        constexpr nstd::cursor::detail::key         key{};
        constexpr nstd::cursor::detail::at_same_pos at_same_pos{};

        template <typename T> struct distance_type;
        template <typename T>
        using distance_type_t = typename nstd::cursor::distance_type<T>::type;
    }
}

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::cursor::distance_type {
    typedef decltype(nstd::cursor::detail::distance_type(nstd::type_traits::declval<T>())) type;
};

// ----------------------------------------------------------------------------

#endif
