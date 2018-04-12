// cbt/testequal.hpp                                                  -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2018 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CBT_TESTEQUAL
#define INCLUDED_CBT_TESTEQUAL

#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include <ostream>
#include <string>
#include <utility>

// ----------------------------------------------------------------------------

namespace cbt {
    namespace detail {
        template <typename T>
        using test_equal_fun = decltype(test_equal(std::declval<T const&>(), std::declval<T const&>()));
        template <typename T>
        using test_equal_op = decltype(std::declval<T const&>() == std::declval<T const&>());
    }
    template <typename T>
    bool TestEqual(std::ostream& out, std::string_view name, T const& obj1, T const& obj2);
}

// ----------------------------------------------------------------------------

template <typename T>
bool cbt::TestEqual(std::ostream& out, std::string_view name, T const& obj1, T const& obj2) {
    if constexpr (cbt::is_detected_v<cbt::detail::test_equal_fun, T>) {
            if (test_equal(obj1, obj2)) {
                return true;
            }
            out << cbt::level::error << " " << name << " "
                << "objects are not equal\n";
            return false;
    }
    if constexpr (cbt::is_detected_v<cbt::detail::test_equal_op, T>) {
            if (obj1 == obj2) {
                return true;
            }
            out << cbt::level::error << " " << name << " "
                << "objects are not equal\n";
            return false;
    }
    out << cbt::level::warning << " neither test_equal(a, b) nor a == b is defined "
        << "for objects of type " << name << "\n";
    return true;
}

// ----------------------------------------------------------------------------

#endif
