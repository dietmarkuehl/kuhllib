// cbt/bidirectionaliterator.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_CBT_BIDIRECTIONALITERATOR
#define INCLUDED_CBT_BIDIRECTIONALITERATOR

// ----------------------------------------------------------------------------

#include "cbt/forwarditerator.hpp"
#include "cbt/detection.hpp"
#include "cbt/iterator_declarations.hpp"
#include "cbt/level.hpp"
#include "cbt/test.hpp"
#include <iterator>
#include <ostream>
#include <string>
#include <utility>
#include <type_traits>

// ----------------------------------------------------------------------------

// template <typename BiDirIt>
// concept BidirectionalIterator =
//     ForwardIterator<BiDirIt>
//     && DerivedFrom<iterator_category_t<BiDirIt>, bidirectional_iterator_tag>
//     && requires(BiDirIt it) {
//         { --it } -> Same<BiDirIt>&;
//         { it-- } -> Same<BiDirIt>&&;
//     };

namespace cbt {
    template <template <typename...> class Concept, typename T>
    struct ConceptBind{
        template <typename... A>
        using type = Concept<T, A...>;
    };

    template <typename BiDirIt>
    bool BidirectionalIterator(std::ostream& out, std::string_view name,
                               BiDirIt&& begin, BiDirIt&& end) {
        (void)begin;
        (void)end;
        using T = std::decay_t<BiDirIt>;
        return cbt::test({ cbt::ForwardIterator(out, name, begin, end)
                         , cbt::NestedRequirement<cbt::iterator_category_t, T>(out, "iterator_category_t<" + std::string(name) + ">",
                             [](std::ostream& out, auto name) {
                                 if (!std::is_base_of_v<std::bidirectional_iterator_tag, cbt::iterator_category_t<T>>) {
                                     out << cbt::level::error << " iterator_category of " << name << " is not derived from bidirectional_iterator_tag\n";
                                     return false;
                                 }
                                 return true;
                             })
                         , cbt::CompoundRequirement<OpPreDecrement, BiDirIt&, BiDirIt>(out, name, "pre-decrement")
                         , cbt::CompoundRequirement<OpPostDecrement, BiDirIt&&, BiDirIt>(out, name, "post-decrement")
                    , [&]{
                    if (!cbt::Same<cbt::detected_t<OpPreDecrement, BiDirIt>, BiDirIt&>()
                        && cbt::is_detected_v<OpPreDecrement, BiDirIt>) {
                            out << cbt::level::error << " return type of --" << name << " is incorrect\n";
                            return false;
                        }
                    return true;
                        }()
                    , [&]{
                    if (!cbt::Same<cbt::detected_t<OpPostDecrement, BiDirIt>, BiDirIt>()
                        && cbt::is_detected_v<OpPostDecrement, BiDirIt>) {
                            out << cbt::level::error << " return type of " << name << "-- is incorrect\n";
                            return false;
                        }
                    return true;
                        }()
#if 1
                   , [&]{
                    if (cbt::is_detected_v<OpPreDecrement, BiDirIt&>
                        && !is_detected_v<OpPostDecrement, BiDirIt&>) {
                        out << cbt::level::info << " potential implementation: " << name << " operator++(int) { "
                            << "name tmp(*this); ++*this; return tmp; }\n";
                    }
                    return true;
                }()
#endif
#if 0
                    , cbt::CompoundConceptRequirement<OpPostDecrement,
                                                      ConceptBind<Same, BiDirIt&&>::type, BiDirIt>(
                                    out, name, "post-decrement", "Same<" + std::string(name) + ">")
#endif
                    , [&]{
                    if constexpr (cbt::is_detected_v<OpPreDecrement, T>
                                  && cbt::is_detected_v<CopyCtorFor, BiDirIt>
                                  && cbt::is_detected_v<cbt::equality_operator, T>) {
                            T    it(end);
                            auto ptr = &it;
                            (void)ptr;
                            if (begin == end) {
                                out << cbt::level::warning << " predecrement for " << name << " "
                                    << "wasn't tested because begin and end are the same\n";
                                return false;
                            }
                    if constexpr (cbt::Same<cbt::detected_t<OpPreDecrement, BiDirIt>, BiDirIt&>()) {
                            if (ptr != &--it) {
                                out << cbt::level::error << " predecrement for " << name << " "
                                    << "doesn't return correct address\n";
                                return false;
                            }
                        }
                        }
                    return true;
                }()
                    , [&]{
                    if constexpr (cbt::is_detected_v<OpPreDecrement, T>
                                  && cbt::is_detected_v<CopyCtorFor, BiDirIt>
                                  && cbt::is_detected_v<cbt::equality_operator, T>) {
                            T    it(end);
                            if (begin == end) {
                                out << cbt::level::warning << " predecrement for " << name << " "
                                    << "wasn't tested because begin and end are the same\n";
                                return false;
                            }
                            int count = 0;
                            while (count != 1000 && it != begin) {
                                ++count;
                                --it;
                            }
                            if (it != begin) {
                                out << cbt::level::error << " it seems predecrement for "
                                    << name << " doesn't reach the beginning\n";
                                return false;
                            }
                        }
                    return true;
                }()
                    , [&]{
                    if constexpr (cbt::is_detected_v<OpPostDecrement, T>
                                  && cbt::is_detected_v<CopyCtorFor, BiDirIt>
                                  && cbt::is_detected_v<cbt::equality_operator, T>) {
                            T    it(end);
                            if (begin == end) {
                                out << cbt::level::warning << " postdecrement for " << name << " "
                                    << "wasn't tested because begin and end are the same\n";
                                return false;
                            }
                            int count = 0;
                            while (count != 1000 && it != begin) {
                                ++count;
                                it--;
                            }
                            if (it != begin) {
                                out << cbt::level::error << " it seems postdecrement for "
                                    << name << " doesn't reach the beginning\n";
                                return false;
                            }
                        }
                    return true;
                }()
                    , [&]{
                    if constexpr (cbt::is_detected_v<OpPreDecrement, T>
                                  && cbt::is_detected_v<OpPostDecrement, BiDirIt>
                                  && cbt::is_detected_v<CopyCtorFor, BiDirIt>
                                  && cbt::is_detected_v<cbt::equality_operator, T>) {
                            T    it1(end);
                            T    it2(end);
                            if (begin == end) {
                                out << cbt::level::warning << " predecrement/postdecrement for " << name << " "
                                    << "wasn't tested because begin and end are the same\n";
                                return false;
                            }
                            int count = 0;
                            while (count != 1000 && it1 != begin) {
                                ++count;
                                --it1;
                                it2--;
                                if (it1 != it2) {
                                    out << cbt::level::error << " pre/post decrementing " << name << " "
                                        << "yields different iterators\n";
                                    return false;
                                }
                            }
                        }
                    return true;
                }()
                         });
#if 0
        //-dk:TODO
&--a == &a.
(3.2) — If bool(a == b), then bool(a-- == b).
(3.3) — If bool(a == b), then after evaluating both a-- and --b, bool(a == b) still holds.
(3.4) — If a is incrementable and bool(a == b), then bool(--(++a) == b).
(3.5) — If bool(a == b), then bool(++(--a) == b).
#endif
    }
}

// ----------------------------------------------------------------------------

#endif
