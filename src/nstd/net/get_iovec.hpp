// nstd/net/get_iovec.hpp                                             -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2023 Dietmar Kuehl http://www.dietmar-kuehl.de
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

#ifndef INCLUDED_NSTD_NET_GET_IOVEC
#define INCLUDED_NSTD_NET_GET_IOVEC

#include "nstd/buffer/const_buffer.hpp"
#include "nstd/buffer/mutable_buffer.hpp"
#include "nstd/buffer/sequence.hpp"
#include "nstd/functional/tag_invoke.hpp"
#include <algorithm>
#include <iterator>
#include <array>
#include <utility>
#include <vector>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names::get_iovec {
    struct vector_iovec {
        ::std::vector<::iovec> d_seq;

        vector_iovec(vector_iovec&&) = delete;
        template <typename BufferSequence>
        vector_iovec(BufferSequence const& bseq) {
            ::std::transform(buffer_sequence_begin(bseq),
                             buffer_sequence_end(bseq),
                             ::std::back_inserter(this->d_seq),
                             [](auto&& buffer){
                                ::iovec rc;
                                rc.iov_base = const_cast<void*>(buffer.data());
                                rc.iov_len  = buffer.size();
                                return rc;
                             });
        };
        auto data() noexcept -> ::iovec* { return this->d_seq.data(); }
        auto size() noexcept -> int      { return this->d_seq.size(); }
    };

    template <::std::size_t N, typename = ::std::make_index_sequence<N>>
    struct array_iovec;
    template <::std::size_t N, ::std::size_t... I>
    struct array_iovec<N, ::std::index_sequence<I...>> {
        using transform = decltype(
            [](auto&& buffer){
                ::iovec rc;
                rc.iov_base = const_cast<void*>(buffer.data());
                rc.iov_len  = buffer.size();
                return rc;
            });
        ::std::array<::iovec, N> d_seq;

        array_iovec(array_iovec&&) = delete;
        template <typename BufferSequence>
        array_iovec(BufferSequence const& bseq)
            : d_seq{transform()(buffer_sequence_begin(bseq)[I])...}
        {
        }
        auto data() noexcept -> ::iovec* { return this->d_seq.data(); }
        auto size() noexcept -> int      { return this->d_seq.size(); }
    };

    struct cpo {
        template <typename Env, typename BufferSequence>
        friend auto tag_invoke(cpo, Env const&, BufferSequence const& bseq) {
            return vector_iovec(bseq);
        }
        template <typename Env>
        friend auto tag_invoke(cpo, Env const&, ::nstd::net::mutable_buffer const& bseq) {
            return array_iovec<1u>(bseq);
        }
        template <typename Env>
        friend auto tag_invoke(cpo, Env const&, ::nstd::net::const_buffer const& bseq) {
            return array_iovec<1u>(bseq);
        }
        template <typename Env, ::std::size_t N>
        friend auto tag_invoke(cpo, Env const&, ::nstd::net::mutable_buffer const (&bseq)[N]) {
            return array_iovec<N>(bseq);
        }
        template <typename Env, ::std::size_t N>
        friend auto tag_invoke(cpo, Env const&, ::nstd::net::const_buffer const (&bseq)[N]) {
            return array_iovec<N>(bseq);
        }
        template <typename Env, ::std::size_t N>
        friend auto tag_invoke(cpo, Env const&, ::std::array<::nstd::net::mutable_buffer, N> const& bseq) {
            return array_iovec<N>(bseq);
        }
        template <typename Env, ::std::size_t N>
        friend auto tag_invoke(cpo, Env const&, ::std::array<::nstd::net::const_buffer, N> const& bseq) {
            return array_iovec<N>(bseq);
        }

        template <typename Env, typename BufferSequence>
        auto operator()(Env const& env, BufferSequence const& bseq) const {
            return ::nstd::tag_invoke(*this, env, bseq);
        }
    };
}

namespace nstd::net {
    using get_iovec_t = ::nstd::hidden_names::get_iovec::cpo;
    inline constexpr get_iovec_t get_iovec{};
}


// ----------------------------------------------------------------------------

#endif
