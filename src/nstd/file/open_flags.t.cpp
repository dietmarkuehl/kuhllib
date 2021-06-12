// nstd/file/open_flags.t.cpp                                         -*-C++-*-
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

#include "nstd/file/open_flags.hpp"
#include "kuhl/test.hpp"
#include <array>
#include <fcntl.h>

namespace test_declarations {}
namespace File = ::nstd::file;
namespace TD   = test_declarations;
namespace KT   = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace test_declarations
{
    constexpr ::std::array flags{
        File::open_flags::in,
        File::open_flags::rdonly,
        File::open_flags::out,
        File::open_flags::wronly,
        File::open_flags::inout,
        File::open_flags::rdwr,

        File::open_flags::append,
        File::open_flags::async,
        File::open_flags::cloexec,
        File::open_flags::close_exec,
        File::open_flags::create,
        File::open_flags::direct,
        File::open_flags::directory,
        File::open_flags::dsync,
        File::open_flags::excl,
        File::open_flags::largefile,
        File::open_flags::noattime,
        File::open_flags::noctty,
        File::open_flags::nofollow,
        File::open_flags::nonblock,
        File::open_flags::ndelay,
        File::open_flags::path,
        File::open_flags::sync,
        File::open_flags::tmpfile,
        File::open_flags::trunc,
        };
}

static KT::testcase const tests[] = {
    KT::expect_success("open_flags members", [](KT::context& )->bool{
            // there are at least members for in, out, inout
            return 3u <= TD::flags.size();
        }),
    KT::expect_success("basic open_flags conversion", []{
            return File::to_system(File::open_flags::in) == O_RDONLY
                && File::to_system(File::open_flags::out) == O_WRONLY
                && File::to_system(File::open_flags::inout) == O_RDWR
                ;
        }),
    KT::expect_success("aliases compare equal", []{
            return File::open_flags::in      == File::open_flags::rdonly
                && File::open_flags::out     == File::open_flags::wronly
                && File::open_flags::inout   == File::open_flags::rdwr
                && File::open_flags::cloexec == File::open_flags::close_exec
                && File::open_flags::ndelay  == File::open_flags::nonblock
                ;
        }),
    KT::expect_success("bitwise non-assignment operators functionality", []{
            for (auto flag: TD::flags) {
                if (flag != File::open_flags::none
                    && (   (flag &  File::open_flags::none) !=  File::open_flags::none
                        || (flag &  File::open_flags::all)  !=  flag
                        || (flag & ~File::open_flags::none) !=  flag
                        || (flag &  flag)                   !=  flag
                        || (flag |  File::open_flags::none) !=  flag
                        || (flag |  File::open_flags::all)  !=  File::open_flags::all
                        || (flag | ~File::open_flags::none) != ~File::open_flags::none
                        || (flag |  flag)                   !=  flag
                        || (flag ^  File::open_flags::none) !=  flag
                        || (flag ^  File::open_flags::all)  !=  (~flag & File::open_flags::all)
                        || (flag ^  flag)                   !=  File::open_flags::none
                    )) {
                        return false;
                }
            }
            return true;
        }),
    KT::expect_success("bitwise assignment operators functionality", []{
            for (auto flag: TD::flags) {
                if (flag != File::open_flags::none
                    && (   [flag          ]()mutable{ flag &=  File::open_flags::none; return flag != File::open_flags::none;         }()
                        || [flag, tmp=flag]()mutable{ flag &=  File::open_flags::all;  return flag != tmp;                            }()
                        || [flag, tmp=flag]()mutable{ flag &= ~File::open_flags::none; return flag != tmp;                            }()
                        || [flag, tmp=flag]()mutable{ auto x(flag); flag &=  x;        return flag != tmp;                            }()
                        || [flag, tmp=flag]()mutable{ flag |=  File::open_flags::none; return flag != tmp;                            }()
                        || [flag          ]()mutable{ flag |=  File::open_flags::all;  return flag != File::open_flags::all;          }()
                        || [flag          ]()mutable{ flag |= ~File::open_flags::none; return flag != ~File::open_flags::none;        }()
                        || [flag, tmp=flag]()mutable{ auto x(flag); flag |=  x;        return flag != tmp;                            }()
                        || [flag, tmp=flag]()mutable{ flag ^=  File::open_flags::none; return flag != tmp;                            }()
                        || [flag, tmp=flag]()mutable{ flag ^=  File::open_flags::all;  return flag != (~tmp & File::open_flags::all); }()
                        || [flag          ]()mutable{ auto x(flag); flag ^=  x;        return flag != File::open_flags::none;         }()
                    )) {
                        return false;
                }
            }
            return true;
        }),
};

static KT::add_tests suite("open_flags", ::tests);
