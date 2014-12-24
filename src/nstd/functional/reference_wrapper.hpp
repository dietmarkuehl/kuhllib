// nstd/functional/reference_wrapper.hpp                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_FUNCTIONAL_REFERENCE_WRAPPER
#define INCLUDED_NSTD_FUNCTIONAL_REFERENCE_WRAPPER

// ----------------------------------------------------------------------------

namespace nstd
{
    namespace functional {
        template <typename> class reference_wrapper;

        template <typename T>
        nstd::functional::reference_wrapper<T> ref(T&) noexcept(true);
        template <typename T>
        nstd::functional::reference_wrapper<T> ref(nstd::functional::reference_wrapper<T>) noexcept(true);
        template <typename T>
        nstd::functional::reference_wrapper<T const> cref(T const&) noexcept(true);
        template <typename T>
        nstd::functional::reference_wrapper<T const> cref(nstd::functional::reference_wrapper<T>) noexcept(true);

        template <typename T> void ref(T const&&) = delete;
        template <typename T> void cref(T const&&) = delete;
    }

}

// ----------------------------------------------------------------------------

template <typename>
class nstd::functional::reference_wrapper
{
};

// ----------------------------------------------------------------------------

#endif
