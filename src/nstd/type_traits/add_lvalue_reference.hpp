// nstd/type_traits/add_lvalue_reference.hpp                          -*-C++-*-
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

#ifndef INCLUDED_NSTD_TYPE_TRAITS_ADD_LVALUE_REFERENCE
#define INCLUDED_NSTD_TYPE_TRAITS_ADD_LVALUE_REFERENCE

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace type_traits {
        template <typename> struct add_lvalue_reference;
        template <> struct add_lvalue_reference<void>;
        template <> struct add_lvalue_reference<void const>;
        template <> struct add_lvalue_reference<void volatile>;
        template <> struct add_lvalue_reference<void const volatile>;

        template <typename T>
        using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
    }

}

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::type_traits::add_lvalue_reference
{
    using type = T&;
};

template <>
struct nstd::type_traits::add_lvalue_reference<void>
{
    using type = void;
};

template <>
struct nstd::type_traits::add_lvalue_reference<void const>
{
    using type = void const;
};

template <>
struct nstd::type_traits::add_lvalue_reference<void volatile>
{
    using type = void volatile;
};

template <>
struct nstd::type_traits::add_lvalue_reference<void const volatile>
{
    using type = void const volatile;
};

// ----------------------------------------------------------------------------

#endif
