// nstd/iostream/sstream_fwd.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_NSTD_IOSTREAM_SSTREAM_FWD
#define INCLUDED_NSTD_IOSTREAM_SSTREAM_FWD

#include "nstd/memory/allocator_fwd.hpp"
#include "nstd/string/char_traits_fwd.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    template <typename cT, typename = ::nstd::char_traits<cT>, typename = ::nstd::allocator<cT> >
    class basic_stringbuf;
    template <typename cT, typename = ::nstd::char_traits<cT>, typename = ::nstd::allocator<cT> >
    class basic_istringstream;
    template <typename cT, typename = ::nstd::char_traits<cT>, typename = ::nstd::allocator<cT> >
    class basic_ostringstream;
    template <typename cT, typename = ::nstd::char_traits<cT>, typename = ::nstd::allocator<cT> >
    class basic_stringstream;

    using stringbuf  = ::nstd::basic_stringbuf<char>;
    using wstringbuf = ::nstd::basic_stringbuf<wchar_t>;
    using istringstream  = ::nstd::basic_istringstream<char>;
    using wistringstream = ::nstd::basic_istringstream<wchar_t>;
    using ostringstream  = ::nstd::basic_ostringstream<char>;
    using wostringstream = ::nstd::basic_ostringstream<wchar_t>;
    using stringstream  = ::nstd::basic_stringstream<char>;
    using wstringstream = ::nstd::basic_stringstream<wchar_t>;
}

// ----------------------------------------------------------------------------

#endif
