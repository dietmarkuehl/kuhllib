// nstd/cursor/category.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_CURSOR_CATEGORY
#define INCLUDED_NSTD_CURSOR_CATEGORY

#include "nstd/type_traits/choose_type.hpp"
#include "nstd/type_traits/is_convertible.hpp"
#include "nstd/type_traits/remove_cv.hpp"

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace cursor
    {
        struct single_pass;
        struct forward;
        struct bidirectional;
        struct random_access;
        
        template <typename T> struct category;
        template <typename T>
        using category_t = typename nstd::cursor::category<T>::type;

        template <> struct category<wchar_t> { typedef random_access type; };
        template <> struct category<char16_t> { typedef random_access type; };
        template <> struct category<char32_t> { typedef random_access type; };
        template <> struct category<char> { typedef random_access type; };
        template <> struct category<signed char> { typedef random_access type; };
        template <> struct category<unsigned char> { typedef random_access type; };
        template <> struct category<signed short> { typedef random_access type; };
        template <> struct category<unsigned short> { typedef random_access type; };
        template <> struct category<signed int> { typedef random_access type; };
        template <> struct category<unsigned int> { typedef random_access type; };
        template <> struct category<signed long> { typedef random_access type; };
        template <> struct category<unsigned long> { typedef random_access type; };
        template <> struct category<signed long long> { typedef random_access type; };
        template <> struct category<unsigned long long> { typedef random_access type; };
    }
}

// ----------------------------------------------------------------------------

struct nstd::cursor::single_pass
{
};

struct nstd::cursor::forward
    : nstd::cursor::single_pass
{
};

struct nstd::cursor::bidirectional
    : nstd::cursor::forward
{
};

struct nstd::cursor::random_access
    : nstd::cursor::bidirectional
{
};

// ----------------------------------------------------------------------------

template <typename T>
struct nstd::cursor::category
    : nstd::type_traits::choose_type<
        nstd::type_traits::choice<nstd::type_traits::is_convertible<nstd::type_traits::remove_cv_t<T>*, nstd::cursor::random_access*>::value, nstd::cursor::random_access>,
        nstd::type_traits::choice<nstd::type_traits::is_convertible<nstd::type_traits::remove_cv_t<T>*, nstd::cursor::bidirectional*>::value, nstd::cursor::bidirectional>,
        nstd::type_traits::choice<nstd::type_traits::is_convertible<nstd::type_traits::remove_cv_t<T>*, nstd::cursor::forward*>::value, nstd::cursor::forward>,
        nstd::type_traits::choice<nstd::type_traits::is_convertible<nstd::type_traits::remove_cv_t<T>*, nstd::cursor::single_pass*>::value, nstd::cursor::single_pass>
    >
{
};

// ----------------------------------------------------------------------------

#endif
