// nstd/stop_token.hpp                                                -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN
#define INCLUDED_NSTD_STOP_TOKEN

// ----------------------------------------------------------------------------
// [thread.stoptoken.syn]

#include "nstd/stop_token/nostopstate.hpp"
#include "nstd/stop_token/stop_callback.hpp"
#include "nstd/stop_token/stop_source.hpp"
#include "nstd/stop_token/stop_token.hpp"
#include "nstd/stop_token/stoppable_token.hpp"
#include "nstd/stop_token/stoppable_token_for.hpp"
#include "nstd/stop_token/unstoppable_token.hpp"
#include "nstd/stop_token/never_stop_token.hpp"
#include "nstd/stop_token/in_place_stop_token.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    using ::nstd::stop_token_ns::nostopstate;
    using ::nstd::stop_token_ns::nostopstate_t;
    using ::nstd::stop_token_ns::stop_callback;
    using ::nstd::stop_token_ns::stop_source;
    using ::nstd::stop_token_ns::stop_token;
    using ::nstd::stop_token_ns::stoppable_token;
    using ::nstd::stop_token_ns::stoppable_token_for;
    using ::nstd::stop_token_ns::unstoppable_token;
    using ::nstd::stop_token_ns::never_stop_token;
    using ::nstd::stop_token_ns::in_place_stop_token;
    using ::nstd::stop_token_ns::in_place_stop_source;
    using ::nstd::stop_token_ns::in_place_stop_callback;
}

// ----------------------------------------------------------------------------

#endif
