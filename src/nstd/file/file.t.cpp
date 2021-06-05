// nstd/file/file.t.cpp                                               -*-C++-*-
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

#include "nstd/file/file.hpp"
#include "nstd/net/io_context.hpp"
#include "nstd/sender/just.hpp"
#include "nstd/sender/then.hpp"
#include <iostream>
#include "kuhl/test.hpp"

namespace NET = ::nstd::net;
namespace KT  = ::kuhl::test;

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("file usage", []{
            NET::io_context ctxt;
            auto x = NET::just("/dev/null")
#if 0
                   | NET::then([](auto name){
                       ::std::cout << "name='" << name << "'\n";
                       return name;
                     })
                   | NET::file_in()
                   | NET::on_error([](auto const& name, auto const& error){
                       ::std::cout << "open filed, name='" << name < "', error=" << error << "\n";
                     })
                   | NET::read()
                   | NET::then([](auto buffer){
                       ::std::cout << "read='" << buffer << "'\n" << ::std::flush;
                     })
                   | NET::then([]{ ::std::cout << "done\n"; })
#endif
                   ;
#if 1
            KT::use(x);
#else
            NET::async_wait(ctxt, x);
#endif
            return true;
        }),
    KT::expect_failure("placeholder", [](KT::context& )->bool{
           return false;
        }),
};

static KT::add_tests suite("file", ::tests);