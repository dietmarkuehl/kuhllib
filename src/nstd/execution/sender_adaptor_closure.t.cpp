// nstd/execution/sender_adaptor_closure.t.cpp                        -*-C++-*-
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

#include "nstd/execution/sender_adaptor_closure.hpp"
#include "nstd/execution/just.hpp"
#include "nstd/execution/then.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/thread/sync_wait.hpp"
#include "kuhl/test.hpp"

namespace test_declaration {}
namespace TD = ::test_declaration;
namespace KT = ::kuhl::test;
namespace EX = ::nstd::execution;
namespace TT = ::nstd::this_thread;
namespace UT = ::nstd::utility;

// ----------------------------------------------------------------------------

namespace test_declaration {
    namespace {
        constexpr struct sender
            : ::nstd::execution::sender_adaptor_closure<sender> {
            template <::nstd::execution::sender Sender>
            auto operator()(Sender&& sender) const {
                return EX::then(::nstd::utility::forward<Sender>(sender), [](int v){ return 2 * v; });
            };
            template <::nstd::execution::sender Sender, typename T>
            auto operator()(Sender&& sender, T&& x) const {
                return EX::then(sender, [=](int v){ return ((v * x) + 0); });
            };
            //template <::nstd::execution::sender Sender, typename... T>
            //auto operator()(Sender&& sender, T&&... x) const {
            //    return EX::then(sender, [=](int v){ return ((v * x) + ... + 0); });
            //};
        } cpo;

        struct env {};
        struct receiver {
            friend auto tag_invoke(EX::get_env_t, receiver const&) noexcept -> env { return {}; }
            friend auto tag_invoke(EX::set_value_t, receiver&&, auto...) noexcept -> void {}
            friend auto tag_invoke(EX::set_error_t, receiver&&, auto) noexcept -> void {}
            friend auto tag_invoke(EX::set_stopped_t, receiver&&) noexcept -> void {}
        };
    }
}

#include "nstd/type_traits/remove_cvref.hpp"
namespace XX {
    template <typename> std::ostream& print_tag(std::ostream&);
    template <> std::ostream& print_tag<EX::set_value_t>(std::ostream& out) { return out << "set_value_t"; }
    template <> std::ostream& print_tag<EX::set_error_t>(std::ostream& out) { return out << "set_error_t"; }
    template <> std::ostream& print_tag<EX::set_stopped_t>(std::ostream& out) { return out << "set_stopped_t"; }

    template <typename> struct print_type;
    template <> struct print_type<int> { static std::string str() { return "int"; } };
    template <> struct print_type<double> { static std::string str() { return "double"; } };

    template <typename T> struct print_type<T&&> { static std::string str() { return print_type<T>::str() + "&&"; } };
    template <typename T> struct print_type<T&> { static std::string str() { return print_type<T>::str() + "&"; } };
    template <typename T> struct print_type<T const> { static std::string str() { return print_type<T>::str() + " const"; } };

    template <typename T>
    std::ostream& operator<< (std::ostream& out, print_type<T> const&) {
        return out << print_type<T>::str() << ", ";
    };

    template <typename> struct signature_printer;
    template <typename Tag, typename... Args>
    struct signature_printer<Tag(Args...)> {
        static auto print() {
            std::cout << print_tag<Tag> << "(";
            (std::cout << ... << (print_type<Args>()));
            std::cout << ")\n";
        }
    };

    template <typename> struct printer;
    template <typename... S>
    struct printer<EX::completion_signatures<S...>> {
        static auto print() {
            ((signature_printer<S>::print()), ...);
        }
    };
    struct env {};
    template <EX::sender Sender>
    void print_completion_signatures(Sender&& s) {
        // printer<typename ::nstd::type_traits::remove_cvref_t<Sender>::completion_signatures>::print();
        printer<decltype(EX::get_completion_signatures(s, env{}))>::print();
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("basic operation", []{
            //auto sender = TD::cpo(EX::just(17));
            (void)TD::cpo;
            auto sender = EX::then(EX::just(17), [](int v){ return 2 * v; });

            XX::print_completion_signatures(EX::just(17));
            XX::print_completion_signatures(EX::just(17, 3.14));
            XX::print_completion_signatures(sender);
#if 1
            auto state = EX::connect(UT::move(sender), TD::receiver());
            EX::start(state);
            std::optional<std::tuple<int>> value;
            (void)value;
#else
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            (void)value;
#endif
#if 1
            return true;
#else
            return value
                && ::std::get<0>(*value) == 17
                ;
#endif
        }),
#if 0
    KT::expect_success("one argument", []{
            auto sender = TD::cpo(EX::just(2), 1);
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 12
                ;
        }),
#endif
#if 0
    KT::expect_success("basic pipe", []{
            auto sender = EX::just(17) | TD::cpo;
            auto value = TT::sync_wait(::nstd::utility::move(sender));
            return value
                && ::std::get<0>(*value) == 17
                ;
        }),
#endif
};

static KT::add_tests suite("nstd/execution/sender_adaptor_closure", ::tests);
