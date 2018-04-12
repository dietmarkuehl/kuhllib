// detection.t.cpp                                                    -*-C++-*-
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

#include "detection.hpp"
#include "cbt/level.hpp"
#include "format.hpp"
#include "utility.hpp"
#include <iostream>
#include <string_view>
#include <initializer_list>
#include <utility>

// ----------------------------------------------------------------------------

namespace test {
    template <typename T>
    using copy_constructor = decltype(T(std::declval<T const&>));
    template <typename T>
    inline constexpr bool has_copy_constructor = cbt::is_detected_v<copy_constructor, T>;

    template <typename U, typename V>
    using equality_operator = decltype(std::declval<U const&>() == std::declval<U const&>()? true: false);
    template <typename U, typename V = U>
    inline constexpr bool has_equality = cbt::is_detected_v<equality_operator, U, V>;

    template <typename U, typename V>
    using inequality_operator = decltype(std::declval<U const&>() != std::declval<U const&>()? true: false);
    template <typename U, typename V = U>
    inline constexpr bool has_inequality = cbt::is_detected_v<inequality_operator, U, V>;

    template <typename T>
    bool test_equality_operator_reflexive(T const& value) {
        // if the type is copyable prevent identity comparison
        if constexpr (has_copy_constructor<T>) {
            T other(value);
            return bool(other == value);
        }
        else {
            return value == value;
        }
    }
    template <typename T>
    bool test_equality_operator(std::ostream& out, std::string_view name,
                                std::initializer_list<T> const& values) {
        for (auto oit = values.begin(), end = values.end(); oit != end; ++oit) {
            // concern: verify that equality is reflexive.
            if (!test_equality_operator_reflexive(*oit)) {
                out << cbt::level::error << " " << name << " equality isn't reflexive"
                    << cbt::format(" for ", *oit)
                    << "\n";
                return false;
            }
            for (auto iit = oit; ++iit != end; ) {
                // concern: verify that unequal objects are not equal
                if (bool(*oit == *iit)) {
                    out << cbt::level::error << " " << name
                        << " objects unexpectedly compare equal"
                        << cbt::format(": ", *oit, " and ", *iit)
                        << "\n";
                    return false;
                }
                // concern: verify that equality is commutative
                if (bool(*iit == *oit)) {
                    out << cbt::level::error << " " << name
                        << "equality isn't commutative"
                        << cbt::format(" for ", *oit, " and ", *iit)
                        << "\n";
                    return false;
                }
            }
        }
        return true;
    }
    template <typename T>
    bool test_inequality_operator(std::ostream& out, std::string_view name,
                                std::initializer_list<T> const& values) {
        if (!has_inequality<T>) {
            out << cbt::level::warning << " " << name << " does not have an inequality operator: consider providing one!\n";
        }
        bool report_equal(true);
        bool report_inequal(true);
        for (auto oit = values.begin(), end = values.end(); oit != end; ++oit) {
            if (!(*oit == *oit) && report_equal) {
                out << "   equality for same object doesn't work\n";
                report_equal = false;
            }
            if constexpr (has_inequality<T>) {
                if ((*oit != *oit) && report_inequal) {
                    out << "   inequality for same object doesn't work\n";
                    report_inequal = false;
                }
            }
            for (auto iit = oit; ++iit != end; ) {
                if (*oit == *iit && report_equal) {
                    out << "   equality for non-equal elements doesn't work\n";
                    report_equal = false;
                }
            }
        }
        return true;
    }
    template <typename T>
    bool equality_comparable(std::ostream& out, std::string_view name,
                             std::initializer_list<T> values) {
        if constexpr (has_equality<T>) {
            out << cbt::level::info << " " << name << " has an equality operator\n";
            return test_equality_operator(out, name, values);
        }
        else {
            cbt::use(values);
            out << cbt::level::error << " " << name << " has no equality operator\n";
            return false;
        }
    }
}

// ----------------------------------------------------------------------------

namespace {
    struct empty {
    };
    enum class vals { v0, v1 };
    template <typename T>
    struct value {
        T val;
        T operator== (value const& other) const {
            return T(val == other.val);
        }
    };
    long operator!= (value<long> v0, value<long> v1) {
        return v0.val == v1.val;
    }
}

// ----------------------------------------------------------------------------

int main() {
    test::equality_comparable(std::cout, "empty", { empty{}, empty{} });
    test::equality_comparable(std::cout, "value<bool>", { value<bool>{true}, value<bool>{ false } });
    test::equality_comparable(std::cout, "value<int>", { value<int>{0}, value<int>{ 1 } });
    test::equality_comparable(std::cout, "value<long>", { value<long>{0}, value<long>{ 1 } });
    test::equality_comparable(std::cout, "long", { long{0}, long{ 1 } });
    test::equality_comparable(std::cout, "value<vals>", { value<vals>{vals::v0}, value<vals>{ vals::v1 } });
}
