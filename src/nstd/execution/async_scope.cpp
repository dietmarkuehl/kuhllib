// nstd/execution/async_scope.cpp                                     -*-C++-*-
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

#include <nstd/execution/async_scope.hpp>
#include <exception>
#include <cassert>

// ----------------------------------------------------------------------------

namespace nstd::execution {
    int async_scope_dummy{};
}

// ----------------------------------------------------------------------------

auto nstd::hidden_names::async_scope::upon_empty_sender::state_base::insert() -> void
{
    if (not this->scope->insert(this)) {
        this->complete();
    }
}

// ----------------------------------------------------------------------------

nstd::execution::async_scope::~async_scope()
{
    assert(this->d_on_empty.empty());
    if (this->d_outstanding != 0u) {
        ::std::terminate();
    }
}

// ----------------------------------------------------------------------------

auto nstd::execution::async_scope::complete() -> void
{
    ::nstd::container::intrusive_list<upon_empty_state_base> list;
    {
        ::std::lock_guard kerberos(this->d_mutex);
        this->d_on_empty.swap(list);
    }
    while (!list.empty()) {
        auto* next = &list.front();
        list.pop_front();
        next->complete();
    }
}
auto nstd::execution::async_scope::insert(::nstd::hidden_names::async_scope::upon_empty_sender::state_base* s) -> bool {
    {
        ::std::lock_guard kerberos(this->d_mutex);
        ++this->d_outstanding;
        this->d_on_empty.push_back(*s);
        if (0u != --this->d_outstanding) {
            return true;
        }
        this->d_on_empty.erase(this->d_on_empty.make_iterator(s));
    }
    this->complete();
    return false;
}

auto nstd::execution::async_scope::erase(::nstd::hidden_names::async_scope::upon_empty_sender::state_base* s) -> void {
    ::std::lock_guard kerberos(this->d_mutex);
    this->d_on_empty.erase(this->d_on_empty.make_iterator(s));
}