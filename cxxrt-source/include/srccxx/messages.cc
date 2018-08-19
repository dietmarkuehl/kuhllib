// -*-C++-*- srccxx/messages.cc
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
// Title:   Implementations for the srccxx/messages category
// Version: $Id: messages.cc,v 1.2 2002/06/07 00:46:11 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(__SRCCXX_MESSAGES_CC__)
#define __SRCCXX_MESSAGES_CC__ 1

#if !defined(_CXXRT_MESSAGES_H__)
#  include <cxxrt/messages.h>
#endif
#if !defined(_CXXRT_STRING_H__)
#  include <cxxrt/string.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT>
locale::id messages<_CS_cT>::id;

// --------------------------------------------------------------------------

template <class _CS_cT>
messages<_CS_cT>::messages(size_t _CS_refs):
  locale::facet(_CS_refs)
{
}

template <class _CS_cT>
messages<_CS_cT>::~messages()
{
}

template <class _CS_cT>
messages_base::catalog
messages<_CS_cT>::open(basic_string<char> const& _CS_cn, locale const& _CS_l) const
{
  return do_open(_CS_cn, _CS_l);
}

template <class _CS_cT>
typename messages<_CS_cT>::string_type
messages<_CS_cT>::get(catalog _CS_cat,
		      int _CS_set,
		      int _CS_msgid,
		      basic_string<_CS_cT> const& _CS_dfault) const
{
  return do_get(_CS_cat, _CS_set, _CS_msgid, _CS_dfault);
}

template <class _CS_cT>
void
messages<_CS_cT>::close(catalog _CS_cat) const
{
        do_close(_CS_cat);
}

// --------------------------------------------------------------------------

template <class _CS_cT>
messages_base::catalog
messages<_CS_cT>::do_open(basic_string<char> const&,
			  locale const&) const
{
  return 0; //-dk:TODO
}

template <class _CS_cT>
typename messages<_CS_cT>::string_type
messages<_CS_cT>::do_get(catalog,
			 int,
			 int,
			 basic_string<_CS_cT> const& _CS_dfault) const
{
  return _CS_dfault; //-dk:TODO
}

template <class _CS_cT>
void
messages<_CS_cT>::do_close(catalog) const
{
  //-dk:TODO
}

// --------------------------------------------------------------------------


template <class _CS_cT>
messages_byname<_CS_cT>::messages_byname(char const*,
					 size_t _CS_refs):
  messages<_CS_cT>(_CS_refs)
{
  //-dk:TODO
}

template <class _CS_cT>
messages_byname<_CS_cT>::~messages_byname()
{
}

template <class _CS_cT>
messages_base::catalog
messages_byname<_CS_cT>::do_open(basic_string<char> const&,
				 locale const&) const
{
  return 0; //-dk:TODO
}

template <class _CS_cT>
typename messages_byname<_CS_cT>::string_type
messages_byname<_CS_cT>::do_get(messages_base::catalog,
				int,
				int,
				basic_string<_CS_cT> const& _CS_dfault) const
{
  return _CS_dfault; //-dk:TODO
}

template <class _CS_cT>
void
messages_byname<_CS_cT>::do_close(messages_base::catalog) const
{
  //-dk:TODO
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __SRCCXX_MESSAGES_CC__ */
