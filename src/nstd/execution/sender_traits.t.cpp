// nstd/execution/sender_traits.t.cpp                                 -*-C++-*-
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

#include "nstd/execution/sender_traits.hpp"
#include <type_traits>
#include <variant>
#include <tuple>
#include "kuhl/test.hpp"

namespace test_declarations {}
namespace TD = test_declarations;
namespace EX = ::nstd::execution;
namespace TT = ::std;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations {
    template <typename Sender>
    concept has_sender_traits
        = !requires{ typename EX::sender_traits<TT::remove_cvref_t<Sender>>::not_specialized; }
        ;

    //template <template <template <typename...> class, template <typename...> class> class>
    //class test_value_types;
    //template <template <template <typename...> class> class>
    //class test_error_types;

    struct non_sender {};

    struct typed_sender_missing_value_types
    {
        template <template <typename...> class Tuple>
        using error_types = Tuple<int>;
        static constexpr bool sends_done = true;
    };
    struct typed_sender_missing_error_types
    {
        template <template <typename...> class Variant, template <typename...> class Tuple>
        using value_types = Variant<Tuple<bool>>;
        static constexpr bool sends_done = true;
    };
    struct typed_sender_missing_sends_done
    {
        template <template <typename...> class Variant, template <typename...> class Tuple>
        using value_types = Variant<Tuple<bool>>;
        template <template <typename...> class Tuple>
        using error_types = Tuple<int>;
    };
    struct typed_sender
    {
        template <template <typename...> class Tuple, template <typename...> class Variant>
        using value_types = Variant<Tuple<bool>>;
        template <template <typename...> class Tuple>
        using error_types = Tuple<int>;
        static constexpr bool sends_done = true;
    };

    struct derived_sender
        : EX::sender_base
    {
    };

    struct derived_typed_sender
        : EX::sender_base
    {
        template <template <typename...> class Tuple, template <typename...> class Variant>
        using value_types = Variant<Tuple<bool>>;
        template <template <typename...> class Tuple>
        using error_types = Tuple<int>;
        static constexpr bool sends_done = true;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("a random class doesn't have sender_traits", []{
            return !TD::has_sender_traits<TD::non_sender>
                && !TD::has_sender_traits<TD::typed_sender_missing_value_types>
                && !TD::has_sender_traits<TD::typed_sender_missing_error_types>
                && !TD::has_sender_traits<TD::typed_sender_missing_sends_done>
                ;
        }),
    KT::expect_success("a typed sender has sender_traits", []{
            return TD::has_sender_traits<TD::typed_sender>
                && KT::type<EX::sender_traits<TD::typed_sender>::value_types<::std::variant, ::std::tuple>>
                    == KT::type<TD::typed_sender::value_types<::std::variant, ::std::tuple>>
                && KT::type<EX::sender_traits<TD::typed_sender>::error_types<::std::tuple>>
                    == KT::type<TD::typed_sender::error_types<::std::tuple>>
                && sizeof(::nstd::type_traits::bool_constant<EX::sender_traits<TD::typed_sender>::sends_done>)
                && EX::sender_traits<TD::typed_sender>::sends_done == TD::typed_sender::sends_done
                ;
        }),
    KT::expect_success("-dk:TODO: executor-of_impl<S, as-invocable<void-receiver, S>", []{
            return true;
        }),
    KT::expect_success("a class derived from execution::sender_base has sender_traits", []{
            return TD::has_sender_traits<TD::derived_sender>
                ;
        }),
    KT::expect_success("a typed sender derived execution::sender_base has sender_traits", []{
            return TD::has_sender_traits<TD::derived_typed_sender>
                && KT::type<EX::sender_traits<TD::derived_typed_sender>::value_types<::std::variant, ::std::tuple>>
                    == KT::type<TD::derived_typed_sender::value_types<::std::variant, ::std::tuple>>
                && KT::type<EX::sender_traits<TD::derived_typed_sender>::error_types<::std::tuple>>
                    == KT::type<TD::derived_typed_sender::error_types<::std::tuple>>
                && sizeof(::nstd::type_traits::bool_constant<EX::sender_traits<TD::derived_typed_sender>::sends_done>)
                && EX::sender_traits<TD::derived_typed_sender>::sends_done == TD::derived_typed_sender::sends_done
                ;
        }),
};

static KT::add_tests suite("sender_traits", ::tests);
