// uint128_t.hpp                                                      -*-C++-*-
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

#ifndef INCLUDED_UINT128
#define INCLUDED_UINT128

#include <cinttypes>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace kuhllib
{
    class uint128_t;
}

// ----------------------------------------------------------------------------

class kuhllib::uint128_t
{
private:
    std::uint64_t d_value[2];
public:
    constexpr uint128_t(): d_value{} {}
    constexpr uint128_t(std::uint64_t value): d_value{ value, 0u } {}
    constexpr uint128_t(std::uint64_t value0, std::uint64_t value1)
        : d_value{ value0, value1 } {
    }

    std::uint64_t operator[](std::size_t index) const {
        return this->d_value[index];
    }

    constexpr uint128_t operator<< (std::size_t step) const {
        return step < 64u
            ? (step == 0u
               ? *this
               : uint128_t(this->d_value[0] << step, (this->d_value[1] << step) | (this->d_value[0] >> (64u - step)))
               )
            : uint128_t(0u, this->d_value[0] << (step - 64u));
    }
    constexpr uint128_t operator>> (std::size_t step) const {
        return step < 64u
            ? (step == 0u
               ? *this
               : uint128_t((this->d_value[0] >> step) | (this->d_value[1] << (64u - step)), this->d_value[1] >> step)
               )
            : uint128_t(this->d_value[1] >> (step - 64u), 0u);
    }
};

// ----------------------------------------------------------------------------

#endif
