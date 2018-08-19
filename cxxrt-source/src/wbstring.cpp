// -*-C++-*- wbstring.cc
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
// Title:   Instantiations for basic_string<wchar_t>
// Version: $Id: wbstring.cpp,v 1.1.1.1 2002/06/05 01:00:15 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#define _CXXRT_STRINGIO_IMPL

#define _CXXRT_STRING_INSTANTIATION 1
#include <srccxx/string.cc>
#include <cxxrt/ctraits.h>
#include <cxxrt/sstr_fwd.h>
#include <cxxrt/alloc.h>

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

#if defined(__STL_USE_STD_ALLOCATORS)
template class _String_base<wchar_t, allocator<wchar_t> >;
template class basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >;
#else
template class _String_base<wchar_t, alloc>;
template class basic_string<wchar_t, char_traits<wchar_t>, alloc>;
#endif

template bool operator== (basic_string<wchar_t> const&, basic_string<wchar_t> const&);
template bool operator== (wchar_t const*, basic_string<wchar_t> const&);
template bool operator== (basic_string<wchar_t> const&, wchar_t const*);
#if !defined(__EDG__)
template bool operator!= (basic_string<wchar_t> const&, basic_string<wchar_t> const&);
template bool operator!= (wchar_t const*, basic_string<wchar_t> const&);
template bool operator!= (basic_string<wchar_t> const&, wchar_t const*);
#endif

template basic_string<wchar_t> operator+ (wchar_t const*, basic_string<wchar_t> const&);
template basic_string<wchar_t> operator+ (basic_string<wchar_t> const&, wchar_t const*);

template basic_istream<wchar_t>& getline(basic_istream<wchar_t>&, basic_string<wchar_t>&, wchar_t);
template basic_istream<wchar_t>& getline(basic_istream<wchar_t>&, basic_string<wchar_t>&);
template basic_ostream<wchar_t>& operator<< (basic_ostream<wchar_t>&, basic_string<wchar_t> const&);
template basic_istream<wchar_t>& operator>> (basic_istream<wchar_t>&, basic_string<wchar_t>&);

// -------------------------------------------------------------------------- 

template wchar_t * search<wchar_t *, wchar_t const *, _Eq_traits<char_traits<wchar_t> > >(wchar_t *, wchar_t *, wchar_t const *, wchar_t const *, _Eq_traits<char_traits<wchar_t> >);
template wchar_t const * find_first_of<wchar_t const *, wchar_t const *, _Eq_traits<char_traits<wchar_t> > >(wchar_t const *, wchar_t const *, wchar_t const *, wchar_t const *, _Eq_traits<char_traits<wchar_t> >);
#if defined(__STL_USE_STD_ALLOCATORS)
template reverse_iterator<wchar_t const*> find_first_of<reverse_iterator<wchar_t const*>, wchar_t const *, _Eq_traits<char_traits<wchar_t> > >(reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, wchar_t const *, wchar_t const *, _Eq_traits<char_traits<wchar_t> >);
#else
template reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t> find_first_of<reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, wchar_t const *, _Eq_traits<char_traits<wchar_t> > >(reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, wchar_t const *, wchar_t const *, _Eq_traits<char_traits<wchar_t> >);
#endif
template wchar_t * __uninitialized_fill_n_aux<wchar_t *, unsigned int, wchar_t>(wchar_t *, unsigned int, wchar_t const &, __false_type);
#if !defined(_MSC_VER)
template basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > & basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >::append<wchar_t const *>(wchar_t const *, wchar_t const *, forward_iterator_tag);
template void basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >::insert<wchar_t const *>(wchar_t *, wchar_t const *, wchar_t const *, forward_iterator_tag);
template basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > & basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >::replace<wchar_t const *>(wchar_t *, wchar_t *, wchar_t const *, wchar_t const *, forward_iterator_tag);
template basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > & basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >::replace<wchar_t *>(wchar_t *, wchar_t *, wchar_t *, wchar_t *, forward_iterator_tag);
template void basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >::insert<wchar_t *>(wchar_t *, wchar_t *, wchar_t *, forward_iterator_tag);
#endif
template wchar_t * __uninitialized_copy_aux<wchar_t *, wchar_t *>(wchar_t *, wchar_t *, wchar_t *, __false_type);
template wchar_t * find_if<wchar_t *, binder2nd<_Eq_traits<char_traits<wchar_t> > > >(wchar_t *, wchar_t *, binder2nd<_Eq_traits<char_traits<wchar_t> > >, random_access_iterator_tag);
template wchar_t const * __find_end<wchar_t const *, wchar_t const *, _Eq_traits<char_traits<wchar_t> > >(wchar_t const *, wchar_t const *, wchar_t const *, wchar_t const *, bidirectional_iterator_tag, bidirectional_iterator_tag, _Eq_traits<char_traits<wchar_t> >);
#if defined(__STL_USE_STD_ALLOCATORS)
template reverse_iterator<wchar_t const*> find_if<reverse_iterator<wchar_t const*>, binder2nd<_Eq_traits<char_traits<wchar_t> > > >(reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, binder2nd<_Eq_traits<char_traits<wchar_t> > >, random_access_iterator_tag);
#else
template reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t> find_if<reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, binder2nd<_Eq_traits<char_traits<wchar_t> > > >(reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, binder2nd<_Eq_traits<char_traits<wchar_t> > >, random_access_iterator_tag);
#endif
template wchar_t * find_if<wchar_t *, _Not_within_traits<char_traits<wchar_t> > >(wchar_t *, wchar_t *, _Not_within_traits<char_traits<wchar_t> >, random_access_iterator_tag);
template wchar_t const * find_if<wchar_t const *, unary_negate<binder2nd<_Eq_traits<char_traits<wchar_t> > > > >(wchar_t const *, wchar_t const *, unary_negate<binder2nd<_Eq_traits<char_traits<wchar_t> > > >, random_access_iterator_tag);
#if defined(__STL_USE_STD_ALLOCATORS)
template reverse_iterator<wchar_t const*> find_if<reverse_iterator<wchar_t const*>, _Not_within_traits<char_traits<wchar_t> > >(reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, _Not_within_traits<char_traits<wchar_t> >, random_access_iterator_tag);
template reverse_iterator<wchar_t const*> find_if<reverse_iterator<wchar_t const*>, unary_negate<binder2nd<_Eq_traits<char_traits<wchar_t> > > > >(reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, unary_negate<binder2nd<_Eq_traits<char_traits<wchar_t> > > >, random_access_iterator_tag);
template reverse_iterator<wchar_t const*> search<reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, _Eq_traits<char_traits<wchar_t> > >(reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, reverse_iterator<wchar_t const*>, _Eq_traits<char_traits<wchar_t> >);
#else
template reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t> find_if<reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, _Not_within_traits<char_traits<wchar_t> > >(reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, _Not_within_traits<char_traits<wchar_t> >, random_access_iterator_tag);
template reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t> find_if<reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, unary_negate<binder2nd<_Eq_traits<char_traits<wchar_t> > > > >(reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, unary_negate<binder2nd<_Eq_traits<char_traits<wchar_t> > > >, random_access_iterator_tag);
template reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t> search<reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, _Eq_traits<char_traits<wchar_t> > >(reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, reverse_iterator<wchar_t const*, wchar_t, wchar_t const&, ptrdiff_t>, _Eq_traits<char_traits<wchar_t> >);
#endif
template wchar_t const * find_if<wchar_t const *, binder1st<_Eq_traits<char_traits<wchar_t> > > >(wchar_t const *, wchar_t const *, binder1st<_Eq_traits<char_traits<wchar_t> > >, random_access_iterator_tag);

template basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >& basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >::append<wchar_t*>(wchar_t*, wchar_t*, forward_iterator_tag);

// -------------------------------------------------------------------------- 

_CXXRT_NAMESPACE_END
