// nstd/hidden_names/message_flags.hpp                                -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_MESSAGE_FLAGS
#define INCLUDED_NSTD_HIDDEN_NAMES_MESSAGE_FLAGS

#include <functional>
#include <iosfwd>
#include "nstd/file/socket.hpp"

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    enum class message_flags: unsigned int {
        peek = MSG_PEEK,
        out_of_band = MSG_OOB,
        do_not_route = MSG_DONTROUTE
    };

    template <typename Operation>
    constexpr auto message_flags_apply(message_flags m0, message_flags m1) {
        return message_flags(Operation()(static_cast<unsigned int>(m0), static_cast<unsigned int>(m1)));
    }

    inline constexpr auto operator& (message_flags m0, message_flags m1) -> message_flags {
        return message_flags_apply<::std::bit_and<>>(m0, m1);
    }
    inline constexpr auto operator| (message_flags m0, message_flags m1) -> message_flags {
        return message_flags_apply<::std::bit_or<>>(m0, m1);
    }
    inline constexpr auto operator^ (message_flags m0, message_flags m1) -> message_flags {
        return message_flags_apply<::std::bit_xor<>>(m0, m1);
    }
    inline constexpr auto operator~ (message_flags m) -> message_flags {
        return message_flags(~static_cast<unsigned int>(m));
    }
    inline constexpr auto operator&= (message_flags& m0, message_flags m1) -> message_flags& {
        return m0 = m0 & m1;
    }
    inline constexpr auto operator|= (message_flags& m0, message_flags m1) -> message_flags& {
        return m0 = m0 | m1;
    }
    inline constexpr auto operator^= (message_flags& m0, message_flags m1) -> message_flags& {
        return m0 = m0 ^ m1;
    }

    auto operator<< (::std::ostream&, message_flags) -> ::std::ostream&;
}

// ----------------------------------------------------------------------------

#endif
