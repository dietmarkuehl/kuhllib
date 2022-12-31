// nstd/net/async_write.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_NET_ASYNC_WRITE
#define INCLUDED_NSTD_NET_ASYNC_WRITE

#include "nstd/net/async_write_some.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/let.hpp"
#include "nstd/execution/repeat_effect_until.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/utility/forward.hpp"

// ----------------------------------------------------------------------------

namespace nstd::net::hidden_names::async_write {
    struct cpo {
        template <typename StreamSocket, typename CBS>
        friend auto tag_invoke(cpo const&, StreamSocket& socket, CBS const& cbs) {
            return ::nstd::execution::let_value(::nstd::execution::just(),
                [&socket, buffer = cbs]() mutable {
                    return ::nstd::execution::repeat_effect_until(
                        ::nstd::execution::let_value(::nstd::execution::just(),
                            [&socket, &buffer]{
                                return ::nstd::net::async_write_some(socket, buffer)
                                    |  ::nstd::execution::then([&buffer](::std::size_t n){
                                        buffer += n;
                                    });
                            }),
                        [&buffer]{ return buffer.size() == 0; }
                    );
                });
        }
        template <typename StreamSocket, typename CBS>
        auto operator()(StreamSocket& socket, CBS const& cbs) const {
            return tag_invoke(*this, socket, cbs);
        }
    };
}

namespace nstd::net {
    using async_write_t = ::nstd::net::hidden_names::async_write::cpo;
    inline constexpr ::nstd::net::async_write_t async_write{};
}

// ----------------------------------------------------------------------------

#endif
