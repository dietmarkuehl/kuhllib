// uint_t.t.cpp                                                       -*-C++-*-
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

#include "uint_t.hpp"
#include "erltest_test.hpp"

// ----------------------------------------------------------------------------

static erl::test::testcase const tests[] = {
    erl::test::expect_success("breezing", []()->bool{
            constexpr kuhllib::uint_t ud{};
            constexpr kuhllib::uint_t u0{0u};
            return true;
        }),
    erl::test::expect_success("initialization", []()->bool{
            constexpr kuhllib::uint_t ud{};
            constexpr kuhllib::uint_t u1(1u);
            constexpr kuhllib::uint_t u12(1u, 2u);
            return ud  == kuhllib::uint_t(0u, 0u)
                && u1  == kuhllib::uint_t(1u, 0u)
                && u12 == kuhllib::uint_t(1u, 2u);
        }),
    erl::test::expect_success("equality", []()->bool {
            constexpr bool rc{kuhllib::uint_t(1) == kuhllib::uint_t(1, 0)};
            return rc
                && kuhllib::uint_t() == kuhllib::uint_t()
                && kuhllib::uint_t(0x12345) == kuhllib::uint_t(0x12345)
                && !(kuhllib::uint_t(0x12345) == kuhllib::uint_t(0x12245))
                && kuhllib::uint_t(0x12345, 0x6789) == kuhllib::uint_t(0x12345, 0x6789)
                && !(kuhllib::uint_t(0x12345, 0x6689) == kuhllib::uint_t(0x12345, 0x6789))
                && !(kuhllib::uint_t(0x12245, 0x6789) == kuhllib::uint_t(0x12345, 0x6789))
                ;
        }),
    erl::test::expect_success("inequality", []()->bool {
            constexpr bool rc{kuhllib::uint_t(1) != kuhllib::uint_t(1, 1)};
            return rc
                && !(kuhllib::uint_t() != kuhllib::uint_t())
                && !(kuhllib::uint_t(0x12345) != kuhllib::uint_t(0x12345))
                && kuhllib::uint_t(0x12345) != kuhllib::uint_t(0x12245)
                && !(kuhllib::uint_t(0x12345, 0x6789) != kuhllib::uint_t(0x12345, 0x6789))
                && kuhllib::uint_t(0x12345, 0x6689) != kuhllib::uint_t(0x12345, 0x6789)
                && kuhllib::uint_t(0x12245, 0x6789) != kuhllib::uint_t(0x12345, 0x6789)
                ;
        }),
    erl::test::expect_success("invert", []()->bool {
            constexpr kuhllib::uint_t all(~kuhllib::uint_t{});
            return all == kuhllib::uint_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull)
                ;
        }),
    erl::test::expect_success("left shift", []()->bool {
            constexpr kuhllib::uint_t u1(1u);
            constexpr kuhllib::uint_t us{u1 << 5};
            constexpr kuhllib::uint_t um(0x8000000000000000ull);
            for (unsigned int step(0); step != 64; ++step) {
                kuhllib::uint_t result(u1 << step);
                if (result != kuhllib::uint_t(0x1ull << step, 0u)) {
                    return false;
                }
            }
            for (unsigned int step(1); step <= 64; ++step) {
                kuhllib::uint_t result(um << step);
                if (result != kuhllib::uint_t(0u, 0x1ull << (step - 1u))) {
                    return false;
                }
            }
            for (unsigned int step(64); step != 128; ++step) {
                kuhllib::uint_t result(u1 << step);
                if (result != kuhllib::uint_t(0u, 0x1ull << (step - 64u))) {
                    return false;
                }
            }
            return us == kuhllib::uint_t(32u, 0u);
        }),
    erl::test::expect_success("right shift", []()->bool {
            constexpr kuhllib::uint_t u1(0u, 1u);
            constexpr kuhllib::uint_t um(0u, 0x8000000000000000ull);
            constexpr kuhllib::uint_t us{um >> 5};
            for (unsigned int step(0); step != 64; ++step) {
                kuhllib::uint_t result(um >> step);
                if (result != kuhllib::uint_t(0u, 0x8000000000000000ull >> step)) {
                    return false;
                }
            }
            for (unsigned int step(1); step <= 64; ++step) {
                kuhllib::uint_t result(u1 >> step);
                if (result != kuhllib::uint_t(0x8000000000000000ull >> (step - 1u), 0u)) {
                    return false;
                }
            }
            for (unsigned int step(64); step != 128; ++step) {
                kuhllib::uint_t result(um >> step);
                if (result != kuhllib::uint_t(0x8000000000000000ull >> (step - 64), 0u)) {
                    return false;
                }
            }
            return us == kuhllib::uint_t(0u, 0x0400000000000000ull);
        }),
    erl::test::expect_success("literal", []()->bool {
            using namespace kuhllib;
            return kuhllib::uint_t() == 0_uLL
                && kuhllib::uint_t(9u) == 9_uLL
                && kuhllib::uint_t(15u) == 0xf_uLL
                ;
        }),
};

int main(int ac, char* av[])
{
    using namespace kuhllib;
    std::cout << "9_uLL=" << 9_uLL << '\n';
    std::cout << "0xf_uLL=" << 0xf_uLL << '\n';
    std::cout << "0xff_uLL=" << 0xff_uLL << '\n';
    std::cout << "1234567890_uLL=" << 1234567890_uLL << '\n';
    std::cout << "12345678901234567890_uLL=" << 12345678901234567890_uLL << '\n';
    std::cout << std::oct << "0x123456789abcdef0_uLL=" << 0x123456789abcdef0_uLL << '\n';
    std::cout << std::hex << "0x123456789abcdef0_uLL=" << 0x123456789abcdef0_uLL << '\n';
    return erl::test::run_tests("uint_t", ac, av, ::tests);
}
