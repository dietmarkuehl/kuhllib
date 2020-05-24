// include/timer.hpp                                                  -*-C++-*-
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

#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

#include <io_context.hpp>

#include <chrono>
#include <deque>
#include <exception>
#include <memory>
#include <system_error>

// ----------------------------------------------------------------------------

namespace cxxrt::net
{
    template<typename> struct wait_traits;
    template<typename Clock, typename = wait_traits<Clock>>

    class basic_waitable_timer;

    using system_timer = basic_waitable_timer<std::chrono::system_clock>;
    using steady_timer = basic_waitable_timer<std::chrono::steady_clock>;
    using high_resolution_timer = basic_waitable_timer<std::chrono::high_resolution_clock>; 
}

// ----------------------------------------------------------------------------

template<typename Clock>
struct cxxrt::net::wait_traits
{
    using duration   = typename Clock::duration;
    using time_point = typename Clock::time_point;
    
    static auto to_wait_duration(duration const& d) -> duration;
    static auto to_wait_duration(time_point const& t) -> duration;
};

// ----------------------------------------------------------------------------

template<typename Clock, typename WaitTraits>
class cxxrt::net::basic_waitable_timer
{
public:
    using executor_type = io_context::executor_type;
    using clock_type    = Clock;
    using duration      = typename clock_type::duration;
    using time_point    = typename clock_type::time_point;
    using traits_type   = WaitTraits;
    using timer_base    = io_context::timer_base;
    struct sender;

private:
    cxxrt::net::io_context* d_context;
    time_point              d_expiry;
    // std::deque<timer_base*> d_waiting; //-dk:TODO use an intrusive list?

    void add(timer_base* s)
    {
        // this->d_waiting.push_back(s);
        this->d_context->add(this->d_expiry, s);
    }

public:
    explicit basic_waitable_timer(io_context& c);
    basic_waitable_timer(io_context& c, time_point const& t);
    basic_waitable_timer(io_context& c, duration const& d);
    basic_waitable_timer(basic_waitable_timer const&) = delete;
    basic_waitable_timer(basic_waitable_timer&& rhs) = delete; //-dk:TODO
    ~basic_waitable_timer() = default;

    basic_waitable_timer& operator=(basic_waitable_timer const&) = delete;
    basic_waitable_timer& operator=(basic_waitable_timer&& rhs) = delete; //-dk:TODO

    executor_type get_executor() noexcept;
    std::size_t cancel();
    std::size_t cancel_one();
    time_point  expiry() const;
    std::size_t expires_at(time_point const& t);
    std::size_t expires_after(duration const& d);

    sender sender_wait();
    
    void wait();
    void wait(std::error_code& ec);
    //-dk:TODO template<typename CompletionToken> DEDUCED async_wait(CompletionToken&& token);
};

// ----------------------------------------------------------------------------

template<typename C, typename W>
struct cxxrt::net::basic_waitable_timer<C, W>::sender
{
public:
    template <template <typename...> class, template <typename...> class V>
    using value_types = V<>;
    template <template <typename...> class V>
    using error_types = V<std::exception_ptr>;

    static constexpr bool sends_done = true;

    template <typename R>
    struct state
        : cxxrt::net::io_context::timer_base
    {
    private:
        basic_waitable_timer* d_timer;
        R                     d_r;

    public:
        template <typename S>
        state(basic_waitable_timer* t, S&& r)
            : d_timer(t)
            , d_r(std::forward<S>(r))
        {
        }
        void notify() override try
        {
            std::move(this->d_r).set_value();
        }
        catch (...)
        {
            std::move(this->d_r).set_error(std::current_exception());
        }
        void cancel() override { std::move(this->d_r).set_done(); }
        void start() noexcept try
        {
            this->d_timer->add(this);
        }
        catch (...)
        {
            std::move(this->d_r).set_error(std::current_exception());
        }
    };

private:
    basic_waitable_timer* d_timer;
    
public:
    explicit sender(basic_waitable_timer* t): d_timer(t) {}

    template <typename R>
    auto connect(R&& r) { return state<R> { this->d_timer, std::move(r) }; }
};

// ----------------------------------------------------------------------------

template<typename Clock>
inline auto
cxxrt::net::wait_traits<Clock>::to_wait_duration(duration const& d)
    -> duration
{
    return d;
}
template<typename Clock>
inline auto
cxxrt::net::wait_traits<Clock>::to_wait_duration(time_point const& t)
    -> duration
{
    //-dk:TODO deal with duration boundary conditions
    auto now = Clock::now();
    return t - now;
}

// ----------------------------------------------------------------------------

template<typename C, typename W>
cxxrt::net::basic_waitable_timer<C, W>::basic_waitable_timer(io_context& c)
    : basic_waitable_timer(c, time_point{})
{
}

template<typename C, typename W>
cxxrt::net::basic_waitable_timer<C, W>::basic_waitable_timer(io_context&       c,
                                                             time_point const& t)
    : d_context(std::addressof(c))
    , d_expiry(t)
{
}

template<typename C, typename W>
cxxrt::net::basic_waitable_timer<C, W>::basic_waitable_timer(io_context&     c,
                                                             duration const& d)
    : basic_waitable_timer(c, clock_type::now() + d)
{
}

// ----------------------------------------------------------------------------

template<typename C, typename W>
auto cxxrt::net::basic_waitable_timer<C, W>::expiry() const
    -> time_point
{
    return this->d_expiry;
}

template<typename C, typename W>
auto cxxrt::net::basic_waitable_timer<C, W>::cancel_one()
    -> std::size_t
{
    //-dk:TODO cancel waiting operations
    return 0u;
}

template<typename C, typename W>
auto cxxrt::net::basic_waitable_timer<C, W>::cancel()
    -> std::size_t
{
    std::size_t n{};
    while (auto c = this->cancel_one())
    {
        n += c;
    }
    return n;
}

template<typename C, typename W>
auto cxxrt::net::basic_waitable_timer<C, W>::expires_at(time_point const& t)
    -> std::size_t
{
    std::size_t n{this->cancel()};
    this->d_expiry = t;
    return n;
}

template<typename C, typename W>
auto cxxrt::net::basic_waitable_timer<C, W>::expires_after(duration const& d)
    -> std::size_t
{
    return this->expires_at(clock_type::now() + d);
}

// ----------------------------------------------------------------------------

template<typename C, typename W>
auto cxxrt::net::basic_waitable_timer<C, W>::sender_wait()
    -> sender
{
    return sender(this);
}


// ----------------------------------------------------------------------------

#endif
