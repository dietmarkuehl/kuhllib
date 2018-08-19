// -*-C++-*- src/cbstring.cc
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
// Title:   Instantiation of basic_string for narrow characters
// Version: $Id: cbstring.cpp,v 1.1.1.1 2002/06/05 01:00:23 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#define _CXXRT_STRINGIO_IMPL 1

#define _CXXRT_STRING_INSTANTIATION 1
#include <srccxx/string.cc>
#include <cxxrt/ctraits.h>
#include <cxxrt/alloc.h>
#include <cxxrt/sstr_fwd.h>
#ifdef __EDG__
#  include <stdexcept.h>
#else
#  include <stdexcept>
#endif

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

char const* __get_c_string(string const& __s) { return __s.c_str(); }

#if defined(__STL_USE_STD_ALLOCATORS)
template class _String_base<char, allocator<char> >;
template class basic_string<char, char_traits<char>, allocator<char> >;
#else
template class _String_base<char, alloc>;
template class basic_string<char, char_traits<char>, alloc>;
#endif

template bool operator== (basic_string<char, char_traits<char>, allocator<char> > const&, basic_string<char, char_traits<char>, allocator<char> > const&);
template bool operator== (char const*, basic_string<char, char_traits<char>, allocator<char> > const&);
template bool operator== (basic_string<char, char_traits<char>, allocator<char> > const&, char const*);
#if !defined(__EDG__)
template bool operator!= (basic_string<char> const&, basic_string<char> const&);
template bool operator!= (char const*, basic_string<char> const&);
template bool operator!= (basic_string<char> const&, char const*);
#endif

template basic_string<char> operator+ (char const*, basic_string<char> const&);
template basic_string<char> operator+ (basic_string<char> const&, char const*);

template basic_istream<char>& getline(basic_istream<char>&, basic_string<char>&, char);
template basic_istream<char>& getline(basic_istream<char>&, basic_string<char>&);
template basic_ostream<char, char_traits<char> >& operator<< (basic_ostream<char, char_traits<char> >&, basic_string<char, char_traits<char> > const&);
template basic_istream<char, char_traits<char> >& operator>> (basic_istream<char, char_traits<char> >&, basic_string<char, char_traits<char> >&);

// -------------------------------------------------------------------------- 

template char*
search<char*, char const*, _Eq_traits<char_traits<char> > >(char*, char*, char const*, char const*,_Eq_traits<char_traits<char> >);

template char const * find_first_of<char const *, char const *, _Eq_traits<char_traits<char> > >(char const *, char const *, char const *, char const *, _Eq_traits<char_traits<char> >);
#if defined(__STL_CLASS_PARTIAL_SPECIALIZATION)
template reverse_iterator<char const *> find_first_of<reverse_iterator<char const *>, char const *, _Eq_traits<char_traits<char> > >(reverse_iterator<char const *>, reverse_iterator<char const *>, char const *, char const *, _Eq_traits<char_traits<char> >);
template reverse_iterator<char const*> find_if<reverse_iterator<char const*>, binder2nd<_Eq_traits<char_traits<char> > > >(reverse_iterator<char const*>, reverse_iterator<char const*>, binder2nd<_Eq_traits<char_traits<char> > >, random_access_iterator_tag);
template reverse_iterator<char const*> find_if<reverse_iterator<char const*>, _Not_within_traits<char_traits<char> > >(reverse_iterator<char const*>, reverse_iterator<char const*>, _Not_within_traits<char_traits<char> >, random_access_iterator_tag);
template reverse_iterator<char const*> find_if<reverse_iterator<char const*>, unary_negate<binder2nd<_Eq_traits<char_traits<char> > > > >(reverse_iterator<char const*>, reverse_iterator<char const*>, unary_negate<binder2nd<_Eq_traits<char_traits<char> > > >, random_access_iterator_tag);
template reverse_iterator<char const*> search<reverse_iterator<char const*>, reverse_iterator<char const*>, _Eq_traits<char_traits<char> > >(reverse_iterator<char const*>, reverse_iterator<char const*>, reverse_iterator<char const*>, reverse_iterator<char const*>, _Eq_traits<char_traits<char> >);
#else
template reverse_iterator<char const *, char, char const&, ptrdiff_t> find_first_of<reverse_iterator<char const *, char, char const&, ptrdiff_t>, char const *, _Eq_traits<char_traits<char> > >(reverse_iterator<char const *, char, char const&, ptrdiff_t>, reverse_iterator<char const *, char, char const&, ptrdiff_t>, char const *, char const *, _Eq_traits<char_traits<char> >);
template reverse_iterator<char const *, char, char const&, ptrdiff_t> find_if<reverse_iterator<char const *, char, char const&, ptrdiff_t>, binder2nd<_Eq_traits<char_traits<char> > > >(reverse_iterator<char const *, char, char const&, ptrdiff_t>, reverse_iterator<char const *, char, char const&, ptrdiff_t>, binder2nd<_Eq_traits<char_traits<char> > >, random_access_iterator_tag);
template reverse_iterator<char const *, char, char const&, ptrdiff_t> find_if<reverse_iterator<char const *, char, char const&, ptrdiff_t>, _Not_within_traits<char_traits<char> > >(reverse_iterator<char const*, char, char const&, ptrdiff_t>, reverse_iterator<char const*, char, char const&, ptrdiff_t>, _Not_within_traits<char_traits<char> >, random_access_iterator_tag);
template reverse_iterator<char const*, char, char const&, ptrdiff_t> find_if<reverse_iterator<char const*, char, char const&, ptrdiff_t>, unary_negate<binder2nd<_Eq_traits<char_traits<char> > > > >(reverse_iterator<char const*, char, char const&, ptrdiff_t>, reverse_iterator<char const*, char, char const&, ptrdiff_t>, unary_negate<binder2nd<_Eq_traits<char_traits<char> > > >, random_access_iterator_tag);
template reverse_iterator<char const*, char, char const&, ptrdiff_t> search<reverse_iterator<char const*, char, char const&, ptrdiff_t>, reverse_iterator<char const*, char, char const&, ptrdiff_t>, _Eq_traits<char_traits<char> > >(reverse_iterator<char const*, char, char const&, ptrdiff_t>, reverse_iterator<char const*, char, char const&, ptrdiff_t>, reverse_iterator<char const*, char, char const&, ptrdiff_t>, reverse_iterator<char const*, char, char const&, ptrdiff_t>, _Eq_traits<char_traits<char> >);
#endif
#if !defined(_MSC_VER)
template char * fill_n<char *, unsigned int, char>(char *, unsigned int, char const &);
template basic_string<char, char_traits<char>, allocator<char> > & basic_string<char, char_traits<char>, allocator<char> >::append<char const *>(char const *, char const *, forward_iterator_tag);
template void basic_string<char, char_traits<char>, allocator<char> >::insert<char const *>(char *, char const *, char const *, forward_iterator_tag);
template basic_string<char, char_traits<char>, allocator<char> > & basic_string<char, char_traits<char>, allocator<char> >::replace<char const *>(char *, char *, char const *, char const *, forward_iterator_tag);
template basic_string<char, char_traits<char>, allocator<char> > & basic_string<char, char_traits<char>, allocator<char> >::replace<char *>(char *, char *, char *, char *, forward_iterator_tag);
template void basic_string<char, char_traits<char>, allocator<char> >::insert<char *>(char *, char *, char *, forward_iterator_tag);
#endif
template char * find_if<char *, binder2nd<_Eq_traits<char_traits<char> > > >(char *, char *, binder2nd<_Eq_traits<char_traits<char> > >, random_access_iterator_tag);
template char const * __find_end<char const *, char const *, _Eq_traits<char_traits<char> > >(char const *, char const *, char const *, char const *, bidirectional_iterator_tag, bidirectional_iterator_tag, _Eq_traits<char_traits<char> >);
template char * find_if<char *, _Not_within_traits<char_traits<char> > >(char *, char *, _Not_within_traits<char_traits<char> >, random_access_iterator_tag);
template char const * find_if<char const *, unary_negate<binder2nd<_Eq_traits<char_traits<char> > > > >(char const *, char const *, unary_negate<binder2nd<_Eq_traits<char_traits<char> > > >, random_access_iterator_tag);
template char const * find_if<char const *, binder1st<_Eq_traits<char_traits<char> > > >(char const *, char const *, binder1st<_Eq_traits<char_traits<char> > >, random_access_iterator_tag);

template basic_string<char, char_traits<char>, allocator<char> >& basic_string<char, char_traits<char>, allocator<char> >::append<char*>(char*, char*, forward_iterator_tag);

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
