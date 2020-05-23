// src/executor/execution_context.cpp                                 -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include <executor/execution_context.hpp>

// ----------------------------------------------------------------------------

int executor_execution_context = 0;

// ----------------------------------------------------------------------------

cxxrt::net::execution_context::execution_context()
{
}

cxxrt::net::execution_context::~execution_context()
{
    this->shutdown();
    this->destroy();
}

// ----------------------------------------------------------------------------

void cxxrt::net::execution_context::shutdown() noexcept
{
    //-dk:TODO shutdown the services
}

void cxxrt::net::execution_context::destroy() noexcept
{
    //-dk:TODO destroy the services
}

// ----------------------------------------------------------------------------

cxxrt::net::execution_context::service::service(execution_context& owner)
    : d_owner(owner)
{
}

cxxrt::net::execution_context::service::~service()
{
}

void cxxrt::net::execution_context::service::notify_fork(fork_event)
{
}
