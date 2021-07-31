// kuhltest_test.hpp                                                  -*-C++-*-
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

#ifndef INCLUDED_KUHLTEST_TEST
#define INCLUDED_KUHLTEST_TEST

#include "kuhl/test/context.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace kuhl
{
    namespace test
    {
        namespace detail {
            template <typename, typename> struct is_same_type { static constexpr bool value{false}; };
            template <typename T> struct is_same_type<T, T> { static constexpr bool value{true}; };
            template <typename T0, typename T1>
            inline constexpr bool is_same_type_v{is_same_type<T0, T1>::value};
        }
        class testcase;
        class add_tests;

        kuhl::test::testcase
        expect_success(char const*, bool(*)());
        kuhl::test::testcase
        expect_success(char const*, bool(*)(kuhl::test::context&));

        kuhl::test::testcase
        expect_failure(char const*, bool(*)());
        kuhl::test::testcase
        expect_failure(char const*, bool(*)(kuhl::test::context&));

        template <typename Exception>
        kuhl::test::testcase
        expect_exception(char const*, bool(*)());
        template <typename Exception>
        kuhl::test::testcase
        expect_exception(char const*, bool(*)(kuhl::test::context&));

        template <typename E, typename... M>
        kuhl::test::testcase
        check_enum(char const*, M&&...);

        int run_tests(char const*, int, char*[],
                      kuhl::test::testcase const*, kuhl::test::testcase const*);
        template <std::size_t NoTests>
        int run_tests(char const*, int, char*[], 
                      kuhl::test::testcase const (&tests)[NoTests]);

        template <typename T>
        bool use(T const&);
    }
}

// ----------------------------------------------------------------------------

class kuhl::test::testcase
{
private:
    char const* d_name;
    bool (*d_no_context)();
    bool (*d_context)(kuhl::test::context&);
    bool (testcase::*d_run_test)(std::ostream&, kuhl::test::context&) const;

    bool expect_success_with_context(std::ostream&      ,
                                     kuhl::test::context&) const;
    bool expect_success_without_context(std::ostream&      ,
                                        kuhl::test::context&) const;
    bool expect_failure_with_context(std::ostream&      ,
                                     kuhl::test::context&) const;
    bool expect_failure_without_context(std::ostream&      ,
                                        kuhl::test::context&) const;
    template <typename Exception>
    bool expect_exception_with_context(std::ostream&      ,
                                       kuhl::test::context&) const;
    template <typename Exception>
    bool expect_exception_without_context(std::ostream&      ,
                                          kuhl::test::context&) const;

public:
    testcase(bool, char const*, bool (*)());
    testcase(bool, char const*, bool (*)(kuhl::test::context&));
    template <typename Exception>
    testcase(Exception*, char const*, bool (*)());
    template <typename Exception>
    testcase(Exception*, char const*, bool (*)(kuhl::test::context&));

    bool run(std::ostream&, std::size_t, char const*) const;
};

// ----------------------------------------------------------------------------

class kuhl::test::add_tests
{
private:
    struct suite
    {
        char const*                 name;
        kuhl::test::testcase const* begin;
        kuhl::test::testcase const* end;
    };
    static std::vector<suite>& suites();

public:
    template <std::size_t NoTests>
    add_tests(char const* name, kuhl::test::testcase const (&tests)[NoTests]) {
        suites().push_back(suite{name, +tests, +tests + NoTests});
    }
    static int run(int ac, char* av[]);
};

// ----------------------------------------------------------------------------

inline
kuhl::test::testcase::testcase(bool success, char const* name, bool (*test)())
    : d_name(name)
    , d_no_context(test)
    , d_context()
    , d_run_test(success
                 ? &testcase::expect_success_without_context
                 : &testcase::expect_failure_without_context)
{
}

inline
kuhl::test::testcase::testcase(bool success, char const* name,
                              bool (*test)(kuhl::test::context&))
    : d_name(name)
    , d_no_context()
    , d_context(test)
    , d_run_test(success
                 ? &testcase::expect_success_with_context
                 : &testcase::expect_failure_with_context)
{
}

template <typename Exception>
inline
kuhl::test::testcase::testcase(Exception*, char const* name, bool (*test)())
    : d_name(name)
    , d_no_context(test)
    , d_context()
    , d_run_test(&testcase::expect_exception_without_context<Exception>)
{
}

template <typename Exception>
inline
kuhl::test::testcase::testcase(Exception*, char const* name,
                              bool (*test)(kuhl::test::context&))
    : d_name(name)
    , d_no_context()
    , d_context(test)
    , d_run_test(&testcase::expect_exception_with_context<Exception>)
{
}

// ----------------------------------------------------------------------------

inline
bool
kuhl::test::testcase::expect_success_with_context(std::ostream&       out,
                                                 kuhl::test::context& context) const
{
    try {
        context.reset();
        bool result(this->d_context(context));
        out << (result? " OK": " fail")
            << (context.empty()? "": " ") << context.c_str()
            << '\n';
        return result;
    }
    catch (...) {
        out << "unexpected exception\n";
        return false;
    }
}

inline
bool
kuhl::test::testcase::expect_failure_with_context(std::ostream&       out,
                                                 kuhl::test::context& context) const
{
    try {
        bool result(this->d_context(context));
        out << (result? " OK (unexpected)": " fail (expected)") << '\n';
        return result;
    }
    catch (...) {
        out << "exception (fail expected)\n";
        return false;
    }
}

inline
bool
kuhl::test::testcase::expect_success_without_context(std::ostream&       out,
                                                    kuhl::test::context& ) const
{
    try {
        bool result(this->d_no_context());
        out << (result? " OK": " fail") << '\n';
        return result;
    }
    catch (...) {
        out << "unexpected exception\n";
        return false;
    }
}

inline
bool
kuhl::test::testcase::expect_failure_without_context(std::ostream&       out,
                                                    kuhl::test::context& ) const
{
    try {
        bool result(this->d_no_context());
        out << (result? " OK (unexpected)": " fail (expected)") << '\n';
        return result;
    }
    catch (...) {
        out << "exception (fail expected)\n";
        return false;
    }
}

template <typename Exception>
inline
bool
kuhl::test::testcase::expect_exception_with_context(
                                             std::ostream&       out,
                                             kuhl::test::context& context) const
{
    try {
        bool result(this->d_context(context));
        out << (result
                      ? " fail (OK instead of exception)"
                      : " fail (instead of exception)") << '\n';
        return false;
    }
    catch (Exception const&) {
        out << " OK (received exception)\n";
        return true;
    }
    catch (...) {
        out << " fail (different exception)\n";
        return false;
    }
}

template <typename Exception>
inline
bool
kuhl::test::testcase::expect_exception_without_context(
                                                    std::ostream&       out,
                                                    kuhl::test::context& ) const
{
    try {
        bool result(this->d_no_context());
        out << (result
                      ? " fail (OK instead of exception)"
                      : " fail (instead of exception)") << '\n';
        return false;
    }
    catch (Exception const&) {
        out << " OK (received exception)\n";
        return true;
    }
    catch (...) {
        out << " fail (different exception)\n";
        return false;
    }
}

// ----------------------------------------------------------------------------

inline
kuhl::test::testcase
kuhl::test::expect_success(char const* name, bool (*test)())
{
    return kuhl::test::testcase(true, name, test);
}

inline
kuhl::test::testcase
kuhl::test::expect_success(char const* name, bool (*test)(kuhl::test::context&))
{
    return kuhl::test::testcase(true, name, test);
}

inline
kuhl::test::testcase
kuhl::test::expect_failure(char const* name, bool (*test)())
{
    return kuhl::test::testcase(false, name, test);
}

inline
kuhl::test::testcase
kuhl::test::expect_failure(char const* name, bool (*test)(kuhl::test::context&))
{
    return kuhl::test::testcase(false, name, test);
}

template <typename Exception>
kuhl::test::testcase
kuhl::test::expect_exception(char const* name, bool(*test)())
{
    return kuhl::test::testcase(static_cast<Exception*>(0), name, test);
}

template <typename Exception>
kuhl::test::testcase
kuhl::test::expect_exception(char const* name, bool(*test)(kuhl::test::context&))
{
    return kuhl::test::testcase(static_cast<Exception*>(0), name, test);
}

template <typename E, typename... M>
kuhl::test::testcase
kuhl::test::check_enum(char const* name, M&&...)
{
    return kuhl::test::testcase(true, name,[]{
            return (kuhl::test::detail::is_same_type_v<E, M> && ...);
    });
}

template <std::size_t NoTests>
int
kuhl::test::run_tests(char const* name, int ac, char* av[],
                     kuhl::test::testcase const (&tests)[NoTests])
{
    return kuhl::test::run_tests(name, ac, av, tests, tests + NoTests);
}

// ----------------------------------------------------------------------------

template <typename T>
bool kuhl::test::use(T const&)
{
    return true;
}

// ----------------------------------------------------------------------------

#endif
