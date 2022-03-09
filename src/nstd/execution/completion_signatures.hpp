// src/nstd/execution/completion_signatures.hpp                       -*-C++-*-
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

#ifndef INCLUDED_SRC_NSTD_EXECUTION_COMPLETION_SIGNATURES
#define INCLUDED_SRC_NSTD_EXECUTION_COMPLETION_SIGNATURES

#include "nstd/hidden_names/completion_signature.hpp"
#include <concepts>

// ----------------------------------------------------------------------------
// [exec.utils.cmplsigs]

namespace nstd::execution {
    template <::nstd::hidden_names::completion_signature ... Signatures>
    struct completion_signatures {
        static constexpr bool sends_stopped = (false || ... || ::std::same_as<Signatures, ::nstd::execution::set_stopped_t()>);
    };
}

// ----------------------------------------------------------------------------

#endif
