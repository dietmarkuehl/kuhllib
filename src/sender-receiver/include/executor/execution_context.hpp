// include/executor/execution_context.hpp                             -*-C++-*-
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

#ifndef INCLUDED_EXECUTOR_EXECUTION_CONTEXT
#define INCLUDED_EXECUTOR_EXECUTION_CONTEXT

#include <stdexcept>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    enum class fork_event {};
    class execution_context;

    class service_already_exists;

    template<typename Service>
    typename Service::key_type& use_service(execution_context& ctx);
    template<typename Service, typename... Args>
    Service& make_service(execution_context& ctx, Args&&... args);
    template<typename Service>
    bool has_service(execution_context const& ctx) noexcept;
}

// ----------------------------------------------------------------------------

class cxxrt::net::service_already_exists
    : public std::logic_error
{
};

// ----------------------------------------------------------------------------

class cxxrt::net::execution_context
{
public:
    class service;

    execution_context();
    execution_context(execution_context const&) = delete;
    execution_context& operator=(execution_context const&) = delete;
    virtual ~execution_context();

    void notify_fork(cxxrt::net::fork_event);

protected:
    void shutdown() noexcept;
    void destroy() noexcept;
};

// ----------------------------------------------------------------------------

class cxxrt::net::execution_context::service
{
private:
    execution_context& d_owner;
    
    virtual void shutdown() noexcept = 0;
    virtual void notify_fork(fork_event e);

protected:
    explicit service(execution_context& owner);
    service(service const&) = delete;
    service& operator=(service const&) = delete;
    virtual ~service();

    execution_context& context() noexcept;
};

// ----------------------------------------------------------------------------

inline cxxrt::net::execution_context&
cxxrt::net::execution_context::service::context() noexcept
{
    return this->d_owner;
}

// ----------------------------------------------------------------------------

#endif
