// nstd/iostream/fstream_fwd.hpp                                      -*-C++-*-
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

#ifndef INCLUDED_NSTD_IOSTREAM_FSTREAM_FWD
#define INCLUDED_NSTD_IOSTREAM_FSTREAM_FWD

#include "nstd/string/char_traits_fwd.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    template <typename cT, typename = ::nstd::char_traits<cT> >
    class basic_filebuf;
    template <typename cT, typename = ::nstd::char_traits<cT> >
    class basic_ifstream;
    template <typename cT, typename = ::nstd::char_traits<cT> >
    class basic_ofstream;
    template <typename cT, typename = ::nstd::char_traits<cT> >
    class basic_fstream;

    using filebuf  = ::nstd::basic_filebuf<char>;
    using wfilebuf = ::nstd::basic_filebuf<wchar_t>;
    using ifstream  = ::nstd::basic_ifstream<char>;
    using wifstream = ::nstd::basic_ifstream<wchar_t>;
    using ofstream  = ::nstd::basic_ofstream<char>;
    using wofstream = ::nstd::basic_ofstream<wchar_t>;
    using fstream  = ::nstd::basic_fstream<char>;
    using wfstream = ::nstd::basic_fstream<wchar_t>;
}

// ----------------------------------------------------------------------------

#endif