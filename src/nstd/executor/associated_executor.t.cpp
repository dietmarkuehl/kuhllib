// nstd/executor/associated_executor.t.cpp                            -*-C++-*-
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

#include "nstd/executor/associated_executor.hpp"
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace KT  = ::kuhl::test;
namespace test_declarations {}
namespace TD  = test_declarations;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    struct my_executor {
        int id = 17;
        bool operator== (my_executor const&) const = default;
    };
    struct other_executor {
        int id = 18;
    };
    struct no_executor {
        no_executor() noexcept = default;
        no_executor(no_executor const&) noexcept = default;
    };
    struct has_executor {
        using executor_type = my_executor;
        auto get_executor() const { return my_executor{3}; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("associated_executor types", []{
            return KT::type<NET::system_executor>
                    == KT::type<NET::associated_executor<TD::no_executor>::type>
                && KT::type<NET::system_executor>
                    == KT::type<NET::associated_executor_t<TD::no_executor>>
                && KT::type<TD::my_executor>
                    == KT::type<NET::associated_executor<TD::no_executor, TD::my_executor>::type>
                && KT::type<TD::my_executor>
                    == KT::type<NET::associated_executor_t<TD::no_executor, TD::my_executor>>
                && KT::type<TD::my_executor>
                    == KT::type<NET::associated_executor<TD::has_executor>::type>
                && KT::type<TD::my_executor>
                    == KT::type<NET::associated_executor_t<TD::has_executor>>
                && KT::type<TD::my_executor>
                    == KT::type<NET::associated_executor<TD::has_executor, TD::other_executor>::type>
                && KT::type<TD::my_executor>
                    == KT::type<NET::associated_executor_t<TD::has_executor, TD::other_executor>>
                ;
        }),
    KT::expect_success("associated_executor members", []{
            TD::no_executor  ne;
            TD::has_executor he;
            return KT::type<NET::system_executor>
                    == KT::type<decltype(NET::associated_executor<TD::no_executor>::get(ne))>
                && noexcept(NET::associated_executor<TD::no_executor>::get(ne))
                && NET::system_executor() == NET::associated_executor<TD::no_executor>::get(ne)

                && KT::type<NET::system_executor>
                    == KT::type<decltype(NET::associated_executor<TD::no_executor>::get(ne, NET::system_executor()))>
                && noexcept(NET::associated_executor<TD::no_executor>::get(ne, NET::system_executor()))
                && NET::system_executor() == NET::associated_executor<TD::no_executor>::get(ne, NET::system_executor())

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::associated_executor<TD::no_executor, TD::my_executor>::get(ne))>
                && noexcept(NET::associated_executor<TD::no_executor, TD::my_executor>::get(ne))
                && TD::my_executor() == NET::associated_executor<TD::no_executor, TD::my_executor>::get(ne)

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::associated_executor<TD::no_executor, TD::my_executor>::get(ne, TD::my_executor{42}))>
                && noexcept(NET::associated_executor<TD::no_executor, TD::my_executor>::get(ne, TD::my_executor{42}))
                && TD::my_executor{42} == NET::associated_executor<TD::no_executor, TD::my_executor>::get(ne, TD::my_executor{42})

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::associated_executor<TD::has_executor>::get(he))>
                && noexcept(NET::associated_executor<TD::has_executor>::get(he))
                && TD::my_executor{3} == NET::associated_executor<TD::has_executor>::get(he)

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::associated_executor<TD::has_executor>::get(he, NET::system_executor()))>
                && noexcept(NET::associated_executor<TD::has_executor>::get(he, NET::system_executor()))
                && TD::my_executor{3} == NET::associated_executor<TD::has_executor>::get(he, NET::system_executor())

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::associated_executor<TD::has_executor, TD::other_executor>::get(he))>
                && noexcept(NET::associated_executor<TD::has_executor, TD::other_executor>::get(he))
                && TD::my_executor{3} == NET::associated_executor<TD::has_executor, TD::other_executor>::get(he)

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::associated_executor<TD::has_executor, TD::other_executor>::get(he, TD::other_executor{42}))>
                && noexcept(NET::associated_executor<TD::has_executor, TD::other_executor>::get(he, TD::other_executor{42}))
                && TD::my_executor{3} == NET::associated_executor<TD::has_executor, TD::other_executor>::get(he, TD::other_executor{42})
                ;
        }),
    KT::expect_success("get_associated_executor", []{
            TD::no_executor  ne;
            TD::has_executor he;
            return KT::type<NET::system_executor>
                    == KT::type<decltype(NET::get_associated_executor(ne))>
                && noexcept(NET::get_associated_executor(ne))
                && NET::system_executor() == NET::get_associated_executor(ne)

                && KT::type<NET::system_executor>
                    == KT::type<decltype(NET::get_associated_executor(ne, NET::system_executor()))>
                && noexcept(NET::get_associated_executor(ne, NET::system_executor()))
                && NET::system_executor() == NET::get_associated_executor(ne, NET::system_executor())

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::get_associated_executor(ne, TD::my_executor{42}))>
                && noexcept(NET::get_associated_executor(ne, TD::my_executor{42}))
                && TD::my_executor{42} == NET::get_associated_executor(ne, TD::my_executor{42})

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::get_associated_executor(he))>
                && noexcept(NET::get_associated_executor(he))
                && TD::my_executor{3} == NET::get_associated_executor(he)

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::get_associated_executor(he, NET::system_executor()))>
                && noexcept(NET::get_associated_executor(he, NET::system_executor()))
                && TD::my_executor{3} == NET::get_associated_executor(he, NET::system_executor())

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::get_associated_executor(he))>
                && noexcept(NET::get_associated_executor(he))
                && TD::my_executor{3} == NET::get_associated_executor(he)

                && KT::type<TD::my_executor>
                    == KT::type<decltype(NET::get_associated_executor(he, TD::other_executor{42}))>
                && noexcept(NET::get_associated_executor(he, TD::other_executor{42}))
                && TD::my_executor{3} == NET::get_associated_executor(he, TD::other_executor{42})
                ;
        }),
};

static KT::add_tests suite("associated_executor", ::tests);
