// nstd/hidden_names/has_sender_types.t.cpp                           -*-C++-*-
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

#include "nstd/hidden_names/has_sender_types.hpp"
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace HN = ::nstd::hidden_names;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    namespace {
        struct no_types {};

        struct sender {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<int>;
            static constexpr bool sends_done = true;
        };

        struct no_value_types {
            template <template <typename...> class V>
            using error_types = V<int>;
            static constexpr bool sends_done = true;
        };

        struct wrong_value_types {
            template <typename T, template <typename...> class V>
            using value_types = V<T>;
            template <template <typename...> class V>
            using error_types = V<int>;
            static constexpr bool sends_done = true;
        };

        struct no_error_types {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            static constexpr bool sends_done = true;
        };

        struct wrong_error_types {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <typename V>
            using error_types = V;
            static constexpr bool sends_done = true;
        };

        struct no_sends_done {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<int>;
        };

        struct non_static_sends_done {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<int>;
            bool const sends_done = true;
        };

        struct non_constexpr_sends_done {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<int>;
            static bool const sends_done;
        };

        struct non_boolean_sends_done {
            template <template <typename...> class T, template <typename...> class V>
            using value_types = V<T<>>;
            template <template <typename...> class V>
            using error_types = V<int>;
            static constexpr int sends_done = 3;
        };

    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("no nested declarations", []{
            return !HN::has_sender_types<TD::no_types>;
        }),
    KT::expect_success("nested declarations missing value_types", []{
            return !HN::has_sender_types<TD::no_value_types>
                && TD::no_value_types::sends_done
                ;
        }),
    KT::expect_success("nested declarations wrong value_types", []{
            return !HN::has_sender_types<TD::wrong_value_types>
                && TD::wrong_value_types::sends_done
                ;
        }),
    KT::expect_success("nested declarations missing error_types", []{
            return !HN::has_sender_types<TD::no_error_types>
                && TD::no_error_types::sends_done
                ;
        }),
    KT::expect_success("nested declarations wrong error_types", []{
            return !HN::has_sender_types<TD::wrong_error_types>
                && TD::wrong_error_types::sends_done
                ;
        }),
    KT::expect_success("nested declarations missing sends_done", []{
            return !HN::has_sender_types<TD::no_sends_done>;
        }),
    KT::expect_success("nested declarations non-static sends_done", []{
            return !HN::has_sender_types<TD::non_static_sends_done>;
        }),
    KT::expect_success("nested declarations non-constexpr sends_done", []{
            return !HN::has_sender_types<TD::non_constexpr_sends_done>
                && TD::non_constexpr_sends_done::sends_done
                ;
        }),
    KT::expect_success("nested declarations non-boolean sends_done", []{
            return !HN::has_sender_types<TD::non_boolean_sends_done>
                && TD::non_boolean_sends_done::sends_done
                ;
        }),
    KT::expect_success("complete nested declarations", []{
            return HN::has_sender_types<TD::sender>
                && TD::sender::sends_done
                ;
        }),
};

static KT::add_tests suite("has_sender_types", ::tests);

namespace test_declarations {
    namespace {
        bool const non_constexpr_sends_done::sends_done = true;
    }
}
