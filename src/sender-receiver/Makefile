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

NAME       = basic
NAME       = run-timer

LIB        = sender
COMPONENTS =                                \
	concepts                            \
	execution                           \
	execution/as_invocable              \
	execution/bulk_execute              \
	execution/connect                   \
	execution/execute                   \
	execution/executor                  \
	execution/executor_of               \
	execution/executor_of_impl          \
	execution/has_sender_types          \
	execution/operation_state           \
	execution/receiver                  \
	execution/receiver_invocation_error \
	execution/receiver_of               \
	execution/schedule                  \
	execution/scheduler                 \
	execution/sender                    \
	execution/sender_base               \
	execution/sender_to                 \
	execution/sender_traits             \
	execution/set_done                  \
	execution/set_error                 \
	execution/set_value                 \
	execution/start                     \
	execution/submit                    \
	execution/typed_sender              \
	executor                            \
	executor/execution_context          \
	io_context                          \
	timer                               \

CXXFILES = src/$(NAME).cpp $(COMPONENTS:%=src/%.cpp)

#  ----------------------------------------------------------------------------

ifeq ($(COMPILER),gcc)
   VERSION = 10.1.0
   PREFIX   = /opt/gcc-$(VERSION)
   CPPFLAGS = -std=c++2a
   CXXFLAGS = -W -Wall
   OPTFLAGS = -O3
   CXX      = $(PREFIX)/bin/g++
   CC       = $(PREFIX)/bin/gcc
endif
ifeq ($(COMPILER),clang)
   CPPFLAGS = -std=c++2a
   CXXFLAGS = -W -Wall
   OPTFLAGS = -O3
   CXX      = /opt/llvm-10.0.0/bin/clang++
endif
ifeq ($(COMPILER),intel)
   CXXFLAGS  = -std=c++17
   OPTFLAGS  = -O2
   #LDFLAGS   = -stdlib=libstdc++
   CXX       = /opt/intel/bin/icc
endif
ifeq ($(COMPILER),edg)
   CXXFLAGS  = --c++20
   OPTFLAGS  = -O2
   CXX       = /opt/edg/bin/eccp
endif

ifeq ($(DEBUG),)
    CXXFLAGS += $(OPTFLAGS)
    SUFFIX   = opt
else
    CXXFLAGS += -g
    SUFFIX   = dbg
endif

CPPFLAGS += -Iinclude
CXXFLAGS += $(CPPFLAGS)
BUILD    = bin/$(COMPILER)-$(VERSION)-$(SUFFIX)

#  ----------------------------------------------------------------------------

.PHONY: default
default: $(BUILD)/$(NAME)
	$(BUILD)/$(NAME)

%:
	$(MAKE) NAME=$@

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
	$(CXX) $(CPPFLAGS) -M $(CXXFILES) | sed -e 's@.*\.o: *\(src/\(.*\)\.cpp\)@$(BUILD)/\2.o: \1@' > $(BUILD)/make.depend

include $(BUILD)/make.depend
