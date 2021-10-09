// nstd/execution/inplace_stop_token.hpp                              -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_INPLACE_STOP_TOKEN
#define INCLUDED_NSTD_EXECUTION_INPLACE_STOP_TOKEN

// ----------------------------------------------------------------------------

namespace nstd::execution {
    class inplace_stop_state;
    class inplace_stop_token;
}

// ----------------------------------------------------------------------------

class nstd::execution::inplace_stop_state
{
public:
    friend class ::nstd::execution::inplace_stop_token;

    auto token() const -> ::nstd::execution::inplace_stop_token;
};

// ----------------------------------------------------------------------------

class nstd::execution::inplace_stop_token
{
    friend class ::nstd::execution::inplace_stop_state;
private:
    ::nstd::execution::inplace_stop_state* d_stop;

    explicit inplace_stop_token(::nstd::execution::inplace_stop_state* stop)
        : d_stop(stop)
    {
    }
public:
    auto operator== (inplace_stop_token const& other) const noexcept -> bool = default;
    auto stop_requested() const noexcept -> bool;
    auto stop_possible() const noexcept -> bool;
};


// ----------------------------------------------------------------------------

#endif