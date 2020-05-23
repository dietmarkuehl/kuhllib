// include/io_context.hpp                                             -*-C++-*-
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

#ifndef INCLUDED_IO_CONTEXT
#define INCLUDED_IO_CONTEXT

#include <executor.hpp>

#include <chrono>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    class io_context;
}

// ----------------------------------------------------------------------------

class cxxrt::net::io_context
    : public cxxrt::net::execution_context
{
public:
    class executor_type;
    using count_type = std::size_t;

    io_context();
    explicit io_context(int concurrency_hint);
    io_context(io_context const&) = delete;
    io_context& operator=(io_context const&) = delete;

    executor_type get_executor() noexcept;

    count_type run();
    count_type run_one();

    template<typename Rep, typename Period>
    count_type run_for(std::chrono::duration<Rep, Period> const& rel_time);
    template<typename Clock, typename Duration>
    count_type run_until(std::chrono::time_point<Clock, Duration> const& abs_time);
    template<typename Rep, typename Period>
    count_type run_one_for(std::chrono::duration<Rep, Period> const& rel_time);
    template<typename Clock, typename Duration>
    count_type run_one_until(std::chrono::time_point<Clock, Duration> const& abs_time);

    count_type poll();
    count_type poll_one();

    void stop();
    bool stopped() const noexcept;
    void restart();
};

// ----------------------------------------------------------------------------

class cxxrt::net::io_context::executor_type
{
private:
    cxxrt::net::io_context* d_context;
    
public:
    explicit executor_type(cxxrt::net::io_context* context)
        : d_context(context)
    {
    }

    executor_type(executor_type const& other) noexcept = default;
    executor_type(executor_type&& other) noexcept = default;
    executor_type& operator=(executor_type const& other) noexcept = default;
    executor_type& operator=(executor_type&& other) noexcept = default;

    bool running_in_this_thread() const noexcept;
    io_context& context() const noexcept { return *this->d_context; }
    void on_work_started() const noexcept;
    void on_work_finished() const noexcept;

    template<typename Func, typename ProtoAllocator>
    void dispatch(Func&& f, ProtoAllocator const& a) const;
    template<typename Func, typename ProtoAllocator>
    void post(Func&& f, ProtoAllocator const& a) const;
    template<typename Func, typename ProtoAllocator>
    void defer(Func&& f, ProtoAllocator const& a) const;

    friend bool operator==(io_context::executor_type const& a,
                           io_context::executor_type const& b) noexcept
    {
        return a.d_context == b.d_context;
    }
};

// ----------------------------------------------------------------------------

inline cxxrt::net::io_context::executor_type
cxxrt::net::io_context::get_executor() noexcept
{
    return executor_type(this);
}

// ----------------------------------------------------------------------------

#endif
