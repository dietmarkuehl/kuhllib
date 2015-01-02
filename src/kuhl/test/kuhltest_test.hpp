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
#include "kuhl/mini/ostream.hpp"
#include "kuhl/mini/iomanip.hpp"
#include "kuhl/mini/distance.hpp"
#include "kuhl/mini/iostream.hpp"
#include <stddef.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------

namespace kuhl
{
    namespace test
    {
        class testcase;

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

        int run_tests(char const*, int, char*[],
                      kuhl::test::testcase const*, kuhl::test::testcase const*);
        template <size_t NoTests>
        int run_tests(char const*, int, char*[], 
                      kuhl::test::testcase const (&tests)[NoTests]);

        template <typename T>
        void use(T const&);
    }
}

// ----------------------------------------------------------------------------

class kuhl::test::testcase
{
private:
    char const* d_name;
    bool (*d_no_context)();
    bool (*d_context)(kuhl::test::context&);
    bool (testcase::*d_run_test)(kuhl::mini::ostream&, kuhl::test::context&) const;

    bool expect_success_with_context(kuhl::mini::ostream&      ,
                                     kuhl::test::context&) const;
    bool expect_success_without_context(kuhl::mini::ostream&      ,
                                        kuhl::test::context&) const;
    bool expect_failure_with_context(kuhl::mini::ostream&      ,
                                     kuhl::test::context&) const;
    bool expect_failure_without_context(kuhl::mini::ostream&      ,
                                        kuhl::test::context&) const;
    template <typename Exception>
    bool expect_exception_with_context(kuhl::mini::ostream&      ,
                                       kuhl::test::context&) const;
    template <typename Exception>
    bool expect_exception_without_context(kuhl::mini::ostream&      ,
                                          kuhl::test::context&) const;

public:
    testcase(bool, char const*, bool (*)());
    testcase(bool, char const*, bool (*)(kuhl::test::context&));
    template <typename Exception>
    testcase(Exception*, char const*, bool (*)());
    template <typename Exception>
    testcase(Exception*, char const*, bool (*)(kuhl::test::context&));

    bool run(kuhl::mini::ostream&, size_t, char const*) const;
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
kuhl::test::testcase::expect_success_with_context(kuhl::mini::ostream&       out,
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
kuhl::test::testcase::expect_failure_with_context(kuhl::mini::ostream&       out,
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
kuhl::test::testcase::expect_success_without_context(kuhl::mini::ostream& out,
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
kuhl::test::testcase::expect_failure_without_context(kuhl::mini::ostream&       out,
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
                                             kuhl::mini::ostream&       out,
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
                                                    kuhl::mini::ostream&       out,
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
bool
kuhl::test::testcase::run(kuhl::mini::ostream& out,
                         size_t index,
                         char const* name) const
{
    out << kuhl::mini::right << kuhl::mini::setfill(' ') << kuhl::mini::setw(3) << index << ": "
        << kuhl::mini::left  << kuhl::mini::setw(30) << name
        << kuhl::mini::setw(40) << this->d_name;
    kuhl::test::context context;
    return (this->*(this->d_run_test))(out, context);
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

// ----------------------------------------------------------------------------
//-dk:TODO: run_tests() should reside in a library
//-dk:TODO: run_tests() should evaluate its argument to decide what to do

inline
int
kuhl::test::run_tests(char const* name, int ac, char *av[],
                     kuhl::test::testcase const* begin,
                     kuhl::test::testcase const* end)
{
    ptrdiff_t success(0);
    int count(0);
    if (ac == 1) {
        count = end - begin;
        for (kuhl::test::testcase const* it(begin); it != end; ++it) {
            it->run(kuhl::mini::cout, it - begin + 1, name) && ++success;
        }
    }
    else {
        for (int i(1); i != ac; ++i) {
            int idx(atoi(av[i]));
            if (0 < idx && idx <= kuhl::mini::distance(begin, end)) {
                ++count;
                (begin + idx - 1)->run(kuhl::mini::cout, idx, name) && ++success;
            }
        }
    }
    kuhl::mini::cout << name << ": "
                     << success << "/" << count << " succeeded\n";
    return success == (end - begin)? EXIT_SUCCESS: EXIT_FAILURE;
}

template <size_t NoTests>
int
kuhl::test::run_tests(char const* name, int ac, char* av[],
                     kuhl::test::testcase const (&tests)[NoTests])
{
    return kuhl::test::run_tests(name, ac, av, tests, tests + NoTests);
}

// ----------------------------------------------------------------------------

template <typename T>
void kuhl::test::use(T const&)
{
}

// ----------------------------------------------------------------------------

#endif
