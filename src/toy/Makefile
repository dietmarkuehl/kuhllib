#  Makefile                                                      -*-makefile-*-
#  ----------------------------------------------------------------------------
#   Copyright (C) 2022 Dietmar Kuehl http://www.dietmar-kuehl.de
#
#   Permission is hereby granted, free of charge, to any person
#   obtaining a copy of this software and associated documentation
#   files (the "Software"), to deal in the Software without restriction,
#   including without limitation the rights to use, copy, modify,
#   merge, publish, distribute, sublicense, and/or sell copies of
#   the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be
#   included in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#   OTHER DEALINGS IN THE SOFTWARE.
#  ----------------------------------------------------------------------------

NAME     = toy-sender
TECH     = poll
CC       = $(CXX)
RM       = rm -f
CPPFLAGS = -DTOY_NETWORKING_HPP='"toy-networking-$(TECH).hpp"'
CXXFLAGS = -W -Wall -std=c++20 -Wno-missing-field-initializers
SYSTEM   = $(shell uname -s)

.PHONY: default build distclean clean run
default: build

build: 
	@mkdir -p build
	cd build; cmake ..
	cmake --build build

run: build
	./build/$(NAME)

clean:
	$(RM) mkerr olderr *~

distclean: clean
	$(RM) -r build
