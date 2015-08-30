// dec-test.cpp -*-C++-*-
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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstddef>

static bool runTest(std::string const& file);

// ----------------------------------------------------------------------------

namespace
{
    struct test_context
    {
        enum e_flags {
            precision_set    = 0x1,
            rounding_set     = 0x2,
            max_exponent_set = 0x4,
            min_exponent_set = 0x8,
            complete         = 0xf
        };
        e_flags      flags;
        unsigned int precision;
        std::string  rounding;
        unsigned int max_exponent;
        unsigned int min_exponent;
        std::string  version;
        int          extended;
        int          clamp;
        test_context()
            : flags()
            , extended(1)
            , clamp(0)
        {
        }
    };
}

// ----------------------------------------------------------------------------

static std::ostream&
print(std::ostream& out, std::vector<std::string> const& tokens)
{
    for (auto&& token: tokens) {
        out << " '" << token << "'";
    }
    return out;
}

// ----------------------------------------------------------------------------

static std::vector<std::string>
tokenize(std::string const& line) {
    std::vector<std::string> rc;
    std::string::size_type pos(0);
    std::string spaces(" \t\r");
    while ((pos = line.find_first_not_of(spaces, pos)) != line.npos
           && pos != line.find("--", pos)) {
        if (line[pos] == '"' || line[pos] == '\'') {
            char quote(line[pos]);
            std::string::size_type end(line.find(quote, pos + 1));
            rc.push_back(line.substr(pos, end - pos));
            rc.back()[0] = '"';
            pos = end + 1;
            while (pos < line.size() && line[pos] == quote) {
                std::string::size_type end(line.find(quote, pos + 1));
                rc.back() += line.substr(pos, end - pos);
                pos = end + 1;
            }
        }
        else {
            std::string::size_type end(line.find_first_of(spaces, pos));
            rc.push_back(line.substr(pos, end - pos));
            pos = end;
        }
    }
    return rc;
}

// ----------------------------------------------------------------------------

static bool
directive(test_context& context, std::vector<std::string> const& tokens) {
    std::string directive(tokens.front());
    std::transform(directive.begin(), directive.end(),
                   directive.begin(),
                   [](unsigned char c)->char{ return std::tolower(c); });
    if (tokens.size() != 2) {
        print(std::cerr << "invalid directive: ", tokens) << '\n';
        return false;
    }
    else if (directive == "version:") {
        context.version = tokens[1];
    }
    else if (directive == "precision:") {
        context.precision = std::stoi(tokens[1]);
        context.flags     = test_context::e_flags(context.flags | context.precision_set);
    }
    else if (directive == "maxexponent:") {
        context.max_exponent = std::stoi(tokens[1]);
        context.flags        = test_context::e_flags(context.flags | context.max_exponent_set);
    }
    else if (directive == "minexponent:") {
        context.min_exponent = std::stoi(tokens[1]);
        context.flags        = test_context::e_flags(context.flags | context.min_exponent_set);
    }
    else if (directive == "rounding:") {
        context.rounding = tokens[1];
        context.flags    = test_context::e_flags(context.flags | context.rounding_set);
    }
    else if (directive == "extended:") {
        context.extended = std::stoi(tokens[1]);
    }
    else if (directive == "clamp:") {
        context.clamp = std::stoi(tokens[1]);
    }
    else if (directive == "dectest:") {
        std::cout << "running " << tokens[1] << '\n';
        return runTest(tokens[1]);
    }
    else {
        print(std::cout << "unknown directive:", tokens) << '\n';
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

static bool
runTest(std::string const& file)
{
    std::ifstream in("test/" + file + ".decTest");
    if (!in) {
        std::cerr << "ERROR: failed to open file '" << file << "' for reading\n";
        return false;
    }
    test_context context;

    bool rc(true);

    for (std::string line; std::getline(in >> std::ws, line); ) {
        std::vector<std::string> tokens(tokenize(line));
        if (!tokens.empty()) {
            if (tokens.front().back() == ':') {
                rc = directive(context, tokens) && rc;
            }
            else {
                if (context.precision == 7) {
                    print(std::cout << "testcase(d32): ", tokens) << '\n'; 
                }
                else if (context.precision == 16) {
                    print(std::cout << "testcase(d64): ", tokens) << '\n'; 
                }
                else if (context.precision == 34) {
                    print(std::cout << "testcase(d128): ", tokens) << '\n'; 
                }
                else {
                    //print(std::cout << "testcase ", tokens)<< " ignored: precision==" << context.precision << "\n";
                }
            }
        }
    }
    return rc;
}

// ----------------------------------------------------------------------------

int
main(int ac, char* av[])
{
    if (ac < 2) {
        std::cerr << "usage: " << av[0] << " <test-case>+\n";
        return EXIT_FAILURE;
    }

    bool result(true);
    for (int i(1); i != ac; ++i) {
        std::cout << "running " << av[i] << " (toplevel)\n";
        result = runTest(av[i]) && result;
    }
    return result? EXIT_SUCCESS: EXIT_FAILURE;
}
