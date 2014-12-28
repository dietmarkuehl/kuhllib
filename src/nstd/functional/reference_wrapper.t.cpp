// nstd/functional/reference_wrapper.t.cpp                            -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

#include "nstd/functional/reference_wrapper.hpp"
#include "nstd/type_traits/integral_constant.hpp"
#include <tuple>
#include "kuhl/test.hpp"

namespace NF = nstd::functional;
namespace NT = nstd::type_traits;
namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

namespace
{
    struct foo {};

    struct bar {
        int value;
        bar(int value): value(value) {}
        template <typename... Args>
        auto operator()(Args&&... args) -> std::tuple<int, Args...> {
            return std::make_tuple(this->value, args...);
        }
    };

    struct move_ctor {
        template <typename T,
                  typename = decltype(NF::reference_wrapper<T>(T()))>
        static auto test(int) -> NT::true_type;
        template <typename T>
        static auto test(...) -> NT::false_type;
    };

    struct ref_rvalue {
        template <typename T, typename = decltype(NF::ref(T()))>
        static auto test(int) -> NT::true_type;
        template <typename T>
        static auto test(...) -> NT::false_type;
    };
    struct cref_rvalue {
        template <typename T, typename = decltype(NF::cref(T()))>
        static auto test(int) -> NT::true_type;
        template <typename T>
        static auto test(...) -> NT::false_type;
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("typedefs", [](KT::context& c)->bool{
            return KT::assert_type<foo, NF::reference_wrapper<foo>::type>(c, "type")
                //-dk:TODO result_type
                //-dk:TODO argument_type
                //-dk:TODO first_argument_type
                //-dk:TODO second_argument_type
                ;
        }),
    KT::expect_success("ctors", [](KT::context& c)->bool{
            foo object;
            NF::reference_wrapper<foo> const ref0(object);
            NF::reference_wrapper<foo> const ref1(ref0);
            return KT::assert_true(c, "noexcept (object)", noexcept(NF::reference_wrapper<foo>(object)))
                && KT::assert_true(c, "noexcept (copy)", noexcept(NF::reference_wrapper<foo>(ref0)))
                && KT::assert_false(c, "move ctor is deleted", decltype(move_ctor::test<foo>(0))::value)
                && KT::assert_equal(c, "init object (conversion)", &object, &static_cast<foo&>(ref0))
                && KT::assert_equal(c, "init object", &object, &ref0.get())
                && KT::assert_true(c, "noexcept", noexcept(NF::reference_wrapper<foo>(ref0)))
                && KT::assert_equal(c, "copy object (conversion)", &object, &static_cast<foo&>(ref1))
                && KT::assert_equal(c, "copy object", &object, &ref1.get())
                ;
        }),
    KT::expect_success("assignment", [](KT::context& c)->bool{
            foo object0;
            foo object1;
            NF::reference_wrapper<foo> const ref0(object0);
            NF::reference_wrapper<foo>       ref1(object1);
            ref1 = ref0;
            return KT::assert_type<NF::reference_wrapper<foo>&, decltype(ref1 = ref0)>(c, "assignment type")
                && KT::assert_true(c, "noexept", noexcept(ref1 = ref0))
                && KT::assert_equal(c, "copy object (conversion)", &object0, &static_cast<foo&>(ref1))
                && KT::assert_equal(c, "copy object", &object0, &ref1.get())
                ;
        }),
    KT::expect_success("accessor", [](KT::context& c)->bool{
            foo object0;
            NF::reference_wrapper<foo> const ref0(object0);
            return KT::assert_type<foo&, decltype(ref0.get())>(c, "get() type")
                && KT::assert_true(c, "get() noexcept(true)", noexcept(ref0.get()))
                && KT::assert_true(c, "conversion noexcept(true)", noexcept(static_cast<foo&>(ref0)))
                && KT::assert_equal(c, "object (conversion)", &object0, &static_cast<foo&>(ref0))
                && KT::assert_equal(c, "object", &object0, &ref0.get())
                ;
        }),
    KT::expect_success("function call", [](KT::context& c)->bool{
            bar                              object(17);
            NF::reference_wrapper<bar> const ref(object);
            return KT::assert_equal(c, "object state", 17, object.value)
                && KT::assert_type<std::tuple<int>, decltype(ref())>(c, "ref() type")
                && KT::assert_true(c, "ref() value", std::make_tuple(17) == ref())
                && KT::assert_type<std::tuple<int, bool>, decltype(ref(true))>(c, "ref(true) type")
                && KT::assert_true(c, "ref(true) value", std::make_tuple(17, true) == ref(true))
                && KT::assert_type<std::tuple<int, bool, char>, decltype(ref(true, 'c'))>(c, "ref(true, 'c') type")
                && KT::assert_true(c, "ref(true, 'c') value", std::make_tuple(17, true, 'c') == ref(true, 'c'))
                ;
        }),
    KT::expect_success("ref() call", [](KT::context& c)->bool{
            foo                        object;
            foo const                  cobject{};
            NF::reference_wrapper<foo> ref(object);
            return KT::assert_type<NF::reference_wrapper<foo>, decltype(NF::ref(object))>(c, "ref(object) type")
                && KT::assert_type<NF::reference_wrapper<foo const>, decltype(NF::ref(cobject))>(c, "ref(cobject) type")
                && KT::assert_type<NF::reference_wrapper<foo>, decltype(NF::ref(ref))>(c, "ref(ref) type")
                && KT::assert_true(c, "noexcept ref(object)", noexcept(NF::ref(object)))
                && KT::assert_true(c, "noexcept ref(ref)", noexcept(NF::ref(ref)))
                && KT::assert_true(c, "ref(object) value", &object == &NF::ref(object).get())
                && KT::assert_true(c, "ref(ref) value", &object == &NF::ref(ref).get())
                && KT::assert_false(c, "ref(foo()) unavaiable", decltype(ref_rvalue::test<foo>(0))::value)
                ;
        }),
    KT::expect_success("cref() call", [](KT::context& c)->bool{
            foo                              object;
            NF::reference_wrapper<foo>       ref(object);
            NF::reference_wrapper<foo const> cref(object);
            return KT::assert_type<NF::reference_wrapper<foo const>, decltype(NF::cref(object))>(c, "cref(object) type")
                && KT::assert_type<NF::reference_wrapper<foo const>, decltype(NF::cref(ref))>(c, "cref(ref) type")
                && KT::assert_type<NF::reference_wrapper<foo const>, decltype(NF::cref(cref))>(c, "cref(cref) type")
                && KT::assert_true(c, "noexcept cref(object)", noexcept(NF::cref(object)))
                && KT::assert_true(c, "noexcept cref(ref)", noexcept(NF::cref(ref)))
                && KT::assert_true(c, "cref(object) value", &object == &NF::cref(object).get())
                && KT::assert_true(c, "cref(ref) value", &object == &NF::cref(ref).get())
                && KT::assert_false(c, "cref(foo()) unavaiable", decltype(cref_rvalue::test<foo>(0))::value)
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("functional::reference_wrapper", ac, av, ::tests);
}
