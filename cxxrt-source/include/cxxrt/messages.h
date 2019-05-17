// -*-C++-*- cxxrt/messages.h
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
// Title:   Declarations for the messages facet
// Version: $Id: messages.h,v 1.1.1.1 2002/06/05 01:01:44 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_MESSAGES_H__)
#define _CXXRT_MESSAGES_H__ 1

#if !defined(_CXXRT_LOCALE_H__)
#  include <cxxrt/locale.h>
#endif 
#if !defined(_CXXRT_BSTR_FWD_H__)
#  include <cxxrt/bstr_fwd.h>
#endif

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

class messages_base
{
public:
        typedef int catalog;
};

// --------------------------------------------------------------------------

template <class _CS_cT>
class messages: public locale::facet, public messages_base
{
public:
        typedef _CS_cT               char_type;
        typedef basic_string<_CS_cT> string_type;

        static locale::id id;

        _CXXRT_EXPLICIT messages(size_t = 0);

        catalog     open(basic_string<char> const&, locale const&) const;
        string_type get(catalog, int, int, string_type const&) const;
        void        close(catalog) const;

protected:
        virtual ~messages();
        
        virtual catalog     do_open(basic_string<char> const&, locale const&) const;
        virtual string_type do_get(catalog, int, int, string_type const&) const;
        virtual void        do_close(catalog) const;
};

// --------------------------------------------------------------------------

template <class _CS_cT>
class messages_byname: public messages<_CS_cT>
{
public:
        typedef _CS_cT               char_type;
        typedef basic_string<_CS_cT> string_type;

        _CXXRT_EXPLICIT messages_byname(char const*, size_t = 0);

protected:
        virtual ~messages_byname();
        
        virtual messages_base::catalog do_open(basic_string<char> const&, locale const&) const;
        virtual string_type do_get(messages_base::catalog, int, int, string_type const&) const;
        virtual void        do_close(messages_base::catalog) const;
};

// --------------------------------------------------------------------------

template <>
inline bool has_facet<messages<char> >(locale const&) 
{
  return true;
}

template <>
inline messages<char> const& use_facet<messages<char> >(locale const& _CS_l)
{
  return _CS_use_facet<messages<char> >(_CS_l);
}

template <>
inline bool has_facet<messages<wchar_t> >(locale const&) 
{
  return true;
}
template <>
inline messages<wchar_t> const& use_facet<messages<wchar_t> >(locale const& _CS_l)
{
  return _CS_use_facet<messages<wchar_t> >(_CS_l);
}

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#if defined(_CXXRT_LOCALE_IMPL) && !defined(__SRCCXX_MESSAGES_CC__)
#  include <srccxx/messages.cc>
#endif

#endif /* _CXXRT_MESSAGES_H__ */
