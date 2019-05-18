#  Makefile                                                      -*-makefile-*-
#  ----------------------------------------------------------------------------
#   Copyright (C) 2017 Dietmar Kuehl http://www.dietmar-kuehl.de         
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

NAME = detection
NAME = cbt/movable
NAME = cbt/constructible
NAME = cbt/moveconstructible
NAME = cbt/bidirectionaliterator
NAME = cbt/testequal
NAME = cbt/equalitycomparable
NAME = demo
NAME = inclusive_scan

ALL  = \
   cbt/destructible \
   cbt/bidirectionaliterator \
   cbt/equalitycomparable \
   cbt/testequal \

#  ----------------------------------------------------------------------------

STD      = 17
DEPFLAGS = -M
CPPFLAGS = -std=c++$(STD) -I.
CC       = $(CXX)
CXXFLAGS = -W -Wall -g -O2

.PHONY: default
default: $(NAME).t
	./$(NAME).t | sort -u

.PHONY: all
all:
	for t in $(ALL); do make NAME=$$t; done

#  ----------------------------------------------------------------------------

.PHONY: clean
clean:
	$(RM) -r $(NAME).t.o $(NAME).t $(NAME).t.dSYM
	$(RM) mkerr olderr

.PHONY: clean-all
clean-all:
	for t in $(ALL); do make NAME=$$t clean; done

.PHONY: dist-clean
dist-clean: clean-all
	$(RM) make.depend

depend make.depend:
	$(CXX) $(CPPFLAGS) $(DEPFLAGS) $(NAME).t.cpp \
	    | sed -e 's@^\([^:]*\):@cbt/\1:@' > make.depend

include make.depend
