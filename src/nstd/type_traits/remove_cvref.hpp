// nstd/type_traits/remove_cvref.hpp                                  -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_REMOVE_CVREF
#define INCLUDED_NSTD_TYPE_TRAITS_REMOVE_CVREF

#include "nstd/type_traits/remove_cv.hpp"
#include "nstd/type_traits/remove_reference.hpp"

// ----------------------------------------------------------------------------

namespace nstd::type_traits {
    template <typename>
    struct remove_cvref;

    template <typename T>
    using remove_cvref_t = typename ::nstd::type_traits::remove_cvref<T>::type;
}

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::type_traits::remove_cvref
{
    using type = ::nstd::type_traits::remove_cv_t<::nstd::type_traits::remove_reference_t<T>>;

    remove_cvref() = default;
    remove_cvref(type did_you_mean_to_use_remove_cvref_t) = delete;
};

// ----------------------------------------------------------------------------

#endif
