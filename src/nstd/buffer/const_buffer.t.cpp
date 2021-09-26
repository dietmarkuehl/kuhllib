// nstd/buffer/const_buffer.t.cpp                                     -*-C++-*-
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

#include "nstd/buffer/const_buffer.hpp"
#include "nstd/memory/addressof.hpp"
#include "nstd/type_traits/declval.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = ::test_declarations;
namespace KT = ::kuhl::test;
namespace NN = ::nstd::net;
namespace NM = ::nstd::memory;
namespace TT = ::nstd::type_traits;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct type {
            int i{};
        };
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("iovec compatibility", []{
            return sizeof(NN::const_buffer) == sizeof(::iovec);
        }),
    KT::expect_success("interface", []{
            return KT::type<decltype(NN::const_buffer().data())> == KT::type<void const*>
                && noexcept(TT::declval<NN::const_buffer const>().data())
                && KT::type<decltype(NN::const_buffer().size())> == KT::type<::std::size_t>
                && noexcept(TT::declval<NN::const_buffer const>().size())
                && KT::type<decltype(NN::const_buffer() += ::std::size_t() )> == KT::type<NN::const_buffer&>
                && noexcept(TT::declval<NN::const_buffer>() += ::std::size_t())
                ;
        }),
    KT::expect_success("default ctor", []{
            return NN::const_buffer().size() == ::std::size_t()
                && NN::const_buffer().data() == nullptr
                ;
        }),
    KT::expect_success("value ctor", []{
            char const             data[5] = {};
            NN::const_buffer const buffer(data, sizeof(data));
            return buffer.size() == 5u
                && buffer.data() == data
                ;
        }),
    KT::expect_success("mutable_buffer ctor", []{
            char                   data[5] = {};
            NN::const_buffer const buffer(NN::mutable_buffer(data, sizeof(data)));
            return buffer.size() == 5u
                && buffer.data() == data
                ;
        }),
    KT::expect_success("operator+=", []{
            char const       data[5] = {};
            NN::const_buffer buffer(data, sizeof(data));
            return &buffer == &(buffer += 2u)
                && buffer.data() == data + 2u
                && buffer.size() == sizeof(data) - 2u
                && &buffer == &(buffer += sizeof(data))
                && buffer.data() == data + sizeof(data)
                && buffer.size() == 0u
                ;
        }),
    KT::expect_success("buffer_sequence_begin", []{
            NN::const_buffer const buffer;
            return KT::type<decltype(NN::buffer_sequence_begin(buffer))>
                    == KT::type<NN::const_buffer const*>
                && noexcept(NN::buffer_sequence_begin(buffer))
                && &buffer == NN::buffer_sequence_begin(buffer)
                ;
        }),
    KT::expect_success("buffer_sequence_end", []{
            NN::const_buffer const buffer;
            return KT::type<decltype(NN::buffer_sequence_end(buffer))>
                    == KT::type<NN::const_buffer const*>
                && noexcept(NN::buffer_sequence_end(buffer))
                && &buffer + 1 == NN::buffer_sequence_end(buffer)
                ;
        }),
    KT::expect_success("arithmetic", []{
            char data[7] = {};
            NN::const_buffer const buffer(data, sizeof(data));

            return KT::type<decltype(buffer + std::size_t())>
                    == KT::type<NN::const_buffer>
                && KT::type<decltype(std::size_t() + buffer)>
                    == KT::type<NN::const_buffer>
                && noexcept(buffer + std::size_t())
                && noexcept(std::size_t() + buffer)
                && (buffer + 3).data() == data + 3
                && (buffer + 3).size() == sizeof(data) - 3
                && (3 + buffer).data() == data + 3
                && (3 + buffer).size() == sizeof(data) - 3
                && (buffer + 3 + sizeof(data)).data() == data + sizeof(data)
                && (buffer + 3 + sizeof(data)).size() == 0u
                && (3 + sizeof(data) + buffer).data() == data + sizeof(data)
                && (3 + sizeof(data) + buffer).size() == 0u
                ;
        }),
    KT::expect_success("factory from void const*", []{
            char const  data[7] = {};
            void const* ptr = data;
            auto b = NN::buffer(ptr, sizeof(data));
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(ptr, sizeof(data)))
                && b.data() == data
                && b.size() == sizeof(data)
                ;
        }),
    KT::expect_success("factory from const_buffer", []{
            char const             data[7] = {};
            NN::const_buffer const orig(data, sizeof(data));
            auto                   b(NN::buffer(orig));

            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(orig))
                && b.data() == data
                && b.size() == sizeof(data)
                ;
        }),
    KT::expect_success("sized factory from const_buffer", []{
            char const             data[7] = {};
            NN::const_buffer const orig(data, sizeof(data));
            auto                   b(NN::buffer(orig, 3));

            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(orig))
                && NN::buffer(orig, 3).data() == data
                && NN::buffer(orig, 3).size() == 3
                && NN::buffer(orig, sizeof(data) + 3).data() == data
                && NN::buffer(orig, sizeof(data) + 3).size() == sizeof(data)
                && NN::buffer(orig, 0).data() == data
                && NN::buffer(orig, 0).size() == 0
                ;
        }),
    KT::expect_success("factory from const built-in array", []{
            TD::type const  data[7] = {};
            auto b = NN::buffer(data);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == data
                && b.size() == sizeof(data)
                ;
        }),
    KT::expect_success("factory from non-const array<T const, N>", []{
            ::std::array<TD::type const, 7>  data{};
            ::std::array<TD::type const, 0>  data0;
            auto b = NN::buffer(data);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == NM::addressof(data)
                && b.size() == sizeof(data)
                && NN::buffer(data0).data() == nullptr
                && NN::buffer(data0).size() == 0u
                ;
        }),
    KT::expect_success("factory from const array<T, N>", []{
            ::std::array<TD::type, 7> const data;
            ::std::array<TD::type, 0> const data0;
            auto b = NN::buffer(data);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == NM::addressof(data)
                && b.size() == sizeof(data)
                && NN::buffer(data0).data() == nullptr
                && NN::buffer(data0).size() == 0u
                ;
        }),
    KT::expect_success("factory from const vector<T>", []{
            ::std::vector<TD::type> const data(7);
            ::std::vector<TD::type> const data0;
            auto b = NN::buffer(data);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == data.data()
                && b.size() == data.size() * sizeof(TD::type)
                && NN::buffer(data0).data() == nullptr
                && NN::buffer(data0).size() == 0u
                ;
        }),
    KT::expect_success("factory from const string", []{
            ::std::string const data(7, ' ');
            ::std::string const data0;
            auto b = NN::buffer(data);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == data.data()
                && b.size() == data.size()
                && NN::buffer(data0).data() == nullptr
                && NN::buffer(data0).size() == 0u
                ;
        }),
    KT::expect_success("factory from const string_view", []{
            ::std::string const data(7, ' ');
            ::std::string_view  view(data);
            ::std::string const data0;
            ::std::string_view  view0(data0);
            auto b = NN::buffer(view);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(view))
                && b.data() == data.data()
                && b.size() == data.size()
                && NN::buffer(view0).data() == nullptr
                && NN::buffer(view0).size() == 0u
                ;
        }),


    KT::expect_success("sized factory from built-in array", []{
            TD::type const  data[7] = {};
            auto b = NN::buffer(data, 3u);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == data
                && b.size() == 3u
                && NN::buffer(data, 3u + sizeof(data)).data() == data
                && NN::buffer(data, 3u + sizeof(data)).size() == sizeof(data)
                ;
        }),
    KT::expect_success("sized factory from non-const array<T const, N>", []{
            ::std::array<TD::type const, 7>  data{};
            ::std::array<TD::type const, 0>  data0;
            auto b = NN::buffer(data, 3);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == NM::addressof(data)
                && b.size() == 3
                && NN::buffer(data0, 3).data() == nullptr
                && NN::buffer(data0, 3).size() == 0u
                && NN::buffer(data, 3 + sizeof(data)).data() == NM::addressof(data)
                && NN::buffer(data, 3 + sizeof(data)).size() == sizeof(data)
                ;
        }),
    KT::expect_success("sized factory from const array<T, N>", []{
            ::std::array<TD::type, 7> const data;
            ::std::array<TD::type, 0> const data0;
            auto b = NN::buffer(data, 3);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == NM::addressof(data)
                && b.size() == 3
                && NN::buffer(data0, 3).data() == nullptr
                && NN::buffer(data0, 3).size() == 0u
                && NN::buffer(data, 3 + sizeof(data)).data() == NM::addressof(data)
                && NN::buffer(data, 3 + sizeof(data)).size() == sizeof(data)
                ;
        }),
    KT::expect_success("sized factory from const vector<T>", []{
            ::std::vector<TD::type> const data(7);
            ::std::vector<TD::type> const data0;
            auto b = NN::buffer(data, 3);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == data.data()
                && b.size() == 3
                && NN::buffer(data0, 3).data() == nullptr
                && NN::buffer(data0, 3).size() == 0u
                && NN::buffer(data, 3 + data.size() * sizeof(TD::type)).data() == data.data()
                && NN::buffer(data, 3 + data.size() * sizeof(TD::type)).size() == data.size() * sizeof(TD::type)
                ;
        }),
    KT::expect_success("sized factory from const string", []{
            ::std::string const data(7, ' ');
            ::std::string const data0;
            auto b = NN::buffer(data, 3);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(data))
                && b.data() == data.data()
                && b.size() == 3
                && NN::buffer(data0, 3).data() == nullptr
                && NN::buffer(data0, 3).size() == 0u
                && NN::buffer(data, data.size() + 3).data() == data.data()
                && NN::buffer(data, data.size() + 3).size() == data.size()
                ;
        }),
    KT::expect_success("sized factory from const string_view", []{
            ::std::string const data(7, ' ');
            ::std::string_view  view(data);
            ::std::string const data0;
            ::std::string_view  view0(data0);
            auto b = NN::buffer(view, 3);
            return KT::type<decltype(b)> == KT::type<NN::const_buffer>
                && noexcept(NN::buffer(view))
                && b.data() == data.data()
                && b.size() == 3
                && NN::buffer(view0).data() == nullptr
                && NN::buffer(view0).size() == 0u
                && NN::buffer(view, 3 + data.size()).data() == data.data()
                && NN::buffer(view, 3 + data.size()).size() == data.size()
                ;
        }),
};

static KT::add_tests suite("const_buffer", ::tests);
