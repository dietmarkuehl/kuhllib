// cbt/constructible.hpp                                              -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CBT_CONSTRUCTIBLE
#define INCLUDED_CBT_CONSTRUCTIBLE

// ----------------------------------------------------------------------------

#include "cbt/destructible.hpp"
#include "cbt/level.hpp"
#include "cbt/testequal.hpp"
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cbt {
    template <typename T, typename... A>
    struct Constructible_t {
        bool operator()(std::ostream& out, std::string_view name) const {
            return cbt::Destructible<T>(out, name)
                && [&](){
                    if (!std::is_constructible_v<T, A...>) {
                        out << cbt::level::error << " " << name << " "
                            << "is not constructible (with the given arguments)\n";
                        return false;
                    }
                    return true;
                   }()
                ;
        }
    };
    template <typename T>
    struct Constructible_t<T> {
        bool operator()(std::ostream& out, std::string_view name) const {
            return cbt::Destructible<T>(out, name)
                && [&](){
                        if (!std::is_constructible_v<T>) {
                            out << cbt::level::error << " " << name << " "
                                << "is not default constructible\n";
                            return false;
                        }
                        return true;
                    }()
                && [&](){
                    if constexpr (std::is_constructible_v<T>
                                  && std::is_nothrow_destructible_v<T>) {
                            T obj1{};
                            T obj2{};
                            return cbt::TestEqual(out, name, obj1, obj2);
                    }
                    return true;
                }()
                ;
        }
        template <typename... A>
        bool operator()(std::ostream& out, std::string_view name, A&&... a) const {
            return cbt::Destructible<T>(out, name)
                && [&](){
                        if (!std::is_constructible_v<T, A...>) {
                            out << cbt::level::error << " " << name << " "
                                << "is not constructible (with the given arguments)\n";
                            return false;
                        }
                        return true;
                    }()
                && [&](){
                        if constexpr (std::is_constructible_v<T, A...>
                                      && std::is_nothrow_destructible_v<T>) {
                                T obj1(std::forward<A>(a)...);
                                T obj2(std::forward<A>(a)...);
                                return cbt::TestEqual(out, name, obj1, obj2);
                        }
                        return true;
                    }()
                ;
        }
    };
    template <typename T, typename... A>
    inline constexpr Constructible_t<T, A...> Constructible{};
}

// ----------------------------------------------------------------------------

#endif
