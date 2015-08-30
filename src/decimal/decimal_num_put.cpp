// decimal_num_put.cpp                                                -*-C++-*-
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

#include "decimal_num_put.hpp"

// ----------------------------------------------------------------------------

template class kuhllib::decimal_num_put<char, std::ostreambuf_iterator<char>>;
template class kuhllib::decimal_num_put<wchar_t, std::ostreambuf_iterator<wchar_t>>;

// ----------------------------------------------------------------------------
template <>
kuhllib::decimal_num_put<char> const&
kuhllib::use_facet<kuhllib::decimal_num_put<char> >(std::locale const& loc)
{
    static kuhllib::decimal_num_put<char> rc(0);
    return std::has_facet<kuhllib::decimal_num_put<char> >(loc)
        ? std::use_facet<kuhllib::decimal_num_put<char> >(loc)
        : rc;
}

template <>
kuhllib::decimal_num_put<wchar_t> const&
kuhllib::use_facet<kuhllib::decimal_num_put<wchar_t> >(std::locale const& loc)
{
    static kuhllib::decimal_num_put<wchar_t> rc(0);
    return std::has_facet<kuhllib::decimal_num_put<wchar_t> >(loc)
        ? std::use_facet<kuhllib::decimal_num_put<wchar_t> >(loc)
        : rc;
}
