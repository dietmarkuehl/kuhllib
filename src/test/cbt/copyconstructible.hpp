// cbt/copyconstructible.hpp                                          -*-C++-*-
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

#ifndef INCLUDED_CBT_COPYCONSTRUCTIBLE
#define INCLUDED_CBT_COPYCONSTRUCTIBLE

// ----------------------------------------------------------------------------

#include "cbt/moveconstructible.hpp"
#include "cbt/constructible.hpp"
#include "cbt/convertibleto.hpp"
#include "cbt/movable.hpp"
#include "cbt/assignable.hpp"
#include "cbt/detection.hpp"
#include "cbt/level.hpp"
#include "cbt/test.hpp"
#include <iterator>
#include <ostream>
#include <string>
#include <utility>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace cbt {
    // template <typename T>
    // concept CopyConstructible =
    //     MoveConstructible<T>
    //     && Constructible<T, T&>
    //     && ConvertibleTo<T&, T>
    //     && Constructible<T, T const&>
    //     && ConvertibleTo<T const&, T>
    //     && Constructible<T, T const>
    //     && ConvertibleTo<T const, T>;

    template <typename V, typename T>
    bool CopyConstructible(std::ostream& out, std::string_view name, T&& value) {
        (void)value;
        std::string n(name);
        return cbt::test({ cbt::MoveConstructible<V>(out, name, std::move(value)) //-dk:TODO
                         , cbt::Constructible<V, V&>(out, n + "(" + n + "&)")
                         , cbt::ConvertibleTo<V&, V>(out, n + "&", name)
                         , cbt::Constructible<V, V const&>(out, n + "(" + n + " const&")
                         , cbt::ConvertibleTo<V const&, V>(out, n + " const&", name)
                         , cbt::Constructible<V, V const>(out, n + "(" + n + " const)")
                         , cbt::ConvertibleTo<V const, V>(out, n + " const", name)
                         });
    }
}


// ----------------------------------------------------------------------------

#endif
