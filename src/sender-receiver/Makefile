#  Makefile                                                      -*-makefile-*-
#  ----------------------------------------------------------------------------
#   Copyright (C) 2020 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

LIB        = sender
NAME       = basic
COMPONENTS = \
	execution/connect         \
	execution/operation_state \
	execution/sender          \
	execution/sender_base     \
	execution/sender_traits   \
	execution/set_done        \
	execution/set_error       \
	execution/set_value       \
	execution/start           \

CXXFILES = src/$(NAME).cpp $(COMPONENTS:%=src/%.cpp)

#  ----------------------------------------------------------------------------

ifeq ($(COMPILER),gcc)
   VERSION = 10.1.0
   PREFIX   = /opt/gcc-$(VERSION)
   CPPFLAGS = -std=c++2a
   CXXFLAGS = -O3
   CXX      = $(PREFIX)/bin/g++
   CC       = $(PREFIX)/bin/gcc
endif
ifeq ($(COMPILER),clang)
   CPPFLAGS = -std=c++17
   CXXFLAGS = -O3
   CXX      = /usr/bin/clang++
endif
ifeq ($(COMPILER),intel)
   CXXFLAGS  = -std=c++17 -O2
   #LDFLAGS   = -stdlib=libstdc++
   CXX       = /opt/intel/bin/icc
endif
ifeq ($(COMPILER),edg)
   CXXFLAGS  = --c++20  -O2
   CXX       = /opt/edg/bin/eccp
endif

CPPFLAGS += -Iinclude
CXXFLAGS += $(CPPFLAGS)
BUILD    = bin/build-$(COMPILER)-$(VERSION)

#  ----------------------------------------------------------------------------

.PHONY: default
default: $(BUILD)/$(NAME)
	$(BUILD)/$(NAME)

.PHONY: install
install: build
	cd $(BUILD); sudo cp source/libunifex.a $(PREFIX)/lib
	cd libunifex; tar c include | (cd $(PREFIX); sudo tar x)

.PHONY: build
build: $(BUILD)/Makefile
	cd $(BUILD); make

$(BUILD)/Makefile: configure

.PHONY: configure
configure: libunifex update
	@mkdir -p $(BUILD)
	if grep -q 'add_subdirectory(examples)' libunifex/CMakeLists.txt; \
	then \
		echo sed -i .orig -e /add_subdirectory.examples./d libunifex/CMakeLists.txt; \
	fi
	cd $(BUILD); \
		cmake -DCMAKE_BUILD_TYPE=Release \
			$(SANITIZER) \
			-DCMAKE_INSTALL_PREFIX=$(PREFIX) \
			-DCMAKE_CXX_COMPILER=$(CXX) \
			-DCMAKE_CXX_FLAGS="$(CXXFLAGS)" \
			-DCMAKE_EXE_LINKER_FLAGS="$(LDFLAGS)" \
			../libunifex

.PHONY: update
update: libunifex
	cd libunifex; echo git pull origin master

libunifex:
	git clone https://github.com/facebookexperimental/libunifex.git

.PHONY: $(NAME)
$(NAME): $(BUILD)/$(NAME)

$(BUILD)/$(NAME): $(BUILD)/lib$(LIB).a $(BUILD)/$(NAME).o
	$(CXX) $(LDFLAGS) -o $@ $@.o $(BUILD)/lib$(LIB).a $(LDLIBS)

$(BUILD)/lib$(LIB).a: $(COMPONENTS:%=$(BUILD)/%.o)
	ar rcu $@ $(COMPONENTS:%=$(BUILD)/%.o)
	ranlib $@

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $(@:$(BUILD)/%.o=src/%.cpp)

.PHONY: clean
clean:
	$(RM) -r $(BUILD)
	$(RM) mkerr olderr

.PHONY: depend
depend $(BUILD)/make.depend:
	@mkdir -p $(BUILD)
	$(CXX) $(CPPFLAGS) -M $(CXXFILES) | sed -e 's@\(.*\.o:\)@$(BUILD)/\1@' > $(BUILD)/make.depend

include $(BUILD)/make.depend
