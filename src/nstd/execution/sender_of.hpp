// nstd/execution/sender_of.hpp                                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_EXECUTION_SENDER_OF
#define INCLUDED_NSTD_EXECUTION_SENDER_OF

#include "nstd/execution/no_env.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/value_types_of_t.hpp"
#include "nstd/hidden_names/type_list.hpp"
#include "nstd/type_traits/type_identity.hpp"
#include <concepts>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    template <typename Sender, typename Environment = nstd::hidden_names::exec_envs::no_env, typename...T> 
    concept sender_of
        =  ::nstd::execution::sender<Sender, Environment>
        && ::std::same_as<::nstd::hidden_names::type_list<T...>,
                          ::nstd::execution::value_types_of_t<Sender,
                                                              Environment,
                                                              ::nstd::hidden_names::type_list,
                                                              ::nstd::type_traits::type_identity_t>
                         >
        ;
}

// ----------------------------------------------------------------------------

#endif
