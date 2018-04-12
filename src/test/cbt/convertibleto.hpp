// cbt/convertibleto.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_CBT_CONVERTIBLETO
#define INCLUDED_CBT_CONVERTIBLETO

// ----------------------------------------------------------------------------

#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include "cbt/test.hpp"
#include <ostream>
#include <string>
#include <utility>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {
    // template <class From, class To>
    // concept ConvertibleTo =
    //     is_convertible<From, To>::value
    //     && requires(From (&f)()) { static_cast<To>(f()); }
    //     ;
    template <typename From> From ConvertibleToFun();

    template <typename From, typename To>
    using ConvertibleToCast = decltype(static_cast<To>(ConvertibleToFun<From>()));

    template <typename From, typename To>
    bool ConvertibleTo(std::ostream& out, std::string_view from, std::string_view to) {
        return cbt::test({ [&]{
                               if (!std::is_convertible_v<From, To>) {
                                   out << cbt::level::error << " " << from << " "
                                       << "isn't convertible to " << to << "\n";
                                   return false;
                               }
                               return true;
                           }()
                         , cbt::SimpleRequirement<ConvertibleToCast, From, To>(out, "static_cast<" + std::string(to) + ">(f<" + std::string(from) + ">)", "conversion")
                         });
    }
}


// ----------------------------------------------------------------------------

#endif
