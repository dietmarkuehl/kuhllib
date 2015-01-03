// nstd/type_traits/add_volatile.hpp                                  -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_ADD_VOLATILE
#define INCLUDED_NSTD_TYPE_TRAITS_ADD_VOLATILE

// ----------------------------------------------------------------------------

namespace nstd {
    namespace type_traits {
        template <typename> struct add_volatile;
        template <typename T> struct add_volatile<T&>;
        template <typename R, typename... A> struct add_volatile<R(A...)>;

        template <typename T>
        using add_volatile_t = typename ::nstd::type_traits::add_volatile<T>::type;
    }
}

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::type_traits::add_volatile<T&> {
    using type = T&;
};
template <typename R, typename... A>
struct nstd::type_traits::add_volatile<R(A...)> {
    using type = R(A...);
};
template <typename T>
struct nstd::type_traits::add_volatile {
    using type = T volatile;
};

// ----------------------------------------------------------------------------

#endif
