// -*-c++-*- blog/endl.cpp 
// -----------------------------------------------------------------------------
// Copyright 2012 Dietmar Kuehl http://www.dietmar-kuehl.de              
// Distributed under the Boost Software License, Version 1.0. (See file  
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).     
// -----------------------------------------------------------------------------
// <http://kuhllib.com/2012/01/14/stop-excessive-use-of-stdendl/>

#include <fstream>
#include <string>

template <typename cT, typename Traits>
std::basic_ostream<cT, Traits>&
nl(std::basic_ostream<cT, Traits>& out)
{
    return out << out.widen('\n');
}

int main(int ac, char*[])
{
    std::string   text("0123456789012345678901234567890123456789");
    std::ostream& (*end)(std::ostream&) = std::endl;
    if (ac == 1)
        end = ::nl;

    std::ofstream out("file.txt", std::ios_base::binary);
    for (size_t i(0); i != 1000000; ++i)
    {
        out << text << text << end;
    }
}
