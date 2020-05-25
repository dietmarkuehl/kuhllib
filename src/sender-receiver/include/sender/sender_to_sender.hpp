// include/sender/sender_to_sender.hpp                                -*-C++-*-
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

#ifndef INCLUDED_SENDER_SENDER_TO_SENDER
#define INCLUDED_SENDER_SENDER_TO_SENDER

#include <execution/connect.hpp>
#include <execution/receiver.hpp>
#include <execution/sender.hpp>
#include <execution/sender_base.hpp>
#include <execution/set_value.hpp>
#include <execution/set_error.hpp>
#include <execution/set_done.hpp>

#include <functional>
#include <utility>
#include <type_traits>
#include <iostream> //-dk:TODO

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    template <typename, typename> class sender_to_sender_sender;
    
    template <typename From, typename To>
        requires cxxrt::execution::sender<From>
              && cxxrt::execution::sender<To>
    sender_to_sender_sender<From, To> operator| (From&&, To&&);

    template <typename, typename> class sender_to_incomplete_sender_sender;
    
    template <typename From, typename To>
        requires cxxrt::execution::sender<From>
    sender_to_incomplete_sender_sender<From, To> operator| (From&&, To&&);
}

// ----------------------------------------------------------------------------

template <typename From, typename To>
class cxxrt::execution::sender_to_sender_sender
    : public cxxrt::execution::sender_base
{
private:
    std::remove_cvref_t<From> d_f;
    std::remove_cvref_t<To>   d_t;

public:
    template <typename F, typename T>
    sender_to_sender_sender(F&& f, T&& t)
        : d_f(std::forward<F>(f))
        , d_t(std::forward<T>(t))
    {
    }

    template <typename R>
    struct receiver
    {
        R d_r;
        void start() noexcept;
    };
    template <typename R>
    receiver<R> connect(R&&);
    //return cxxrt::execution::connect(std::move(this->d_f),
    //receiver
};
    
template <typename From, typename To>
requires cxxrt::execution::sender<From>
      && cxxrt::execution::sender<To>
cxxrt::execution::sender_to_sender_sender<From, To>
cxxrt::execution::operator| (From&& f, To&& t)
{
    return { std::forward<From>(f), std::forward<To>(t) };
}

// ----------------------------------------------------------------------------

template <typename From, typename To>
class cxxrt::execution::sender_to_incomplete_sender_sender
    : public cxxrt::execution::sender_base
{
private:
    From d_from;
    To   d_to;

public:
    template <typename F, typename T>
    sender_to_incomplete_sender_sender(F&& f, T&& t)
        : d_from(std::forward<F>(f))
        , d_to(std::forward<T>(t))
    {
    }

    template <typename R>
    struct receiver
    {
        To d_to;
        R  d_r;

        template <typename... A>
        void set_value(A&&... a)
        {
            auto sender = std::invoke(std::move(this->d_to),
                                      std::forward<A>(a)...);
            auto op = cxxrt::execution::connect(std::move(sender),
                                                std::move(this->d_r));
            op.start();
            //-dk:TODO deal with the life-time issue of the operation object
            std::cout << "op, unfortunately, destroyed\n";
        }
        template <typename E>
        void set_error(E&& e) noexcept
        {
            cxxrt::execution::set_error(std::move(this->d_r), std::forward<E>(e));
        }
        void set_done() noexcept
        {
            cxxrt::execution::set_done(std::move(this->d_r));
        }
    };

    template <typename R>
        requires cxxrt::execution::receiver<R>
    auto connect(R&& r)
    {
        return cxxrt::execution::connect(std::move(this->d_from),
                                         receiver<R>{
                                             std::move(this->d_to),
                                                 std::forward<R>(r)
                                                 });
    }
};

// ----------------------------------------------------------------------------

template <typename From, typename To>
    requires cxxrt::execution::sender<From>
cxxrt::execution::sender_to_incomplete_sender_sender<From, To>
cxxrt::execution::operator| (From&& from, To&& to)
{
    return sender_to_incomplete_sender_sender<From, To>(std::forward<From>(from),
                                             std::forward<To>(to));
}

// ----------------------------------------------------------------------------

#endif
