// nstd/net/scope.cpp                                                 -*-C++-*-
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

#include <nstd/net/scope.hpp>
#include <iostream>
#include <system_error>
#include <cstring>
#include <string.h>

// ----------------------------------------------------------------------------

namespace nstd::net {
    int scope_dummy{};
}

// ----------------------------------------------------------------------------

auto nstd::net::scope::stop() -> void
{
    this->d_stopped = true;
    this->d_stop_source.stop();
}

auto nstd::net::scope::run_one() -> ::std::size_t
{
    return this->d_context.run_one();
}

auto nstd::net::scope::run() -> ::std::size_t
{
    return this->d_context.run();
}

// ----------------------------------------------------------------------------

auto nstd::net::scope::receiver::handle_set_error(std::exception_ptr error) -> void
{
    try { ::std::rethrow_exception(error); }
    catch (std::system_error const& ex) {
#ifndef _MSC_VER
        auto error_str{std::strerror(ex.code().value())};
#else
        char error_str[1024];
        strerror_s(error_str, sizeof(error_str), ex.code().value());
#endif
        ::std::cerr << "scope::set_error(exception_ptr): " << error_str << "\n";
    }
    catch (std::exception const& ex) {
        ::std::cerr << "scope::set_error(exception_ptr): " << ex.what() << "\n";
    }
}

auto nstd::net::scope::receiver::handle_set_error(std::error_code error) -> void
{
#ifndef _MSC_VER
        auto error_str{std::strerror(error.value())};
#else
        char error_str[1024];
        strerror_s(error_str, sizeof(error_str), error.value());
#endif
    ::std::cout << "scope::set_error(error_code): " << error_str << "/" << error << "\n";
}

auto nstd::net::scope::receiver::handle_set_stopped() -> void
{
    ::std::cout << "scope::set_stopped()\n";
}