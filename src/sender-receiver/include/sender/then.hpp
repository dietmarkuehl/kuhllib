// include/sender/then.hpp                                            -*-C++-*-
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

#ifndef INCLUDED_SENDER_THEN
#define INCLUDED_SENDER_THEN

#include <execution/sender_base.hpp>
#include <execution/set_value.hpp>
#include <execution/set_error.hpp>
#include <execution/set_done.hpp>
#include <execution/connect.hpp>
#include <execution/receiver_of.hpp>

#include <functional>
#include <exception>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    template <typename Sender, typename Fun> class then_sender;
    template <typename Fun> class then;
}

// ----------------------------------------------------------------------------

template <typename Sender, typename Fun>
class cxxrt::execution::then_sender
    : public cxxrt::execution::sender_base
{
private:
    Sender d_s;
    Fun    d_f;

public:
    template <typename R>
    class receiver
    {
    private:
        R   d_r;
        Fun d_f;

    public:
        template <typename T>
        receiver(T r, Fun&& f): d_r(std::move(r)), d_f(f) {}

        template <typename... A>
            requires cxxrt::execution::receiver_of<R&&, std::invoke_result_t<Fun, A...>>
        void set_value(A&&... a) try
        {
            cxxrt::execution::set_value(std::move(this->d_r),
                                        std::invoke(std::move(this->d_f), std::forward<A>(a)...));
        }
        catch (...)
        {
            cxxrt::execution::set_error(std::move(this->d_r), std::current_exception());
            
        }
        template <typename... A>
            requires (!cxxrt::execution::receiver_of<R, std::invoke_result_t<Fun, A...>>)
        void set_value(A&&... a) try
        {
            std::invoke(std::move(this->d_f), std::forward<A>(a)...);
            cxxrt::execution::set_value(std::move(this->d_r));
        }
        catch (...)
        {
            cxxrt::execution::set_error(std::move(this->d_r), std::current_exception());
            
        }
        template <typename E>
        void set_error(E&& e) noexcept
        {
            cxxrt::execution::set_error(std::move(this->d_r), std::forward<E>(e));
        }
        void set_done() noexcept { cxxrt::execution::set_done(std::move(this->d_r)); }
    };

    then_sender(Sender s, Fun f)
        : d_s(std::move(s))
        , d_f(std::move(f))
    {
    }

    template <typename R>
    auto connect(R&& r) {
        return cxxrt::execution::connect(std::move(this->d_s),
                                         receiver<R>(std::forward<R>(r),
                                                     std::move(this->d_f)));
    }
};

// ----------------------------------------------------------------------------

template <typename F>
class cxxrt::execution::then
{
private:
    F d_f;

public:
    then(F f): d_f(std::forward<F>(f)) {}

    template <typename Sender>
    friend then_sender<Sender, F> operator| (Sender s, then c)
    {
        return { std::move(s), std::move(c.d_f) };
    }
};

// ----------------------------------------------------------------------------

#endif
