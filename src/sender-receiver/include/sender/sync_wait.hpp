// include/sender/sync_wait.hpp                                       -*-C++-*-
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

#ifndef INCLUDED_SENDER_SYNC_WAIT
#define INCLUDED_SENDER_SYNC_WAIT

#include <execution/connect.hpp>
#include <atomic>
#include <iostream>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    namespace detail { struct receiver; }
    template <typename Sender, typename Context>
    void sync_wait(Sender&&, Context&);
}
    
// ----------------------------------------------------------------------------

struct cxxrt::execution::detail::receiver
{
    std::atomic<bool>& d_done;
    
    template <typename... A>
    void set_value(A&&...) noexcept
    {
        std::cout << "async_wait: success\n";
        this->d_done = true;
    }
    template <typename E>
    void set_error(E&&) noexcept
    {
        std::cout << "async_wait: error\n";
        this->d_done = true;
    }
    void set_done() noexcept
    {
        std::cout << "async_wait: canceled\n";
        this->d_done = true;
    }
};

// ----------------------------------------------------------------------------

template <typename Sender, typename Context>
void cxxrt::execution::sync_wait(Sender&& s, Context& context)
{
    std::atomic<bool> done(false);
    auto operation = cxxrt::execution::connect(std::forward<Sender>(s),
                                               detail::receiver{done});
    operation.start();
    while (!done)
    {
        context.run_one();
    }
}

// ----------------------------------------------------------------------------

#endif
