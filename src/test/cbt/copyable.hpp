// cbt/copyable.hpp                                                   -*-C++-*-
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

#ifndef INCLUDED_CBT_COPYABLE
#define INCLUDED_CBT_COPYABLE

// ----------------------------------------------------------------------------

#include "cbt/copyconstructible.hpp"
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
    // concept bool Copyable =
    //     CopyConstructible<T>
    //     && Movable<T>
    //     && Assignable<T&, const T&>;

    template <typename T>
    bool Copyable(std::ostream& out, std::string_view name, T&& value) {
        using V = std::decay_t<T>;
        return cbt::test({ cbt::CopyConstructible<V>(out, name, value)
                         , cbt::Assignable<V&, V const&>(out, name, value)
                         , cbt::Movable(out, name, std::move(value))
                         });
    }
}


// ----------------------------------------------------------------------------

#endif
