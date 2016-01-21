#  Makefile                                                      -*-makefile-*-
#  ----------------------------------------------------------------------------
#   Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

NAME     = entry
LIBRARY  = sorting
COMPILER = gcc

LDFLAGS += -L$(BINDIR)
LDLIBS  += -l$(LIBRARY)

ifeq ($(COMPILER),gcc)
    CXXPATH  = /opt/gcc-4.9.2
    CXX      = $(CXXPATH)/bin/g++
    CPPFLAGS = -std=c++14
    CXXFLAGS = -W -Wall -O3 $(CPPFLAGS)
    # CXXFLAGS += -fprofile-arcs -ftest-coverage
    DEPFLAGS = -M -MT '$(CXXFILES:%.cpp=$(BINDIR)/%.o)'
    LDFLAGS  += -lgcov
endif
ifeq ($(COMPILER),clang)
    CXXPATH  = /opt/llvm-current
    CXX      = $(CXXPATH)/bin/clang++
    CPPFLAGS = -std=c++14
    CXXFLAGS = -W -Wall -O3 $(CPPFLAGS)
    DEPFLAGS = -M -MT '$(CXXFILES:%.cpp=$(BINDIR)/%.o)'
endif
ifeq ($(COMPILER),intel)
    CXXPATH  = /opt/intel/composerxe
    CXX      = . $(CXXPATH)/bin/compilervars.sh intel64; icc
    CPPFLAGS = -std=c++14
    CXXFLAGS = -O3 $(CPPFLAGS)
    DEPFLAGS = -M -MT '$(CXXFILES:%.cpp=$(BINDIR)/%.o)'
endif

AR      = ar
ARFLAGS = rcu

LIBCXXFILES = \
	timer.cpp \

LIBOFILES   =  $(LIBCXXFILES:%.cpp=$(BINDIR)/%.o)

CXXFILES = \
	$(LIBCXXFILES) \
	$(NAME).cpp \

BINDIR = bin/$(COMPILER)
TASK   = $(BINDIR)/$(NAME).tsk

default: $(TASK)
	DYLD_LIBRARY_PATH=$(CXXPATH)/lib:$$DYLD_LIBRARY_PATH ./$(TASK)

$(BINDIR)/%.tsk: $(BINDIR)/%.o $(BINDIR)/lib$(LIBRARY).a
	$(CXX) $(LDFLAGS) -o $@ $(@:%.tsk=%.o) $(LDLIBS)

$(BINDIR)/lib$(LIBRARY).a: $(LIBOFILES)
	$(AR) $(ARFLAGS) $@ $(LIBOFILES)

$(BINDIR)/%.o: %.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $(@:$(BINDIR)/%.o=%.cpp)

.PHONY: depend
.PHONY: $(BINDIR)/make.depend
depend $(BINDIR)/make.depend:
	@mkdir -p $(BINDIR)
	$(CXX) > $(BINDIR)/make.depend $(CPPFLAGS) $(DEPFLAGS) $(CXXFILES)

.PHONY: clean
clean:
	$(RM) -rf $(BINDIR)
	$(RM) *~
	$(RM) mkerr olderr

.PHONY: distclean
distclean: clean
	$(RM) -rf bin

include $(BINDIR)/make.depend
