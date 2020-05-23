// include/concepts.hpp                                               -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_CONCEPTS
#define INCLUDED_CONCEPTS

#if !defined(__clang__)
#    include <concepts>
#else

// ----------------------------------------------------------------------------

namespace std
{
    template <typename D, typename B>
    concept derived_from
        = true //-dk:TODO derived_from
        ;

    template <typename F, typename... A>
    concept invocable
        = true //-dk:TODO invocable
        ;

    template <typename t>
    concept destructible
        = true //-dk:TODO destructible
        ;
    template <typename T, typename... A>
    concept constructible_from
        = true //-dk:TODO constructible_from
        ;

    template <typename T>
    concept copy_constructible
        = true //-dk:TODO copy_constructible
        ;

    template <typename T>
    concept move_constructible
        = true //-dk:TODO move_constructible
        ;

    template <typename T>
    concept equality_comparable
        = true //-dk:TODO equality_comparable
        ;
}

// ----------------------------------------------------------------------------

#endif
#endif
