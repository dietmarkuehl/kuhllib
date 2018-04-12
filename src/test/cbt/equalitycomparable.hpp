// cbt/equalitycomparable.hpp                                         -*-C++-*-
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

#ifndef INCLUDED_CBT_EQUALITYCOMPARABLE
#define INCLUDED_CBT_EQUALITYCOMPARABLE

#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <string>

// ----------------------------------------------------------------------------

namespace cbt {
    template <typename U, typename V = U>
    using equality_operator = decltype(std::declval<U&&>() == std::declval<V&&>());
    template <typename U, typename V = U>
    using inequality_operator = decltype(std::declval<U const&>() != std::declval<V const&>());

    template <typename T> struct type_tag {
        using type = T;
        template <typename S>
        S&& cast(S&& s) {
            return static_cast<S&&>(s);
        }
    };

    template <template <typename...> class Operation, typename S, typename T, typename Test>
    bool testBinaryVariations2(std::ostream& out, std::string_view operation, std::string_view name, Test&& test) {
        (void)test;
        if (!cbt::is_detected_v<Operation, S, T>) {
            out << cbt::level::error << " " << operation << " does not exist for " << name << "\n";
            return false;
        }
        else {
            return test(cbt::type_tag<S>(), cbt::type_tag<T>());
        }
    }
    template <template <typename...> class Operation, typename S, typename T, typename Test>
    bool testBinaryVariations1(std::ostream& out, std::string_view operation, std::string_view name, Test test) {
        return testBinaryVariations2<Operation, S, std::remove_const_t<T&>>(out, operation, name, test)
            && testBinaryVariations2<Operation, S, std::remove_const_t<T>&&>(out, operation, name, test)
            && testBinaryVariations2<Operation, S, T&>(out, operation, name, test)
            && testBinaryVariations2<Operation, S, T&&>(out, operation, name, test)
            ;
    }
    template <template <typename...> class Operation, typename S, typename T, typename Test>
    bool testBinaryVariations(std::ostream& out, std::string_view operation, std::string_view name, Test test) {
        return testBinaryVariations1<Operation, std::remove_const_t<S>&, T>(out, operation, name, test)
            && testBinaryVariations1<Operation, std::remove_const_t<S>&&, T>(out, operation, name, test)
            && testBinaryVariations1<Operation, S&, T>(out, operation, name, test)
            && testBinaryVariations1<Operation, S&&, T>(out, operation, name, test)
            ;
    }

    template <typename T>
    bool XEqualityComparable(std::ostream& out, std::string_view name,
                            std::initializer_list<T> values) {
        if (!testBinaryVariations<cbt::equality_operator, T const, T const>(out, "operator==()", name,
                                                                            [&](auto t0, auto t1){
                for (auto it(values.begin()), end(values.end()); it != end; ++it) {
                    if constexpr (cbt::is_detected_v<equality_operator, typename decltype(t0)::type, typename decltype(t1)::type>) {
                        if (!bool(t0.cast(*it) == t1.cast(*it))) {
                            out << "value doesn't compare equal to itself\n"; 
                            return false;
                        }
#if 0
                        if(std::any_of(std::next(it), end,
                                       [&](auto&& v){
                                           return bool(t0.cast(*it) == t1.cast(v))
                                               && bool(t0.cast(v) == t1.cast(*it));
                                       })) {
                            out << "values compare equal which shouldn't\n";
                            return false;
                        }
#endif
                        }
                }
                return true;
            })) {
            return false;
        }
        for (auto it(values.begin()), end(values.end()); it != end; ++it) {
            if constexpr (cbt::is_detected_v<equality_operator, T const>) {
                if (!bool(*it == *it)) {
                    out << "value doesn't compare equal to itself\n"; 
                    return false;
                }
                if(std::any_of(std::next(it), end,
                               [&](auto&& v){ return bool(*it == v) && bool(v == *it); })) {
                    out << "values compare equal which shouldn't\n";
                    return false;
                }
            }
            else {
                out << cbt::level::error << " operator==(): not defined for " << name << "\n";
                return false;
            }
            if constexpr (cbt::is_detected_v<inequality_operator, T>) {
                if (bool(*it != *it)) {
                    out << "value does compare unequal to itself\n"; 
                    return false;
                }
                if(std::any_of(std::next(it), end,
                               [&](auto&& v){ return !bool(*it != v) || !bool(v != *it); })) {
                    out << "values don't compare unequal which should\n";
                    return false;
                }
            }
            else {
                out << cbt::level::error << " operator!=(): not defined for " << name << "\n";
                return false;
            }
        }
        return true;
    }

    // ------------------------------------------------------------------------

    template <typename T>
    using TypeName = decltype(T::name);
    
    template <typename T>
    std::string getName() {
        if constexpr (is_detected_v<TypeName, T>) {
                return T::name;
        }
        return typeid(T).name();
    }

    // ------------------------------------------------------------------------

    template <typename S, typename T = S, typename...>
    using EqualityOperation = decltype(std::declval<S>() == std::declval<T>());
    template <typename S, typename T = S, typename...>
    using InequalityOperation = decltype(std::declval<S>() != std::declval<T>());
    
    template <typename T>
    bool testEqualityComparable(std::initializer_list<std::initializer_list<T>> es) {
        if (!is_detected_v<EqualityOperation, T>) {
            std::cerr << "requires(" << getName<T>() << " a, " << getName<T>() << " b) { "
                      << "a == b; } not satisfied\n";
            if (is_detected_v<InequalityOperation, T>) {
                std::cerr << "potential implementation:\n"
                          << "bool operator==(" << getName<T>() << " const& a, "
                                                << getName<T>() << " const& b) {\n"
                          << "    return !(a != b)\n"
                          << "}\n";
            }
            return false;
        }
        else {
            T const& x = *(*es.begin()).begin();
            if (!(x == x))
                std::cerr << "a == a is false (not reflexive)\n";
            return false;
        }
        std::cerr << "*** all is good! ***: " << getName<T>() << "\n";
        return true;
    }
}

// ----------------------------------------------------------------------------

#endif
