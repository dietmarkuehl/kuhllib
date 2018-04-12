// detection.hpp                                                      -*-C++-*-
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

#ifndef INCLUDED_DETECTION
#define INCLUDED_DETECTION

#include "cbt/level.hpp"
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {
    struct nonesuch {
        nonesuch(nonesuch&) = delete;
        void operator=(nonesuch&) = delete;
        ~nonesuch() = delete;
    };

    template <typename T, template <typename...> class Op, typename, typename...>
    struct detect
        : std::false_type {
        using type = T;
    };
    template <typename T, template <typename...> class Op, typename... Args>
    struct detect<T, Op, std::void_t<Op<Args...>>, Args...>
        : std::true_type {
        using type = Op<Args...>;
    };

    template <template <typename...> class Op, typename... Args>
    using is_detected = detect<nonesuch, Op, void, Args...>;
    template <template <typename...> class Op, typename... Args>
    constexpr bool is_detected_v = is_detected<Op, Args...>::value;
    template <template <typename...> class Op, typename... Args>
    using detected_t = typename detect<nonesuch, Op, void, Args...>::type;
    template <typename Default, template <typename...> class Op, typename... Args>
    using detected_or_t = typename detect<Default, Op, void, Args...>::type;

    template <typename T>
    struct remove_rvalue_reference {
        typedef T type;
    };
    template <typename T>
    struct remove_rvalue_reference<T&&> {
        typedef T type;
    };
    template <typename T>
    using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

    template <typename T, typename Op, typename, typename...>
    struct detect_nested
        : std::false_type {
        using type = T;
    };
    template <typename T, typename Op, typename... Args>
    struct detect_nested<T, Op, std::void_t<decltype(Op::template test<Args...>())>, Args...>
        : std::true_type {
        using type = decltype(Op::template test<Args...>());
        using conditional_type = type;
    };

    template <typename Op, typename... Args>
    using is_detect_nested = detect_nested<nonesuch, Op, void, Args...>;
    template <typename Op, typename... Args>
    constexpr bool is_detect_nested_v = is_detect_nested<Op, Args...>::value;

    template <typename Op, typename... Args>
    using if_detect_nested = remove_rvalue_reference_t<typename detect_nested<nonesuch, Op, void, Args...>::conditional_type>;

    // ------------------------------------------------------------------------

    template <template <typename...> class Op, typename... Args>
    bool SimpleRequirement(std::ostream& out, std::string_view name, std::string_view op) {
        if (!cbt::is_detected_v<Op, Args...>) {
            out << cbt::level::error << " " << op << " not detected for " << name << "\n";
            return false;
        }
        return true;
    }
    template <template <typename...> class Op, typename T, typename... Args>
    bool CompoundRequirement(std::ostream& out, std::string_view name, std::string_view op) {
        if (!cbt::is_detected_v<Op, Args...>) {
            out << cbt::level::error << " " << op << " not detected for " << name << "\n";
            return false;
        }
#if 0
        if (!std::is_convertible_v<cbt::detected_t<Op, Args...>, T>) {
            out << cbt::level::error << " " << op << " for " << name << " has wrong type\n";
            return false;
        }
#endif
        return true;
    }

    template <typename S, typename T, typename...>
    using Same = std::is_same<S, T>;

    template <template <typename...> class Op,
              template <typename...> class Concept, typename... Args>
    bool CompoundConceptRequirement(std::ostream& out,
                                    std::string_view name,
                                    std::string_view cname,
                                    std::string_view op) {
        out << "compound concept requirement\n";
        if (!cbt::is_detected_v<Op, Args...>) {
            out << cbt::level::error << " " << op << " not detected for " << name << "\n";
            return false;
        }
        if (!Concept<Args...>()) {
            out << cbt::level::error << " " << op << " for " << name << " doesn't match " << cname << "\n";
            return false;
        }
        return true;
    }

    template <template <typename...> class Type, typename... Args>
    bool TypeRequirement(std::ostream& out, std::string_view name, std::string_view type) {
        if (!cbt::is_detected_v<Type, Args...>) {
            out << cbt::level::error << " no nested type " << type << " for type " << name << "\n";
            return false;
        }
        return true;
    }

    template <template <typename...> class Type, typename T, typename Pred>
    bool NestedRequirement(std::ostream& out, std::string_view name, Pred pred) {
        if constexpr (cbt::is_detected_v<Type, T>) {
            return pred(out, name);
        }
        else {
            (void)pred;
        }
        out << cbt::level::error << " type " << name << " for nested requirement doesn't exist\n";
        return false;
    }

    // ------------------------------------------------------------------------
}

// ----------------------------------------------------------------------------

#endif
