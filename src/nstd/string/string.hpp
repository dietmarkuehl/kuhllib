// nstd/string/string.hpp                                             -*-C++-*-
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

#ifndef INCLUDED_NSTD_STRING_STRING
#define INCLUDED_NSTD_STRING_STRING

#include "nstd/string/string_fwd.hpp"
#include "nstd/iostream/ostream_fwd.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    template <typename cT, typename Traits, typename Allocator>
    auto operator== (nstd::basic_string<cT, Traits, Allocator> const&,
                     nstd::basic_string<cT, Traits, Allocator> const&)
        -> bool;
    template <typename cT, typename Traits, typename Allocator>
    auto operator!= (nstd::basic_string<cT, Traits, Allocator> const&,
                     nstd::basic_string<cT, Traits, Allocator> const&)
        -> bool;

    template <typename cT, typename Traits, typename Allocator>
    auto operator== (nstd::basic_string<cT, Traits, Allocator> const&,
                     cT const*)
        -> bool;
    template <typename cT, typename Traits, typename Allocator>
    auto operator!= (nstd::basic_string<cT, Traits, Allocator> const&,
                     cT const*)
        -> bool;

    template <typename cT, typename Traits, typename Allocator>
    auto operator== (cT const*,
                     nstd::basic_string<cT, Traits, Allocator> const&)
        -> bool;
    template <typename cT, typename Traits, typename Allocator>
    auto operator!= (cT const*,
                     nstd::basic_string<cT, Traits, Allocator> const&)
        -> bool;

    template <typename cT, typename Traits, typename Allocator>
    auto operator<< (nstd::basic_ostream<cT, Traits>&,
                     nstd::basic_string<cT, Traits, Allocator> const&)
        -> nstd::basic_ostream<cT, Traits>&;
}

// ----------------------------------------------------------------------------

template <typename cT, typename Traits, typename Allocator>
class nstd::basic_string {
    cT* start;
public:
    basic_string();
    basic_string(cT const*);
    basic_string(basic_string const&) = delete;
    basic_string(basic_string&&) = delete;
    ~basic_string();
};

// ----------------------------------------------------------------------------

template <typename cT, typename Traits, typename Allocator>
nstd::basic_string<cT, Traits, Allocator>::basic_string()
    : start() {
}

template <typename cT, typename Traits, typename Allocator>
nstd::basic_string<cT, Traits, Allocator>::basic_string(cT const*)
    : start() {
    //-dk:TODO copy the string
}

template <typename cT, typename Traits, typename Allocator>
nstd::basic_string<cT, Traits, Allocator>::~basic_string()
{
    //dk:TODO use allocator faffing about
    delete[] this->start;
}

// ----------------------------------------------------------------------------

template <typename cT, typename Traits, typename Allocator>
auto nstd::operator== (cT const*,
                       nstd::basic_string<cT, Traits, Allocator> const&)
    -> bool {
    //-dk:TODO compare the actual strings
    return false;
}
template <typename cT, typename Traits, typename Allocator>
auto nstd::operator!= (cT const* cstr,
                       nstd::basic_string<cT, Traits, Allocator> const& str)
    -> bool {
    return !(cstr == str);
}

// ----------------------------------------------------------------------------

template <typename cT, typename Traits, typename Allocator>
auto nstd::operator<< (nstd::basic_ostream<cT, Traits>& out,
                       nstd::basic_string<cT, Traits, Allocator> const&)
    -> nstd::basic_ostream<cT, Traits>& {
    //-dk:TODO sent the actual string
    return out;
}

// ----------------------------------------------------------------------------

#endif
