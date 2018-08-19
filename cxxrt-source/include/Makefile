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
# Version: $Id: Makefile,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
# -------------------------------------------------------------------------- 

SUBDIRS = \
	cxxrt \
	srccxx \
	sgi

HEADER = \
	assert.h \
	cassert \
	ccomplex \
	cctype \
	cerrno \
	cfloat \
	ciso646 \
	climits \
	clocale \
	cmath \
	complex \
	csetjmp \
	csignal \
	cstdarg \
	cstddef \
	cstdio \
	cstdlib \
	cstring \
	ctime \
	ctype.h \
	cwchar \
	cwctype \
	cxxrtcf.h \
	errno.h \
	float.h \
	fstream \
	iomanip \
	ios \
	iosfwd \
	iostream \
	iso646.h \
	istream \
	limits \
	limits.h \
	locale \
	locale.h \
	math.h \
	new \
	ostream \
	setjmp.h \
	signal.h \
	sstream \
	stdarg.h \
	stddef.h \
	stdexcept \
	stdio.h \
	stdlib.h \
	streambuf \
	string \
	string.h \
	strstream \
	time.h \
	wchar.h \
	wctype.h

# ------------------------------------------------------------------------------

INSTALL = install
INSTALLINC = $(INSTALLDIR)/include

install: $(HEADER)
	mkdirhier $(INSTALLINC)
	$(INSTALL) -m 0444 $(HEADER) $(INSTALLINC)
	for dir in $(SUBDIRS); do cd $$dir; make INSTALLDIR=$(INSTALLDIR) install; cd ..; done

depend:
