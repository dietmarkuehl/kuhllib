// nstd/container/intrusive_list.t.cpp                                -*-C++-*-
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

#include "nstd/container/intrusive_list.hpp"
#include "nstd/utility/move.hpp"
#include "kuhl/test.hpp"
#include <algorithm>
#include <array>

namespace NC = ::nstd::container;
namespace UT = ::nstd::utility;
namespace test_declarations {};
namespace TD = test_declarations;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct type {
            int value;
            NC::intrusive_list_node<type> link;

            type(int value = {}): value(value) {}
            auto operator== (type const& other) const -> bool {
                return this->value == other.value;
            }
        };

        template <typename A, typename B>
        bool compare(A const& a, B const& b) {
            return ::std::equal(a.begin(), a.end(), b.begin(), b.end());
        }
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("breating", []{
            NC::intrusive_list<TD::type> list0;
            NC::intrusive_list<TD::type> list1;
            TD::type v0{17};
            auto it0 = list1.insert(list1.end(), v0);

            return true
                && TD::compare(::std::array<TD::type, 0>{}, list0)
                && TD::compare(::std::array<TD::type, 1>{ 17 }, list1)
                && &*it0 == &v0
                ;
        }),
    KT::expect_success("insert", []{
            TD::type v[] = { 17, 42, 8, 11 };
            NC::intrusive_list<TD::type> list;
            if (!compare(::std::array<TD::type, 0>{}, list)) return false;

            auto it0 = list.insert(list.begin(), v[0]);
            if (!compare(::std::array<TD::type, 1>{ 17 }, list)) return false;

            list.insert(list.begin(), v[1]);
            if (!compare(::std::array<TD::type, 2>{ 42, 17 }, list)) return false;

            list.insert(it0, v[2]);
            if (!compare(::std::array<TD::type, 3>{ 42, 8, 17 }, list)) return false;

            list.insert(list.end(), v[3]);
            if (!compare(::std::array<TD::type, 4>{ 42, 8, 17, 11 }, list)) return false;

            return true;
        }),
    KT::expect_success("push_back", []{
            TD::type v[] = { 17, 42, 8, 11 };
            NC::intrusive_list<TD::type> list;
            if (!compare(::std::array<TD::type, 0>{}, list)) return false;

            list.push_back(v[0]);
            if (!compare(::std::array<TD::type, 1>{ 17 }, list)) return false;

            list.push_back(v[1]);
            if (!compare(::std::array<TD::type, 2>{ 17, 42 }, list)) return false;

            list.push_back(v[2]);
            if (!compare(::std::array<TD::type, 3>{ 17, 42, 8 }, list)) return false;

            list.push_back(v[3]);
            if (!compare(::std::array<TD::type, 4>{ 17, 42, 8, 11 }, list)) return false;

            return true;
        }),
    KT::expect_success("front", []{
            TD::type v[] = { 17, 42 };
            NC::intrusive_list<TD::type> list;
            list.push_back(v[0]);
            list.push_back(v[1]);
            return v[0] == list.front();
        }),

    KT::expect_success("empty()", []{
            TD::type v[] = { 17, 43, 8, 11 };
            NC::intrusive_list<TD::type> list;
            if (!list.empty()) return false;
            list.insert(list.end(), v[0]);
            return !list.empty();
        }),
    KT::expect_success("move ctor", []{
            TD::type v[] = { 17, 43, 8, 11 };
            NC::intrusive_list<TD::type> list;
            for (auto& r: v) {
                list.insert(list.end(), r);
            }
            if (!compare(std::array<TD::type, 4>{ 17, 43, 8, 11 }, list)) return false;

            NC::intrusive_list<TD::type> moved(UT::move(list));
            if (!compare(std::array<TD::type, 4>{ 17, 43, 8, 11 }, moved)) return false;

            return true;
        }),
    KT::expect_success("move assign", []{
            TD::type v[] = { 17, 43, 8, 11 };
            NC::intrusive_list<TD::type> list;
            for (auto& r: v) {
                list.insert(list.end(), r);
            }
            if (!compare(std::array<TD::type, 4>{ 17, 43, 8, 11 }, list)) return false;

            NC::intrusive_list<TD::type> moved;
            moved = UT::move(list);
            if (!compare(std::array<TD::type, 4>{ 17, 43, 8, 11 }, moved)) return false;

            return true;
        }),
    KT::expect_success("erase", []{
            TD::type v[] = { 17, 42, 8, 11 };
            NC::intrusive_list<TD::type> list;

            auto it0 = list.insert(list.end(), v[0]);
            if (&*it0 != v + 0) return false;
            auto it1 = list.insert(list.end(), v[1]);
            if (&*it1 != v + 1) return false;
            auto it2 = list.insert(list.end(), v[2]);
            if (&*it2 != v + 2) return false;
            auto it3 = list.insert(list.end(), v[3]);
            if (&*it3 != v + 3) return false;
            if (!compare(::std::array<TD::type, 4>{ 17, 42, 8, 11 }, list)) return false;

            list.erase(it2);
            if (!compare(::std::array<TD::type, 3>{ 17, 42, 11 }, list)) return false;
            list.erase(it3);
            if (!compare(::std::array<TD::type, 2>{ 17, 42 }, list)) return false;
            list.erase(it0);
            if (!compare(::std::array<TD::type, 1>{ 42 }, list)) return false;
            list.erase(it1);
            if (!compare(::std::array<TD::type, 0>{ }, list)) return false;

            return true;
        }),
    KT::expect_success("pop_front", []{
            TD::type v[] = { 17, 42, 8, 11 };
            NC::intrusive_list<TD::type> list;
            for (auto& r: v) {
                list.push_back(r);
            }
            if (!compare(::std::array<TD::type, 4>{ 17, 42, 8, 11 }, list)) return false;
            list.pop_front();
            return compare(::std::array<TD::type, 3>{ 42, 8, 11 }, list);
        }),

};

static KT::add_tests suite("intrusive_list", ::tests);