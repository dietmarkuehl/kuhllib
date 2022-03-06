// src/nstd/hidden_names/valid_completion_signatures.hpp              -*-C++-*-
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

#ifndef INCLUDED_SRC_NSTD_HIDDEN_NAMES_VALID_COMPLETION_SIGNATURES
#define INCLUDED_SRC_NSTD_HIDDEN_NAMES_VALID_COMPLETION_SIGNATURES

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/dependent_completion_signatures.hpp"
#include "nstd/execution/no_env.hpp"
#include "nstd/hidden_names/is_instance_of.hpp"
#include <concepts>

// ----------------------------------------------------------------------------
// [exec.snd]

namespace nstd::hidden_names {
    template <typename Signatures, typename Env>
    concept valid_completion_signatures
        =  ::nstd::hidden_names::is_instance_of<Signatures, ::nstd::execution::completion_signatures>
        || (::std::same_as<Signatures, ::nstd::execution::dependent_completion_signatures<::nstd::hidden_names::exec_envs::no_env>>
            && ::std::same_as<Env, ::nstd::hidden_names::exec_envs::no_env>
            )
        ;
}

// ----------------------------------------------------------------------------

#endif
