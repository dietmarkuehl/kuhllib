// nstd/file/descriptor.hpp                                           -*-C++-*-
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

#ifndef INCLUDED_NSTD_FILE_DESCRIPTOR
#define INCLUDED_NSTD_FILE_DESCRIPTOR

#include "nstd/utility/exchange.hpp"

// ----------------------------------------------------------------------------

namespace nstd::file {
    class descriptor;
}

// ----------------------------------------------------------------------------

class nstd::file::descriptor
{
private:
    int d_fd;
    int close();

public:
    explicit descriptor(int = -1);
    descriptor(descriptor const&) = delete;
    descriptor(descriptor&&);
    auto operator=(descriptor const&) -> descriptor& = delete;
    auto operator=(descriptor&&)-> descriptor&;
    ~descriptor();

    auto get() const -> int;
    explicit operator bool() const;
};

// ----------------------------------------------------------------------------

inline nstd::file::descriptor::descriptor(int fd)
    : d_fd(fd)
{
}

inline nstd::file::descriptor::descriptor(::nstd::file::descriptor&& other)
    : d_fd(::nstd::utility::exchange(other.d_fd, -1))
{
}

inline auto nstd::file::descriptor::operator=(::nstd::file::descriptor&& other)
    -> ::nstd::file::descriptor&
{
    if (this != &other) {
        this->close();
        this->d_fd = ::nstd::utility::exchange(other.d_fd, -1);
    }
    return *this;
}

inline nstd::file::descriptor::~descriptor()
{
    this->close();
}

// ----------------------------------------------------------------------------

inline auto nstd::file::descriptor::get() const -> int
{
    return this->d_fd;
}

inline nstd::file::descriptor::operator bool() const
{
    return 0 <= this->get();
}

// ----------------------------------------------------------------------------

#endif
