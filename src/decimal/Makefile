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

COMPILER = clang
SYSTEM = $(shell uname -s)

OBJ = bin/$(SYSTEM)-$(COMPILER)

CPPFLAGS = -std=c++11
CXXFLAGS = -W -Wall
LDFLAGS  = -L$(OBJ)

ifeq ($(COMPILER),clang)
    CXX = /opt/llvm-current/bin/clang++
    DEPFLAGS = -M
endif
ifeq ($(COMPILER),gcc)
    CXX = /opt/gcc-current/bin/g++
    DEPFLAGS = -M
endif
CC = $(CXX)

LIBCXXFILES += \
	decimal.cpp \
	erltest_test.cpp \

TSTCXXFILES += \
	decimal.t.cpp \
	decimal_config.t.cpp \
	uint128_t.t.cpp \

CXXFILES = \
	$(LIBCXXFILES) \
	$(TSTCXXFILES) \
	erltest_test.cpp \

LIBOFILES = $(LIBCXXFILES:%.cpp=$(OBJ)/%.o)

default: check-all

check-all:
	@for t in $(TSTCXXFILES:%.cpp=%); \
	do \
	    $(MAKE) check NAME=$$t; \
	done

check: $(OBJ)/$(NAME)
	$(OBJ)/$(NAME)

$(OBJ)/decimal.t: $(OBJ)/libdec.a $(OBJ)/decimal.t.o
	$(CXX) -o $@ $(LDFLAGS) $(OBJ)/decimal.t.o -ldec

$(OBJ)/libdec.a: $(LIBOFILES)
	ar rcu $@ $(LIBOFILES)

$(OBJ)/%.o: %.cpp
	@mkdir -p $(OBJ)
	$(CXX) -o $@ -c $(CPPFLAGS) $(CXXFLAGS) $(@:$(OBJ)/%.o=%.cpp)

clean:
	$(RM) $(LIBOFILES) $(OBJ)/decimal.t.o $(OBJ)/decimal.t $(OBJ)/libdec.a
	$(RM) make.depend
	$(RM) mkerr olderr *~

depend $(OBJ)/make.depend:
	@mkdir -p $(OBJ)
	$(CXX) $(CPPFLAGS) $(DEPFLAGS) $(CXXFILES) | fixdepend $(OBJ) > $(OBJ)/make.depend

include $(OBJ)/make.depend
