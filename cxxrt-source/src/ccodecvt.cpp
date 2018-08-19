// -*-C++-*- ccodecvt.cc
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
// Title:   Definitions for the facet codecvt<char, char, mbstate_t>
// Version: $Id: ccodecvt.cpp,v 1.1.1.1 2002/06/05 01:00:20 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/codecvt.cc>

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

codecvt<char, char, mbstate_t>::codecvt(size_t _CS_refs):
	locale::facet(_CS_refs)
{
}

locale::id codecvt<char, char, mbstate_t>::id;

// -------------------------------------------------------------------------- 

codecvt_base::result codecvt<char, char, mbstate_t>::do_out(
	state_type &,
	intern_type const *fbegin, intern_type const *, intern_type const *&fnext,
	extern_type *tbegin, extern_type *, extern_type *&tnext) const
{
	fnext = fbegin;
	tnext = tbegin;
	return noconv;
}

codecvt_base::result codecvt<char, char, mbstate_t>::out(
	state_type &state,
	intern_type const *fbegin, intern_type const *fend, intern_type const *&fnext,
	extern_type *tbegin, extern_type *tend, extern_type *&tnext) const
{
	return do_out(state, fbegin, fend, fnext, tbegin, tend, tnext);
}

// -------------------------------------------------------------------------- 

codecvt_base::result codecvt<char, char, mbstate_t>::do_unshift(
	state_type &,
	extern_type *tbegin, extern_type *, extern_type *&tnext) const
{
	tnext = tbegin;
	return noconv;
}

codecvt_base::result codecvt<char, char, mbstate_t>::unshift(
	state_type &state,
	extern_type *tbegin, extern_type *tend, extern_type *&tnext) const
{
	return do_unshift(state, tbegin, tend, tnext);
}

// -------------------------------------------------------------------------- 

codecvt_base::result codecvt<char, char, mbstate_t>::do_in(
	state_type &,
	extern_type const *fbegin, extern_type const *, extern_type const *&fnext,
	intern_type *tbegin, intern_type *, intern_type *&tnext) const
{
	fnext = fbegin;
	tnext = tbegin;
	return noconv;
}

codecvt_base::result codecvt<char, char, mbstate_t>::in(
	state_type &state,
	extern_type const *fbegin, extern_type const *fend, extern_type const *&fnext,
	intern_type *tbegin, intern_type *tend, intern_type *&tnext) const
{
	return do_in(state, fbegin, fend, fnext, tbegin, tend, tnext);
}

// -------------------------------------------------------------------------- 

int codecvt<char, char, mbstate_t>::do_encoding() const throw()
{
	return 1;
}

int codecvt<char, char, mbstate_t>::encoding() const throw()
{
	return do_encoding();
}

// -------------------------------------------------------------------------- 

bool codecvt<char, char, mbstate_t>::do_always_noconv() const throw()
{
	return true;
}

bool codecvt<char, char, mbstate_t>::always_noconv() const throw()
{
	return do_always_noconv();
}

// -------------------------------------------------------------------------- 

int codecvt<char, char, mbstate_t>::do_length(
	state_type const &,
	extern_type const *begin, extern_type const *end,
	size_t max) const
{
	size_t range(end - begin);
	return range < max? range: max;
}

int codecvt<char, char, mbstate_t>::length(
	state_type const &state,
	extern_type const *begin, extern_type const *end,
	size_t max) const
{
	return do_length(state, begin, end, max);
}

// -------------------------------------------------------------------------- 

int codecvt<char, char, mbstate_t>::do_max_length() const throw()
{
	return 1;
}

int codecvt<char, char, mbstate_t>::max_length() const throw()
{
	return do_max_length();
}

//------------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
