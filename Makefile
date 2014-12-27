#  Makefile                                                      -*-makefile-*-
#  ----------------------------------------------------------------------------
#   Copyright (C) 2014 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

COMPILER = unknown
BUILDDIR = build-$(COMPILER)
ifneq ($(CMAKE_CXX),)
    CMAKE_CXX_ARG = -DCMAKE_CXX_COMPILER=$(CMAKE_CXX)
endif

.PHONY: default
default: check

.PHONY: cmake
cmake:
	@mkdir -p $(BUILDDIR)
	cd $(BUILDDIR); cmake $(CMAKE_CXX_ARG) ../src

.PHONY: build
build: cmake
	cd $(BUILDDIR); $(MAKE)

.PHONY: verbose
verbose: cmake
	cd $(BUILDDIR); $(MAKE) VERBOSE=1

.PHONY: check
check: build test

.PHONY: test
test:
	cd $(BUILDDIR); $(MAKE) test

.PHONY: all
all:
	-$(MAKE)
	-$(MAKE) COMPILER=clang CMAKE_CXX_ARG=/opt/llvm-current/bin/clang++
	-$(MAKE) COMPILER=gcc CMAKE_CXX_ARG=/opt/gcc-4.9.2/bin/g++
	-$(MAKE) COMPILER=intel CMAKE_CXX_ARG=/opt/intel/bin/icc

.PHONY: clean
clean:
	$(RM) -r $(BUILDDIR)
	$(RM) *~ mkerr olderr

.PHONY: clean-all
clean-all:
	$(MAKE) clean
	$(MAKE) COMPILER=clang clean
	$(MAKE) COMPILER=gcc clean
	$(MAKE) COMPILER=intel clean

