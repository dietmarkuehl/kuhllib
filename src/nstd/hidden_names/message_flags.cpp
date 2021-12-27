// nstd/hidden_names/message_flags.cpp                                -*-C++-*-
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

#include <nstd/hidden_names/message_flags.hpp>
#include <ostream>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    int message_flags_dummy{0};
}

auto nstd::hidden_names::operator<< (::std::ostream& out, ::nstd::hidden_names::message_flags flags)
    -> ::std::ostream&
{
    char const* sep("");
    auto format_flag([flags, &out, &sep](auto flag, char const* name){
        if ((flag & flags) != ::nstd::hidden_names::message_flags()){
            out << sep << name;
            sep = "|";
        }
    });
    format_flag(::nstd::hidden_names::message_flags::peek, "peek");
    format_flag(::nstd::hidden_names::message_flags::out_of_band, "out_of_band");
    format_flag(::nstd::hidden_names::message_flags::do_not_route, "do_not_route");
    return out;
}