// include/execution.hpp                                              -*-C++-*-
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

#ifndef INCLUDED_CXXRT_EXECUTION
#define INCLUDED_CXXRT_EXECUTION

#include <execution/bulk_execute.hpp>
#include <execution/connect.hpp>
#include <execution/execute.hpp>
#include <execution/executor.hpp>
#include <execution/executor_of.hpp>
#include <execution/operation_state.hpp>
#include <execution/receiver.hpp>
#include <execution/receiver_of.hpp>
#include <execution/schedule.hpp>
#include <execution/scheduler.hpp>
#include <execution/sender.hpp>
#include <execution/sender_to.hpp>
#include <execution/sender_traits.hpp>
#include <execution/set_done.hpp>
#include <execution/set_error.hpp>
#include <execution/set_value.hpp>
#include <execution/start.hpp>
#include <execution/submit.hpp>
#include <execution/typed_sender.hpp>

#include <stdexcept>
#include <utility>

// ----------------------------------------------------------------------------

namespace cxxrt::execution
{
    struct receiver_invocation_error;

    struct context_t {};
    constexpr context_t context;

    struct blocking_t {};
    constexpr blocking_t blocking;

    struct blocking_adaptation_t {};
    constexpr blocking_adaptation_t blocking_adaptation;

    struct relationship_t {};
    constexpr relationship_t relationship;

    struct outstanding_work_t {};
    constexpr outstanding_work_t outstanding_work;

    struct bulk_guarantee_t {};
    constexpr bulk_guarantee_t bulk_guarantee;

    struct mapping_t {};
    constexpr mapping_t mapping;

    template <typename ProtoAllocator>
    struct allocator_t {};
    constexpr allocator_t<void> allocator;

    template<typename Executor> struct executor_shape;
    template<typename Executor> struct executor_index;

    template<typename Executor> using executor_shape_t = typename executor_shape<Executor>::type;
    template<typename Executor> using executor_index_t = typename executor_index<Executor>::type;

    class bad_executor;
    
    template <typename... SupportableProperties> class any_executor;

    template<typename Property> struct prefer_only;

} // namespace cxxrt::execution

// ----------------------------------------------------------------------------

struct cxxrt::execution::receiver_invocation_error
    : std::runtime_error
    , std::nested_exception
{
    receiver_invocation_error() noexcept
        : std::runtime_error("invocation error")
        , std::nested_exception()
    {
    }
};

// ----------------------------------------------------------------------------


#endif
