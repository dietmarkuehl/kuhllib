// nstd/executor/system_executor.hpp                                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTOR_SYSTEM_EXECUTOR
#define INCLUDED_NSTD_EXECUTOR_SYSTEM_EXECUTOR

// ----------------------------------------------------------------------------

namespace nstd::net {
    class system_executor;
    class system_context;

    //-dk:TODO make hidden friend
    bool operator== (::nstd::net::system_executor const&,
                     ::nstd::net::system_executor const&);
}

class nstd::net::system_executor
{
public: 
    system_executor() = default;

    auto context() const noexcept -> ::nstd::net::system_context&;

    auto on_work_started() const noexcept ->void;
    auto on_work_finished() const noexcept ->void;

    template <typename Fun, typename ProtoAllocator>
    auto dispatch(Fun&&, ProtoAllocator const&) const -> void;
    template <typename Fun, typename ProtoAllocator>
    auto post(Fun&&, ProtoAllocator const&) const -> void;
    template <typename Fun, typename ProtoAllocator>
    auto defer(Fun&&, ProtoAllocator const&) const -> void;
};

// ----------------------------------------------------------------------------

#endif
