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
	algorithm \
	alloc.h \
	bitset \
	char_traits.h \
	char_traits_helper.h \
	deque \
	functional \
	iterator \
	list \
	map \
	memory \
	numeric \
	queue \
	set \
	stack \
	stl_algo.h \
	stl_algobase.h \
	stl_alloc.h \
	stl_bvector.h \
	stl_config.h \
	stl_construct.h \
	stl_deque.h \
	stl_exception.h \
	stl_function.h \
	stl_hash_fun.h \
	stl_heap.h \
	stl_iterator.h \
	stl_iterator_base.h \
	stl_list.h \
	stl_map.h \
	stl_multimap.h \
	stl_multiset.h \
	stl_numeric.h \
	stl_pair.h \
	stl_queue.h \
	stl_range_errors.h \
	stl_raw_storage_iter.h \
	stl_relops.h \
	stl_reverse_iterator.h \
	stl_set.h \
	stl_stack.h \
	stl_string_fwd.h \
	stl_tempbuf.h \
	stl_threads.h \
	stl_tree.h \
	stl_uninitialized.h \
	stl_uninitialized_copy.h \
	stl_vector.h \
	type_traits.h \
	utility \
	valarray \
	vector

# ------------------------------------------------------------------------------

INSTALL = install
INSTALLINC = $(INSTALLDIR)/include

install: $(HEADER)
	mkdirhier $(INSTALLINC)
	$(INSTALL) -m 0444 $(HEADER) $(INSTALLINC)

depend:

