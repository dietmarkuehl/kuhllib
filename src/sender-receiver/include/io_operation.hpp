// include/io_operation.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_IO_OPERATION
#define INCLUDED_IO_OPERATION

#include <io_context.hpp>
#include <execution/set_value.hpp>
#include <execution/set_error.hpp>
#include <execution/set_done.hpp>

#include <exception>
#include <system_error>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    template <typename...> class io_operation_base;
    template <typename, template <typename...> class, typename... >
    class io_operation;
}

// ----------------------------------------------------------------------------

template <typename... A>
class cxxrt::net::io_operation_base
    : public cxxrt::net::detail::waiter
{
protected:
    virtual void do_set_value(A...) = 0;
    virtual void do_set_error(std::error_code const&) noexcept = 0;
    virtual void do_set_error(std::exception_ptr const&) noexcept = 0;
    virtual void do_set_done() noexcept = 0;

public:
    void set_value(A... a) noexcept
        try
        {
            this->do_set_value(std::forward<A>(a)...);
        }
        catch(...)
        {
            this->set_error(std::current_exception());
        }
    void set_error(std::error_code const& ec) noexcept
    {
        this->do_set_error(ec);
    }
    void set_error(std::exception_ptr const& ep) noexcept
    {
        this->do_set_error(ep);
    }
    void set_done() noexcept
    {
        this->do_set_done();
    }
};

// ----------------------------------------------------------------------------

template <typename R, template <typename...> class Base, typename... A>
class cxxrt::net::io_operation
    : public Base<A...>
{
private:
    std::remove_cvref_t<R> d_receiver;
        
protected:
    void do_set_value(A... a) override
    {
        cxxrt::execution::set_value(std::move(this->d_receiver),
                                    std::forward<A>(a)...);
    }
    void do_set_error(std::error_code const& ec) noexcept override
    {
        cxxrt::execution::set_error(std::move(this->d_receiver), ec);
    }
    void do_set_error(std::exception_ptr const& ep) noexcept override
    {
        cxxrt::execution::set_error(std::move(this->d_receiver), ep);
    }
    void do_set_done() noexcept override
    {
        cxxrt::execution::set_done(std::move(this->d_receiver));
    }

public:
    template <typename T>
    io_operation(T&& r): d_receiver(std::forward<T>(r)) {}
};


// ----------------------------------------------------------------------------

#endif
