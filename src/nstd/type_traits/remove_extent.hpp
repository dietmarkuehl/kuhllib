// nstd/type_traits/remove_extent.hpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_REMOVE_EXTENT
#define INCLUDED_NSTD_TYPE_TRAITS_REMOVE_EXTENT

#include <cstddef>

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace type_traits {
        template <typename> struct remove_extent;
        template <typename T, ::std::size_t Size> struct remove_extent<T[Size]>;
        template <typename T> struct remove_extent<T[]>;
        template <typename T>
        using remove_extent_t = typename ::nstd::type_traits::remove_extent<T>::type;
    }
}

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::type_traits::remove_extent {
    using type = T;
};

template <typename T, ::std::size_t Size>
struct nstd::type_traits::remove_extent<T[Size]> {
    using type = T;
};

template <typename T>
struct nstd::type_traits::remove_extent<T[]> {
    using type = T;
};

// ----------------------------------------------------------------------------

#endif
