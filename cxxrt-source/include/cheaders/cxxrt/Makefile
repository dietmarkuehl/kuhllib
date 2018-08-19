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
	algorithm.h \
	alloc.h \
	auto_ptr.h \
	bastring.h \
	bstr_fwd.h \
	c_lfile.h \
	c_locale.h \
	c_locdef.h \
	codecvt.h \
	collate.h \
	complex.h \
	convert.h \
	copy.h \
	count.h \
	ctraits.h \
	ctraits_fwd.h \
	ctype.h \
	ctype_base.h \
	cvtbuf.h \
	dkfloat.h \
	dkmap.h \
	extractor.h \
	find.h \
	for_each.h \
	fstr_fwd.h \
	fstream.h \
	in_it.h \
	iomanip.h \
	ioobj.h \
	ios.h \
	ios_base.h \
	iosfwd.h \
	iostream.h \
	istream.h \
	istream_it.h \
	iterator.h \
	iterhelp.h \
	lcache.h \
	limits.h \
	locale.h \
	messages.h \
	money_get.h \
	money_put.h \
	moneypunct.h \
	num_get.h \
	num_put.h \
	numpunct.h \
	ostream.h \
	ostream_it.h \
	sbit_fwd.h \
	sbuf_fwd.h \
	sbuf_it.h \
	sstr_fwd.h \
	sstream.h \
	stdexcept.h \
	streambuf.h \
	string.h \
	string_hash.h \
	stringio.h \
	strstream.h \
	sysfile.h \
	time_base.h \
	time_get.h \
	time_put.h

# ------------------------------------------------------------------------------

INSTALL = install
INSTALLINC = $(INSTALLDIR)/include/cxxrt

install: $(HEADER)
	mkdirhier $(INSTALLINC)
	$(INSTALL) -m 0444 $(HEADER) $(INSTALLINC)

depend:

