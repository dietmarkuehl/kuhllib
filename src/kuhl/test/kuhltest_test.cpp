// test/kuhltest_test.cpp                                             -*-C++-*-
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

#include "kuhl/test/kuhltest_test.hpp"
#include <exception>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace KT = kuhl::test;

// ----------------------------------------------------------------------------

bool kuhl::test::test_terminate(void (*fun)())
{
    constexpr int magic{42};
    switch (::pid_t id = ::fork()) {
    case -1: return false;
    case 0: {
            ::std::set_terminate([]{ ::exit(magic); });
            fun();
            ::exit(0);
        }
    default: {
        int status{};
        return id == ::waitpid(id, &status, 0)
            && WIFEXITED(status)
            && WEXITSTATUS(status) == magic
            ;
        }
    }
}

// ----------------------------------------------------------------------------

std::vector<kuhl::test::add_tests::suite>&
kuhl::test::add_tests::suites()
{
    static std::vector<suite> rc;
    return rc;
}

int kuhl::test::add_tests::run(int ac, char* av[])
{
    int rc(EXIT_SUCCESS);
    for (auto[name, begin, end]: suites())
    {
        if (KT::run_tests(name, ac, av, begin, end)) {
            rc = EXIT_FAILURE;
        }
    }
    return rc;
}

// ----------------------------------------------------------------------------

bool
kuhl::test::testcase::run(std::ostream& out,
                         std::size_t index,
                         char const* name) const
{
    out << std::right << std::setfill(' ') << std::setw(3) << index << ": "
        << std::left  << std::setw(30) << name
        << std::setw(40) << this->d_name;
    kuhl::test::context context;
    return (this->*(this->d_run_test))(out, context);
}

// ----------------------------------------------------------------------------
//-dk:TODO: run_tests() should reside in a library
//-dk:TODO: run_tests() should evaluate its argument to decide what to do

int
kuhl::test::run_tests(char const* name, int ac, char *av[],
                     kuhl::test::testcase const* begin,
                     kuhl::test::testcase const* end)
{
    std::ptrdiff_t success(0);
    int count(0);
    if (ac == 1) {
        count = end - begin;
        for (kuhl::test::testcase const* it(begin); it != end; ++it) {
            std::ostringstream out;
            if (it->run(out, it - begin + 1, name)) {
                ++success;
            }
            else {
                std::cout << out.str();
            }
        }
    }
    else {
        for (int i(1); i != ac; ++i) {
            int idx(atoi(av[i]));
            if (0 < idx && idx <= std::distance(begin, end)) {
                ++count;
                if (false/*verbose*/) {
                    (begin + idx - 1)->run(std::cout, idx, name) && ++success;
                }
                else {
                    std::ostringstream out;
                    if ((begin + idx - 1)->run(out, idx, name)) {
                        ++success;
                    }
                    else {
                        std::cout << out.str();
                    }
                }
            }
        }
    }
    if (false) {
        std::cout << name << ": "
                  << success << "/" << count << " succeeded\n";
    }
    return success == (end - begin)? EXIT_SUCCESS: EXIT_FAILURE;
}

// ----------------------------------------------------------------------------

int main(int ac, char* av[])
{
    return kuhl::test::add_tests::run(ac, av);
}
