#!/usr/bin/python

import sys
import re

inc_re = re.compile(r"#include <(.*).hpp>")

argv = sys.argv[1:]

def get_deps(comp):
    with open("include/" + comp + ".hpp") as file:
        for line in file:
            match = inc_re.match(line)
            if match:
                print(comp + " " + match.group(1))

for comp in sys.argv[1:]:
    get_deps(comp)
