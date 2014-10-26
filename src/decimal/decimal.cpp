// decimal.cpp -*-C++-*-
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

#include "decimal.hpp"
#include "decimal_num_put.hpp"
#include <ostream>

// ----------------------------------------------------------------------------

template <typename cT, typename Tr, int Bits>
std::basic_ostream<cT, Tr>&
kuhllib::operator<< (std::basic_ostream<cT, Tr>& out,
                     kuhllib::decimal<Bits> const& value)
{
    try {
        std::ostream::sentry kerberos(out);
        if (kerberos) {
            typedef kuhllib::decimal_num_put<cT> facet;
            kuhllib::use_facet<facet>(out.getloc()).put(out, out, out.fill(), value);
        }
    }
    catch (std::exception const& ex) {
        try {
            out.setstate(std::ios_base::badbit);
        }
        catch (...) {
        }
        if (out.rdstate() & std::ios_base::badbit) {
            throw;
        }
    }
    return out;
}

template
std::basic_ostream<char, std::char_traits<char> >&
kuhllib::operator<< (std::basic_ostream<char, std::char_traits<char>>&,
                     kuhllib::decimal<32> const&);
template
std::basic_ostream<char, std::char_traits<char> >&
kuhllib::operator<< (std::basic_ostream<char, std::char_traits<char>>&,
                     kuhllib::decimal<64> const&);
