// nstd/locale/locale.hpp                                             -*-C++-*-
// ----------------------------------------------------------------------------
//  Copyright (C) 2015 Dietmar Kuehl http://www.dietmar-kuehl.de         
//                                                                       
//  Permission is hereby granted, free of charge, to any person          
//  obtaining a copy of this software and associated documentation       
//  files (the "Software"), to deal in the Software without restriction, 
//  including without limitation the rights to use, copy, modify,        
//  merge, publish, distribute, sublicense, and/or sell copies of        
//  the Software, and to permit persons to whom the Software is          
//  furnished to do so, subject to the following conditions:             
//                                                                       
//  The above copyright notice and this permission notice shall be       
//  included in all copies or substantial portions of the Software.      
//                                                                       
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING         
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR        
//  OTHER DEALINGS IN THE SOFTWARE. 
// ----------------------------------------------------------------------------

#ifndef INCLUDED_NSTD_LOCALE_LOCALE
#define INCLUDED_NSTD_LOCALE_LOCALE

#include "nstd/locale/locale_fwd.hpp"
#include "nstd/string/string_fwd.hpp"
#include "nstd/cheaders/cstddef.hpp"

// ----------------------------------------------------------------------------

namespace nstd {
    template <typename Facet>
    auto use_facet(::nstd::locale const&) -> Facet const&;
    template <typename>
    auto has_cacet(::nstd::locale const&) noexcept(true) -> bool;
}

// ----------------------------------------------------------------------------

class nstd::locale {
public:
    class facet;
    class id;
    using category = int;

    static constexpr category none     = 0x0000;
    static constexpr category collate  = 0x0001;
    static constexpr category ctype    = 0x0002;
    static constexpr category monetary = 0x0004;
    static constexpr category numeric  = 0x0008;
    static constexpr category time     = 0x0010;
    static constexpr category messages = 0x0020;
    static constexpr category all      = 0x003f;

    locale() noexcept(true);
    locale(locale const&) noexcept(true);
    locale(char const*);
    locale(::nstd::string const&);
    locale(char const*, category);
    locale(::nstd::string const&, category);
    template <typename Facet>
    locale(locale const&, Facet*);
    locale(locale const&, locale const&, category);
    ~locale();

    auto operator= (locale const&) noexcept(true) -> locale const&;
    template <typename>
    auto combine(locale const&) const -> locale;

    auto name() const -> ::nstd::string;

    auto operator== (locale const&) const -> bool;
    auto operator!= (locale const&) const -> bool;

    template <typename cT, typename Traits, typename Allocator>
    auto operator()(::nstd::basic_string<cT, Traits, Allocator> const&,
                    ::nstd::basic_string<cT, Traits, Allocator> const&) const
        -> bool;

    static auto global(locale const&) -> locale;
    static auto classic() -> locale const&;
};

// ----------------------------------------------------------------------------

class nstd::locale::facet {
public:
    explicit facet(::nstd::size_t = 0);
    facet(facet const&) = delete;
    auto operator= (facet const&) -> void = delete;
    virtual ~facet();
};

// ----------------------------------------------------------------------------

class nstd::locale::id {
public:
    id();
    id(id const&) = delete;
    auto operator= (id const&) -> void = delete;
};

// ----------------------------------------------------------------------------

#endif
