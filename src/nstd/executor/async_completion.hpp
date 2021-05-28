// nstd/executor/async_completion.hpp                                 -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTOR_ASYNC_COMPLETION
#define INCLUDED_NSTD_EXECUTOR_ASYNC_COMPLETION

#include "nstd/executor/async_result.hpp"
#include <type_traits>

// ----------------------------------------------------------------------------

namespace nstd::net {
    template <typename CompletionToken, typename Signature>
    class async_completion;
}

// ----------------------------------------------------------------------------

template <typename CompletionToken, typename Signature>
class nstd::net::async_completion
{
public:
    using completion_handler_type = ::nstd::net::async_result<::std::decay_t<CompletionToken>, Signature>::completion_handler_type;

    explicit async_completion(CompletionToken&);
    async_completion(async_completion const&) = delete;
    auto operator=(async_completion const&) ->async_completion& = delete;

    ::std::conditional_t<::std::is_same_v<CompletionToken, completion_handler_type>,
                         completion_handler_type&,
                         completion_handler_type>                         completion_handler;
    ::nstd::net::async_result<::std::decay_t<CompletionToken>, Signature> result;
};

// ----------------------------------------------------------------------------

template <typename Token, typename Signature>
nstd::net::async_completion<Token, Signature>::async_completion(Token& token)
    : completion_handler(token)
    , result(completion_handler)
{
}

// ----------------------------------------------------------------------------

#endif
