// nstd/execution/just_done.hpp                                       -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2021 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#ifndef INCLUDED_NSTD_EXECUTION_JUST_DONE
#define INCLUDED_NSTD_EXECUTION_JUST_DONE

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/connect.hpp"
#include "nstd/execution/receiver.hpp"
#include "nstd/execution/set_done.hpp"
#include "nstd/execution/start.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include "nstd/utility/forward.hpp"
#include "nstd/utility/move.hpp"

// ----------------------------------------------------------------------------

namespace nstd::execution {
    inline constexpr struct just_done_t {
        template <::nstd::execution::receiver> struct state;
        struct sender;
        inline auto operator()() const noexcept -> sender;
    } just_done;
}

// ----------------------------------------------------------------------------

template <::nstd::execution::receiver Receiver>
struct nstd::execution::just_done_t::state
{
    Receiver d_receiver;
    friend auto tag_invoke(::nstd::execution::start_t, state& self) noexcept -> void {
        ::nstd::execution::set_done(::nstd::utility::move(self.d_receiver));
    }
};

// ----------------------------------------------------------------------------

struct nstd::execution::just_done_t::sender {
    using completion_signatures
        = ::nstd::execution::completion_signatures<
            ::nstd::execution::set_done_t()
            >;
    template <template <typename...> class T, template <typename...> class V>
    using value_types = V<T<>>;
    template <template <typename...> class V>
    using error_types = V<>;
    static constexpr bool sends_done{true};

    template <::nstd::execution::receiver Receiver>
    friend auto tag_invoke(::nstd::execution::connect_t, sender const&, Receiver&& receiver)
        -> ::nstd::execution::just_done_t::state<::nstd::type_traits::remove_cvref_t<Receiver>>
    {
        return {::nstd::utility::forward<Receiver>(receiver)};
    }
};

// ----------------------------------------------------------------------------

inline auto nstd::execution::just_done_t::operator()() const noexcept -> sender
{
    return {};
}

// ----------------------------------------------------------------------------

#endif
