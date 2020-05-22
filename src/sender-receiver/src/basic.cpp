// basic.cpp                                                          -*-C++-*-
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

#include "execution.hpp"
#include <iostream>
#include <type_traits>
#include <utility>

namespace EX = cxxrt::execution;

// ----------------------------------------------------------------------------

namespace
{
    struct receiver
    {
        void set_value(int) {}
        void set_done() & {}
        void set_error(int) {}
        //void set_error(std::exception_ptr const&) {}
    };
    bool set_value(receiver, bool b, bool) { return b; }
    bool set_done(receiver&&) { return true; }
    bool set_error(receiver, std::exception_ptr const&) { return true; }

    struct sender_
    {
        template <typename R>
        struct state
        {
            R d_receiver;
            void start() noexcept { EX::set_done(this->d_receiver); }
        };
        template <typename R>
        state<std::remove_cvref_t<R>> connect(R&& r) { return { std::forward<R>(r) }; }
    };

    struct sender
        : EX::sender_base
    {
        template <typename R>
        struct state
        {
            R d_receiver;
            void start() noexcept { EX::set_done(this->d_receiver); }
        };
        template <typename R>
        state<std::remove_cvref_t<R>> connect(R&& r) { return { std::forward<R>(r) }; }
    };

    struct typed_sender
    {
        template <template <typename...> class V,
                  template <typename...> class T>
        using value_types = V<T<int>, T<int, bool>>;
        template <template <typename...> class V>
        using error_types = V<std::exception_ptr, int>;

        static constexpr bool sends_done = false;

        template <typename R>
        struct state
        {
            R d_receiver;
            void start() noexcept { EX::set_done(this->d_receiver); }
        };
        template <typename R>
        state<std::remove_cvref_t<R>> connect(R&& r) { return { std::forward<R>(r) }; }
    };
}

namespace cxxrt::execution
{
    template <>
    struct sender_traits<sender_>
    {
    };
}

// ----------------------------------------------------------------------------

int main()
{
    receiver r;
    r.set_value(0);
    set_value(r, true, false);
    
    //EX::set_value(r);
    EX::set_value(r, 0);
    bool b = EX::set_value(r, true, false);

    receiver const rc;
    EX::set_done(r);
    EX::set_done(receiver());
    //EX::set_done(std::move(rc));

    EX::set_error(r, 1);
    EX::set_error(r, std::exception_ptr());

    {
        auto o = EX::connect(sender_(), receiver());
        EX::start(o);
    }

    {
        auto o = EX::connect(sender(), receiver());
        EX::start(o);
    }

    {
        auto o = EX::connect(typed_sender(), receiver());
        EX::start(o);
    }
}
