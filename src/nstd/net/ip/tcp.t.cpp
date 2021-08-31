// nstd/net/ip/tcp.t.cpp                                              -*-C++-*-
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

#include "nstd/net/ip/tcp.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace NI = ::nstd::net::ip;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        template <typename T>
        concept default_constructible
            = requires(){ T(); }
            ;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("net::ip::tcp isn't default constructible", []{
            return TD::default_constructible<int>
                && not TD::default_constructible<NI::tcp>
                ;
        }),
    KT::expect_success("net::ip::tcp nested type alias", []{
            return KT::type<NI::tcp::endpoint> == KT::type<NI::basic_endpoint<NI::tcp>>
                && KT::type<NI::tcp::resolver> == KT::type<NI::basic_resolver<NI::tcp>>
                && KT::type<NI::tcp::socket> == KT::type<NN::basic_stream_socket<NI::tcp>>
                && KT::type<NI::tcp::acceptor> == KT::type<NN::basic_socket_acceptor<NI::tcp>>
                && KT::type<NI::tcp::iostream> == KT::type<NN::basic_socket_iostream<NI::tcp>>
                ;
        }),
    KT::expect_success("net::ip::tcp nested type no_delay", []{
            return KT::assert_type_exists<NI::tcp::no_delay>;
        }),
    KT::expect_success("net::ip::tcp::v4", []{
            constexpr NI::tcp value(NI::tcp::v4());
            return noexcept(NI::tcp::v4())
                && KT::type<NI::tcp> == KT::type<decltype(NI::tcp::v4())>
                && KT::use(value)
                ;
        }),
    KT::expect_success("net::ip::tcp::v6", []{
            constexpr NI::tcp value(NI::tcp::v6());
            return noexcept(NI::tcp::v6())
                && KT::type<NI::tcp> == KT::type<decltype(NI::tcp::v6())>
                && KT::use(value)
                ;
        }),
    KT::expect_success("net::ip::tcp comparison", []{
            constexpr bool value0 = NI::tcp::v4() == NI::tcp::v4();
            constexpr bool value1 = NI::tcp::v4() != NI::tcp::v4();
            return KT::use(value0)
                && KT::use(value1)
                && noexcept(NI::tcp::v4() == NI::tcp::v4())
                && noexcept(NI::tcp::v4() != NI::tcp::v4())
                && KT::type<bool> == KT::type<decltype(NI::tcp::v4() == NI::tcp::v4())>
                && KT::type<bool> == KT::type<decltype(NI::tcp::v4() != NI::tcp::v4())>
                && NI::tcp::v4() == NI::tcp::v4()
                && not(NI::tcp::v4() != NI::tcp::v4())
                ;
        }),
    KT::expect_success("net::ip::tcp::v4() config", []{
            return AF_INET     == NI::tcp::v4().family()
                && SOCK_STREAM == NI::tcp::v4().type()
                && 0           == NI::tcp::v4().protocol()
                ;
        }),
    KT::expect_success("net::ip::tcp::v6() config", []{
            return AF_INET6    == NI::tcp::v6().family()
                && SOCK_STREAM == NI::tcp::v6().type()
                && 0           == NI::tcp::v6().protocol()
                ;
        }),
};

static KT::add_tests suite("tcp", ::tests);
