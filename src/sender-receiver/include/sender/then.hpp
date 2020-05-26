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
    //-dk:TODO it may be reasonable to have both a one argument and a two
    //-dk:TODO argument then().
    template <typename> class then;
}

// ----------------------------------------------------------------------------

template <typename Fun>
class cxxrt::execution::then
{
private:
    Fun d_fun;

public:
    template <typename> class receiver;
    template <typename> class sender;

    then(Fun);
    template <typename Sender>
    then<Fun>::sender<std::remove_cvref_t<Sender>> operator()(Sender&&);
};

// ----------------------------------------------------------------------------

template <typename Fun>
template <typename Receiver>
class cxxrt::execution::then<Fun>::receiver
{
private:
    Receiver d_receiver;
    Fun      d_fun;

public:
    template <typename R, typename F>
    receiver(R&& r, F&& f)
        : d_receiver(std::forward<R>(r))
        , d_fun(std::forward<F>(f))
    {
    }
                  
    void set_value() && noexcept = delete; //-dk:TODO why is that needed?
    template <typename... A>
        requires cxxrt::execution::receiver_of<Receiver, std::invoke_result_t<Fun, A...>>
    void set_value(A&&... a) && noexcept try
    {
        cxxrt::execution::set_value(std::move(this->d_receiver),
                                    std::invoke(std::move(this->d_fun), std::forward<A>(a)...));
    }
    catch (...)
    {
        cxxrt::execution::set_error(std::move(this->d_receiver), std::current_exception());
    }
    template <typename... A>
        requires (!cxxrt::execution::receiver_of<Receiver, std::invoke_result_t<Fun, A...>>)
    void set_value(A&&... a) && noexcept try
    {
        std::invoke(std::move(this->d_fun), std::forward<A>(a)...);
        cxxrt::execution::set_value(std::move(this->d_receiver));
    }
    catch (...)
    {
        cxxrt::execution::set_error(std::move(this->d_receiver), std::current_exception());
    }
    template <typename E>
    void set_error(E&& e) && noexcept
    {
        cxxrt::execution::set_error(std::move(this->d_receiver), std::forward<E>(e));
    }
    void set_done() && noexcept
    {
        cxxrt::execution::set_done(std::move(this->d_receiver));
    }
};

// ----------------------------------------------------------------------------

template <typename Fun>
template <typename Sender>
class cxxrt::execution::then<Fun>::sender
    : public cxxrt::execution::sender_base
{
public:
    template <typename> friend class then;
    Sender d_sender;
    Fun    d_fun;

public:
    //-dk:TODO can this be a typed sender declaring value_types, error_types?
    //-dk:TODO the declaration can probably be leveraged from Sender

    template <typename R>
    auto connect(R&& r) &&
    {
        return cxxrt::execution::connect(std::move(this->d_sender),
                                         receiver<R>{ std::forward<R>(r),
                                                      std::move(this->d_fun) });
    }
};

// ----------------------------------------------------------------------------

template <typename Fun>
cxxrt::execution::then<Fun>::then(Fun fun)
    : d_fun(fun)
{
}

template <typename Fun>
    template <typename Sender>
typename cxxrt::execution::then<Fun>::template sender<std::remove_cvref_t<Sender>>
cxxrt::execution::then<Fun>::operator()(Sender&& sender)
{
    return { {}, std::forward<Sender>(sender), std::move(this->d_fun) };
}

// ----------------------------------------------------------------------------

#endif
