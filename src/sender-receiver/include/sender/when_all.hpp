// sender/when_all.hpp                                                -*-C++-*-
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

#ifndef INCLUDED_SENDER_WHEN_ALL
#define INCLUDED_SENDER_WHEN_ALL

#include <execution/sender_base.hpp>
#include <execution/set_value.hpp>
#include <execution/set_error.hpp>
#include <execution/set_done.hpp>

#include <tuple>
#include <type_traits>
#include <utility>
#include <iostream> //-dk:TODO remove

// ----------------------------------------------------------------------------

namespace cxxrt::execution {
    template <typename... > class when_all;
}

// ----------------------------------------------------------------------------

template <typename... Sender>
class cxxrt::execution::when_all
    : public cxxrt::execution::sender_base
{
private:
    using Senders = std::tuple<std::remove_cvref_t<Sender>...>;
    Senders d_sender;

public:
    explicit when_all(Sender&&... sender)
        : d_sender(std::forward<Sender>(sender)...)
    {
    }

    template <typename R>
    class state {
    private:
        struct receiver {
            state* d_s;
            void set_value() && { this->d_s->report(); }
            template <typename... T>
            void set_value(T&&...) && { this->d_s->report(); }
            template <typename T>
            void set_error(T&&) && {
                //-dk:TODO cancel other senders
                this->d_s->d_error = true;
                this->d_s->report();
            }
            void set_done() && {
                //-dk:TODO cancel other senders
                this->d_s->d_abort = true;
                this->d_s->report();
            }
        };

        //-dk:TODO produce aggregated result?
        template <typename S>
        using Op = decltype(cxxrt::execution::connect(std::declval<std::remove_cvref_t<S>>(),
                                                      std::declval<receiver>()));
        using Ops = std::tuple<Op<Sender>...>;
        std::size_t            d_count{};
        bool                   d_error{};
        bool                   d_abort{};
        Ops                    d_ops;
        std::remove_cvref_t<R> d_receiver;

        void report() {
            if (++this->d_count == sizeof...(Sender)) {
                if (this->d_abort) {
                    cxxrt::execution::set_done(std::move(this->d_receiver));
                }
                else if (this->d_error) {
                    cxxrt::execution::set_error(std::move(this->d_receiver), 0);
                }
                else {
                    cxxrt::execution::set_value(std::move(this->d_receiver));
                }
            }
        }
    public:
        template <typename S>
        state(S&& senders, R r)
            : d_ops(std::apply([this](auto&&...s){
                                   return std::tuple(cxxrt::execution::connect(std::move(s), receiver{this})...);
                               },
                               std::move(senders)))
            , d_receiver(std::forward<R>(r))
        {
        }
        void start() noexcept {
            std::apply([](auto&... o){ std::initializer_list<bool>{ (o.start(), true)... }; },
                       this->d_ops);
        }
    };
    template <typename R>
    state<R> connect(R&& r) {
        return state<R>(std::move(this->d_sender), std::forward<R>(r));
    }
};

// ----------------------------------------------------------------------------

#endif
