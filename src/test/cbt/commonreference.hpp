// cbt/commonreference.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_CBT_COMMONREFERENCE
#define INCLUDED_CBT_COMMONREFERENCE

// ----------------------------------------------------------------------------

#include "cbt/convertibleto.hpp"
#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include "cbt/test.hpp"
#include <ostream>
#include <string>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {
    // template <class T, class U>
    // concept CommonReference =
    //     Same<common_reference_t<T, U>, common_reference_t<U, T>>
    //     && ConvertibleTo<T, common_reference_t<T, U>>
    //     && ConvertibleTo<U, common_reference_t<T, U>>
    //     ;

    template <typename T, typename U>
    bool CommonReference(std::ostream& out, std::string_view name) {
        std::string n(name);
        return cbt::test({ [&]{
                               if (!std::is_same_v<std::common_type_t<T, U>,
                                                   std::common_type_t<U, T>>) {
                                   out << cbt::level::error << " common_reference for "
                                       << name << " isn't symmetric\n";
                                   return false;
                               }
                               return true;
                           }()
                         , cbt::ConvertibleTo<T, std::common_type_t<T, U>>(out, n, n)
                         , cbt::ConvertibleTo<U, std::common_type_t<T, U>>(out, n, n)
                         });
    }
}


// ----------------------------------------------------------------------------

#endif
