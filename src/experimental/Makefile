#  Makefile                                                      -*-makefile-*-
#  ----------------------------------------------------------------------------

LIBCXXFILES = \
	hazptr.cpp \

LIBOFILES = $(LIBCXXFILES:%.cpp=%.o)

CXXFILES = \
	hazptr.t.cpp \
	hazard_list.t.cpp \

.PHONY: default
default: $(CXXFILES:%.t.cpp=%.t)

hazptr.t: $(LIBOFILES) hazptr.t.o
	$(CXX) -o $@ $(@:%=%.o) $(LIBOFILES)

CPPFLAGS = -I. -std=c++17
CXXFLAGS = -W -Wall

clean:
	$(RM) $(CXXFILES:%.t.cpp=%.t) $(CXXFILES:%.t.cpp=%.t.o) $(LIBOFILES)
	$(RM) make.depend mkerr olderr

depend make.depend:
	$(CXX) -M $(CPPFLAGS) $(CXXFILES) $(LIBCXXFILES) > make.depend

include make.depend
