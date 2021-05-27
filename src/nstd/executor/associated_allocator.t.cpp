// nstd/executor/associated_allocator.t.cpp                           -*-C++-*-
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

#include "nstd/executor/associated_allocator.hpp"
#include <memory>
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace MEM = ::std;
namespace test_definitions {}
namespace TD  = ::test_definitions;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_definitions
{
    struct my_allocator {
        int id = 17;
        bool operator== (my_allocator const&) const = default;
    };

    class no_allocator {};
    class has_allocator {
    public:
        using allocator_type = my_allocator;
        my_allocator get_allocator() const noexcept { return my_allocator{13}; }
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("associated_allocator types", []{
            return KT::type<MEM::allocator<void>>
                    == KT::type<NET::associated_allocator<TD::no_allocator>::type>
                && KT::type<MEM::allocator<int>>
                    == KT::type<NET::associated_allocator<TD::no_allocator, MEM::allocator<int>>::type>
                && KT::type<TD::my_allocator>
                    == KT::type<NET::associated_allocator<TD::has_allocator>::type>
                && KT::type<TD::my_allocator>
                    == KT::type<NET::associated_allocator<TD::has_allocator, MEM::allocator<int>>::type>
                && KT::type<MEM::allocator<void>>
                    == KT::type<NET::associated_allocator_t<TD::no_allocator>>
                && KT::type<MEM::allocator<int>>
                    == KT::type<NET::associated_allocator_t<TD::no_allocator, MEM::allocator<int>>>
                && KT::type<TD::my_allocator>
                    == KT::type<NET::associated_allocator_t<TD::has_allocator>>
                && KT::type<TD::my_allocator>
                    == KT::type<NET::associated_allocator_t<TD::has_allocator, MEM::allocator<int>>>
                ;
        }),
    KT::expect_success("associated_allocator member", []{
            TD::no_allocator  na;
            TD::has_allocator ha;
            return KT::type<MEM::allocator<void>>
                    == KT::type<decltype(NET::associated_allocator<TD::no_allocator>::get(na))>
                && noexcept(NET::associated_allocator<TD::no_allocator>::get(na))
                && KT::type<MEM::allocator<int>>
                    == KT::type<decltype(NET::associated_allocator<TD::no_allocator, MEM::allocator<int>>::get(na, MEM::allocator<int>()))>
                && noexcept(NET::associated_allocator<TD::no_allocator, MEM::allocator<int>>::get(na, MEM::allocator<int>()))
                && KT::type<TD::my_allocator>
                    == KT::type<decltype(NET::associated_allocator<TD::has_allocator>::get(ha))>
                && noexcept(NET::associated_allocator<TD::has_allocator>::get(ha))
                && KT::type<TD::my_allocator>
                    == KT::type<decltype(NET::associated_allocator<TD::has_allocator, MEM::allocator<int>>::get(ha, MEM::allocator<int>()))>
                && noexcept(NET::associated_allocator<TD::has_allocator, MEM::allocator<int>>::get(ha, MEM::allocator<int>()))
                && MEM::allocator<void>() == NET::associated_allocator<TD::no_allocator>::get(na)
                && TD::my_allocator{} == NET::associated_allocator<TD::no_allocator, TD::my_allocator>::get(na)
                && TD::my_allocator{42} == NET::associated_allocator<TD::no_allocator, TD::my_allocator>::get(na, TD::my_allocator(42))
                && TD::my_allocator{13} == NET::associated_allocator<TD::has_allocator>::get(ha)
                && TD::my_allocator{13} == NET::associated_allocator<TD::has_allocator, TD::my_allocator>::get(ha, TD::my_allocator(42))
                ;
        }),
    KT::expect_success("get_associated_allocator", []{
            TD::no_allocator  na;
            TD::has_allocator ha;
            return KT::type<MEM::allocator<void>>
                    == KT::type<decltype(NET::get_associated_allocator(na))>
                && noexcept(NET::get_associated_allocator(na))
                && KT::type<MEM::allocator<int>>
                    == KT::type<decltype(NET::get_associated_allocator(na, MEM::allocator<int>()))>
                && noexcept(NET::get_associated_allocator(na, MEM::allocator<int>()))
                && KT::type<TD::my_allocator>
                    == KT::type<decltype(NET::get_associated_allocator(ha))>
                && noexcept(NET::get_associated_allocator(ha))
                && KT::type<TD::my_allocator>
                    == KT::type<decltype(NET::get_associated_allocator(ha, MEM::allocator<int>()))>
                && noexcept(NET::get_associated_allocator(ha, MEM::allocator<int>()))
                && MEM::allocator<void>() == NET::get_associated_allocator(na)
                && TD::my_allocator{42} == NET::get_associated_allocator(na, TD::my_allocator(42))
                && TD::my_allocator{13} == NET::get_associated_allocator(ha)
                && TD::my_allocator{13} == NET::get_associated_allocator(ha, TD::my_allocator(42))
                ;
        }),
};

static KT::add_tests suite("associated_allocator", ::tests);
