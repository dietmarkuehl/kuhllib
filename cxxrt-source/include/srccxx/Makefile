# -*-makefile-*-
# -------------------------------------------------------------------------- 
# Copyright (c) 2002 Dietmar Kuehl

# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject
# to the following conditions:

# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
# KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# -------------------------------------------------------------------------- 
# Author:  Dietmar Kuehl <http://www.dietmar-kuehl.de/>
# Title:   makefile installing the header files
# Version: $Id: Makefile,v 1.1 2002/06/07 00:46:11 dietmar_kuehl Exp $
# -------------------------------------------------------------------------- 

HEADER = \
	bastring.cc \
	codecvt.cc \
	collate.cc \
	complex.cc \
	convert.cc \
	ctype.cc \
	cvtbuf.cc \
	filebuf.cc \
	fstream.cc \
	iomanip.cc \
	ios.cc \
	iostream.cc \
	isbuf_it.cc \
	istream.cc \
	lcache.cc \
	messages.cc \
	money_get.cc \
	money_put.cc \
	moneypunct.cc \
	num_get.cc \
	num_lim.cc \
	num_put.cc \
	numpunct.cc \
	osbuf_it.cc \
	ostream.cc \
	sstream.cc \
	stl_default_alloc.cc \
	stl_malloc_alloc.cc \
	streambuf.cc \
	string.cc \
	time_get.cc \
	time_put.cc \

# ------------------------------------------------------------------------------

INSTALL = install
INSTALLINC = $(INSTALLDIR)/include/srccxx

install: $(HEADER)
	mkdirhier $(INSTALLINC)
	$(INSTALL) -m 0444 $(HEADER) $(INSTALLINC)

depend:
