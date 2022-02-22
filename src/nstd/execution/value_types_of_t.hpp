// src/nstd/execution/value_types_of_t.hpp                            -*-C++-*-
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

#ifndef INCLUDED_SRC_NSTD_EXECUTION_VALUE_TYPES_OF
#define INCLUDED_SRC_NSTD_EXECUTION_VALUE_TYPES_OF

#include "nstd/execution/no_env.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/hidden_names/decayed_tuple.hpp"
#include "nstd/hidden_names/variant_or_empty.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    template <typename Sender,
              typename Env = ::nstd::hidden_names::exec_envs::no_env,
              template <typename...> class Tuple = ::nstd::hidden_names::decayed_tuple,
              template <typename...> class Variant = ::nstd::hidden_names::variant_or_empty>
        requires ::nstd::execution::sender<Sender, Env>
    using value_types_of_t = void; //-dk:TODO
}

// ----------------------------------------------------------------------------

#endif
