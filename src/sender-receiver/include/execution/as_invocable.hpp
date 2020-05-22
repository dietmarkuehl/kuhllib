// include/execution/as_invocable.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_EXECUTION_AS_INVOCABLE
#define INCLUDED_EXECUTION_AS_INVOCABLE

#include <execution/set_done.hpp>
#include <execution/set_error.hpp>
#include <execution/set_value.hpp>

#include <exception>
#include <memory>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution::detail
{
    template <typename, typename> struct as_invocable;
}

// ----------------------------------------------------------------------------

template <typename R, typename>
struct cxxrt::execution::detail::as_invocable
{
private:
    R* d_r;

public:
    explicit as_invocable(R& r) noexcept;
    as_invocable(as_invocable&& o) noexcept;
    ~as_invocable();

    void operator()() & noexcept;
};

// ----------------------------------------------------------------------------

template <typename R, typename X>
cxxrt::execution::detail::as_invocable<R, X>::as_invocable(R& r) noexcept
    : d_r(std::addressof(r))
{
}

template <typename R, typename X>
cxxrt::execution::detail::as_invocable<R, X>::as_invocable(as_invocable&& o) noexcept
    : d_r(std::exchange(o.d_r, nullptr))
{
}

template <typename R, typename X>
cxxrt::execution::detail::as_invocable<R, X>::~as_invocable()
{
    if (this->d_r) {
        execution::set_done(std::move(*this->d_r));
    }
}

template <typename R, typename X>
void cxxrt::execution::detail::as_invocable<R, X>::operator()() & noexcept
    try
    {
        execution::set_value(std::move(*this->d_r));
        this->d_r = nullptr;
    }
    catch (...)
    {
        execution::set_error(std::move(*this->d_r), std::current_exception());
        this->d_r = nullptr;
    }

// ----------------------------------------------------------------------------

#endif
