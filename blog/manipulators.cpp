// -*-c++-*- blog/manipulators.cpp 
// -----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// -----------------------------------------------------------------------------
// <http://kuhllib.com/2012/01/16/custom-manipulation-of-streams/>

#include <sstream>
#include <iostream>

template <typename cT, typename Traits>
std::basic_istream<cT, Traits>& comma(std::basic_istream<cT, Traits>& in)
{
    if ((in >> std::ws).peek() != in.widen(',')) {
        in.setstate(std::ios_base::failbit);
    }
    else {
        in.ignore();
    }
    return in;
}

template <char S> struct separator {};
template <typename cT, typename Traits, char S>
std::basic_istream<cT, Traits>&
operator>> (std::basic_istream<cT, Traits>& in, separator<S>)
{
    return (in >> std::ws).peek() != in.widen(S)
        ? in.setstate(std::ios_base::failbit), in: in.ignore();
}

extern separator<';'> const semicolon;

int main()
{
    if (std::istringstream("    ") >> comma
        || std::istringstream("    ;") >> comma
        || !(std::stringstream("    ,") >> comma))
    {
        std::cout << "ERROR: comma processing failed\n";
    }

    std::istringstream in("1, 2; 3 \n4, 5; 6 \n");
    for (int x, y, z; in >> x >> comma >> y >> semicolon >> z; ) {
        std::cout << "x=" << x << " y=" << y << " z=" << z << "\n";
    }
}
