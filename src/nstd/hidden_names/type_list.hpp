// nstd/hidden_names/type_list.hpp                                    -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_TYPE_LIST
#define INCLUDED_NSTD_HIDDEN_NAMES_TYPE_LIST

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename...> struct type_list {}; //-dk:TODO add tests

    template <template <typename...> class, typename>
    struct type_list_transform;
    template <template <typename...> class To, template <typename...> class From, typename... T>
    struct type_list_transform<To, From<T...>> {
        using types = To<T...>;
    };

    template <template <typename...> class, template <typename...> class, typename>
    struct type_list_nested_transform;
    template <template <typename...> class ToV, template <typename...> class ToT,
              template <typename...> class FromV, typename... Vs>
    struct type_list_nested_transform<ToV, ToT, FromV<Vs...>> {
        using types = ToV<typename ::nstd::hidden_names::type_list_transform<ToT, Vs>::types...>;
    };
}


// ----------------------------------------------------------------------------

#endif
