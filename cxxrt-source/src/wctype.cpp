// -*-C++-*- wctype.cc
// -------------------------------------------------------------------------- 
// Copyright (c) 2002 Dietmar Kuehl

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -------------------------------------------------------------------------- 
// Author:  Dietmar Kuehl <http://www.dietmar-kuehl.de/>
// Title:  Implementation of 'ctype<wchar_t>' 
// Version: $Id: wctype.cpp,v 1.1.1.1 2002/06/05 01:00:09 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/ctype.cc>
#include <cxxrt/algorithm.h>
#include "wcty_dat.cpp"

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

extern _CS_ctype_info<wchar_t> _CS_wchar_t_info[];
extern size_t               _CS_wchar_t_size;

// --------------------------------------------------------------------------

template class ctype<wchar_t>;

// ---------------------------------------------------------------------------

ctype_byname<wchar_t>::ctype_byname(char const *, size_t)
{
}

ctype_byname<wchar_t>::~ctype_byname()
{
}

// -------------------------------------------------------------------------
// explicit instantiation of used templates:

#if 0
template int _CS_qcompare<wchar_t>(void const*, void const*);
#endif

template _CS_ctype_info<wchar_t>*
__lower_bound(_CS_ctype_info<wchar_t>*,
	      _CS_ctype_info<wchar_t>*,
	      _CS_ctype_info<wchar_t> const&,
	      _CS_compare_t<wchar_t>, int*);
template void
__introsort_loop(_CS_ctype_info<wchar_t>*,
		 _CS_ctype_info<wchar_t>*,
		 _CS_ctype_info<wchar_t>*,
		 int,
		 _CS_compare_t<wchar_t>);
template void
__final_insertion_sort(_CS_ctype_info<wchar_t>*,
		       _CS_ctype_info<wchar_t>*,
		       _CS_compare_t<wchar_t>);
template void
__partial_sort(_CS_ctype_info<wchar_t>*,
	       _CS_ctype_info<wchar_t>*,
	       _CS_ctype_info<wchar_t>*,
	       _CS_ctype_info<wchar_t>*,
	       _CS_compare_t<wchar_t>);
template void
__unguarded_insertion_sort_aux(_CS_ctype_info<wchar_t>*,
			       _CS_ctype_info<wchar_t>*,
			       _CS_ctype_info<wchar_t>*,
			       _CS_compare_t<wchar_t>);
template _CS_ctype_info<wchar_t>*
__unguarded_partition(_CS_ctype_info<wchar_t>*,
		      _CS_ctype_info<wchar_t>*,
		      _CS_ctype_info<wchar_t>,
		      _CS_compare_t<wchar_t>);
template void
__insertion_sort(_CS_ctype_info<wchar_t>*, _CS_ctype_info<wchar_t>*, _CS_compare_t<wchar_t>);

template void
__unguarded_linear_insert(_CS_ctype_info<wchar_t>*, _CS_ctype_info<wchar_t>, _CS_compare_t<wchar_t>);

template void
__make_heap(_CS_ctype_info<wchar_t>*,
	    _CS_ctype_info<wchar_t>*,
	    _CS_compare_t<wchar_t>,
	    _CS_ctype_info<wchar_t>*,
	    int*);

template void
__adjust_heap(_CS_ctype_info<wchar_t> *, int, int, _CS_ctype_info<wchar_t>, _CS_compare_t<wchar_t>);

template void
sort_heap(_CS_ctype_info<wchar_t>*, _CS_ctype_info<wchar_t>*, _CS_compare_t<wchar_t>);

template void
__push_heap(_CS_ctype_info<wchar_t>*, int, int, _CS_ctype_info<wchar_t>, _CS_compare_t<wchar_t>);

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
