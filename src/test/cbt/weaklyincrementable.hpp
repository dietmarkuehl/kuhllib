// cbt/weaklyincrementable.hpp                                        -*-C++-*-
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

#ifndef INCLUDED_CBT_WEAKLYINCREMENTABLE
#define INCLUDED_CBT_WEAKLYINCREMENTABLE

#include "cbt/signedintegral.hpp"
#include "cbt/semiregular.hpp"
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

namespace cbt {
    // template <typename WeakInc>
    // concept WeaklyIncrementable =
    //     Semiregular<WeakInc>
    //     && requires(WeakInc i) {
    //        typename difference_type_t<WeakInc>;
    //        requires SignedIntegral<difference_type_t<WeakInc>>;
    //        { ++i } -> Same<WeakInc>&; // not required to be equality preserving
    //        i++; // not required to be equality preserving
    //        };

    template <typename WeakInc>
    bool WeaklyIncrementable(std::ostream& out, std::string_view name,
                             WeakInc&& begin, WeakInc&& end) {
        (void)begin;
        (void)end;
        using T = std::decay_t<WeakInc>;
        return cbt::test({ cbt::Semiregular<T>(out, name, begin)
                         , cbt::TypeRequirement<cbt::difference_type_t, T>(out, name, "difference_type")
                         , cbt::NestedRequirement<cbt::difference_type_t, T>(out, "difference_type_t<" + std::string(name) + ">",
                             [](std::ostream& out, auto name) {
                                 return cbt::SignedIntegral<cbt::difference_type_t<T>>(out, name);
                             })
                         , cbt::CompoundRequirement<cbt::OpPreIncrement, WeakInc&, WeakInc>(out, name, "pre increment")
                         , cbt::SimpleRequirement<cbt::OpPostIncrement, WeakInc>(out, name, "post increment")
                    , [&]{
                    if constexpr (cbt::is_detected_v<OpPreIncrement, T>
                                  && cbt::is_detected_v<CopyCtorFor, WeakInc>
                                  ) {
                            T     tmp(begin);
                            auto* orig = &tmp;
                            if (orig != &++tmp) {
                                out << cbt::level::error << " preincrement operator for "
                                    << name << " doesn't return a reference to itself\n";
                                return false;
                            }
                        }
                    else {
                        out << cbt::level::warning << " preincrement for " << name
                            << " isn't checked\n";
                    }
                    return true;
                }()
                         });
    }
}


// ----------------------------------------------------------------------------

#endif
