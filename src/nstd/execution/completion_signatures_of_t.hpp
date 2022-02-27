// nstd/execution/completion_signatures_of_t.hpp                      -*-C++-*-
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

#ifndef INCLUDED_NSTD_EXECUTION_COMPLETION_SIGNATURES_OF
#define INCLUDED_NSTD_EXECUTION_COMPLETION_SIGNATURES_OF

#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/type_traits/declval.hpp"
#include "nstd/hidden_names/no_completion_signatures.hpp"
#include <concepts>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    template <typename Sender, typename Env>
        requires (::nstd::execution::sender<Sender, Env>
                  && not ::std::same_as<decltype(
                                           ::nstd::execution::get_completion_signatures(::nstd::type_traits::declval<Sender>(),
                                                                                        ::nstd::type_traits::declval<Env>())
                                        )
                                        , ::nstd::hidden_names::no_completion_signatures::type
                                        >
        )
    using completion_signatures_of_t
        = decltype(
            ::nstd::execution::get_completion_signatures(::nstd::type_traits::declval<Sender>(),
                                                         ::nstd::type_traits::declval<Env>())
          );
}

// ----------------------------------------------------------------------------

#endif
