// nstd/stop_token/stop_callback_base.hpp                             -*-C++-*-
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

#ifndef INCLUDED_NSTD_STOP_TOKEN_STOP_CALLBACK_BASE
#define INCLUDED_NSTD_STOP_TOKEN_STOP_CALLBACK_BASE

#include "nstd/container/intrusive_list.hpp"

// ----------------------------------------------------------------------------

namespace nstd::stop_token_ns::hidden_names {
    class stop_callback_base;
}

// ----------------------------------------------------------------------------

class nstd::stop_token_ns::hidden_names::stop_callback_base
{
protected:
    virtual auto do_call() noexcept -> void {}

public:
    ::nstd::container::intrusive_list_node<stop_callback_base> link;
    auto call() -> void { this->do_call(); }
};

// ----------------------------------------------------------------------------

#endif
