// nstd/hidden_names/print_completion_signatures.hpp                  -*-C++-*-
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

#ifndef INCLUDED_NSTD_HIDDEN_NAMES_PRINT_COMPLETION_SIGNATURES
#define INCLUDED_NSTD_HIDDEN_NAMES_PRINT_COMPLETION_SIGNATURES

#include "nstd/execution/completion_signatures.hpp"
#include "nstd/execution/get_completion_signatures.hpp"
#include "nstd/execution/sender.hpp"
#include "nstd/execution/set_error.hpp"
#include "nstd/execution/set_stopped.hpp"
#include "nstd/execution/set_value.hpp"
#include "nstd/type_traits/remove_cvref.hpp"
#include <iostream>
#include <string>
#include <exception>

// ----------------------------------------------------------------------------

namespace nstd::hidden_names {
    template <typename> ::std::ostream& print_tag(::std::ostream&);
    template <> inline std::ostream& print_tag<::nstd::execution::set_value_t>(::std::ostream& out) { return out << "set_value_t"; }
    template <> inline std::ostream& print_tag<::nstd::execution::set_error_t>(::std::ostream& out) { return out << "set_error_t"; }
    template <> inline std::ostream& print_tag<::nstd::execution::set_stopped_t>(::std::ostream& out) { return out << "set_stopped_t"; }

    template <typename> struct print_type;
    template <> struct print_type<int> { static ::std::string str() { return "int"; } };
    template <> struct print_type<double> { static ::std::string str() { return "double"; } };
    template <> struct print_type<::std::exception_ptr> { static ::std::string str() { return "std::exception_ptr"; } };
    template <> struct print_type<::std::string> { static ::std::string str() { return "std::string"; } };

    template <typename T> struct print_type<T&&> { static ::std::string str() { return print_type<T>::str() + "&&"; } };
    template <typename T> struct print_type<T&> { static ::std::string str() { return print_type<T>::str() + "&"; } };
    template <typename T> struct print_type<T const> { static ::std::string str() { return print_type<T>::str() + " const"; } };

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
    struct printer<::nstd::execution::completion_signatures<S...>> {
        static auto print() {
            std::cout << "signatures(" << sizeof...(S) << ")\n";
            ((signature_printer<S>::print()), ...);
            std::cout << "-----\n";
        }
    };
    struct env {};
    template <::nstd::execution::sender Sender>
    void print_completion_signatures(Sender&& s) {
        printer<decltype(::nstd::execution::get_completion_signatures(s, env{}))>::print();
    }
}

// ----------------------------------------------------------------------------

#endif
