// nstd/iterator/random_access.hpp                                    -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2016 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_ITERATOR_RANDOM_ACCESS
#define INCLUDED_NSTD_ITERATOR_RANDOM_ACCESS

#include <iterator>
#include "nstd/iterator/iterator_traits.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include "nstd/type_traits/decay.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    namespace iterator {
        namespace detail {
            template <typename> struct test_random_access;
        }

        using ::std::random_access_iterator_tag;

        template <typename> struct is_random_access;
        template <typename Iterator>
        constexpr bool is_random_access_v
            = ::nstd::iterator::is_random_access<Iterator>::value;
    }
}

// ----------------------------------------------------------------------------

template <typename Iterator>
struct nstd::iterator::detail::test_random_access {
    using decay_type = ::nstd::type_traits::decay_t<Iterator>;

    template <typename I>
    static char (&test(I*, typename ::nstd::iterator::traits<I>::iterator_category**))[1];
    template <typename I>
    static char (&test(I*, ...))[2];
    static constexpr bool value = 1u == sizeof(test(static_cast<decay_type*>(nullptr),
                                                    static_cast<::nstd::iterator::random_access_iterator_tag**>(nullptr)));
};

// ----------------------------------------------------------------------------

template <typename Iterator>
struct nstd::iterator::is_random_access
    : ::nstd::type_traits::integral_constant<bool,
        ::nstd::iterator::detail::test_random_access<Iterator>::value> {
};

// ----------------------------------------------------------------------------

#endif
