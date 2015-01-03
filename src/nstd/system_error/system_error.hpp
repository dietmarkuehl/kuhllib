// nstd/system_error/system_error.hpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_SYSTEM_ERROR_SYSTEM_ERROR
#define INCLUDED_NSTD_SYSTEM_ERROR_SYSTEM_ERROR

#include "nstd/system_error/error_code.hpp"
#include "nstd/system_error/error_category.hpp"
#include "nstd/string/string_fwd.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    class system_error;
}

// ----------------------------------------------------------------------------

class nstd::system_error {
public:
    system_error(::nstd::error_code, ::nstd::string const&);
    system_error(::nstd::error_code, char const*);
    system_error(::nstd::error_code);
    system_error(int, ::nstd::error_category const&, ::nstd::string const&);
    system_error(int, ::nstd::error_category const&, char const*);
    system_error(int, ::nstd::error_category const&);
    auto code() const noexcept(true) -> ::nstd::error_code const&;
    auto what() const noexcept(true) -> char const*;
};

// ----------------------------------------------------------------------------

#endif
