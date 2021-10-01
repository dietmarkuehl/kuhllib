// nstd/type_traits/condition.hpp                                     -*-C++-*-
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_CONDITION
#define INCLUDED_NSTD_TYPE_TRAITS_CONDITION

// ----------------------------------------------------------------------------

namespace nstd::type_traits {
    template <bool, typename T0, typename T1> struct condition;
    template <typename T0, typename T1> struct condition<true, T0, T1>;
    template <typename T0, typename T1> struct condition<false, T0, T1>;
        
    template <bool Condition, typename T0, typename T1>
    using condition_t = typename ::nstd::type_traits::condition<Condition, T0, T1>::type;
}

// ----------------------------------------------------------------------------

template <typename T0, typename T1>
struct nstd::type_traits::condition<true, T0, T1> {
    using type = T0;
};
template <typename T0, typename T1>
struct nstd::type_traits::condition<false, T0, T1> {
    using type = T1;
};

// ----------------------------------------------------------------------------

#endif
