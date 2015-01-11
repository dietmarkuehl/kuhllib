// nstd/locale/locale.t.cpp                                           -*-C++-*-
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

#include "nstd/locale/locale.hpp"
#include "nstd/cheaders/cstddef.hpp"
#include "kuhl/test.hpp"

namespace NL = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct named_locale {
        NL::locale const& locale;
        char const*       name;
        named_locale(NL::locale const& locale, char const* name)
            : locale(locale)
            , name(name) {
        }
        auto operator== (named_locale const& other) const -> bool {
            return this->locale == other.locale;
        }
        auto operator!= (named_locale const& other) const -> bool {
            return this->locale != other.locale;
        }
    };

    template <typename Stream>
    Stream& operator<< (Stream& out, named_locale const& locale) {
        return out << locale.name;
    }
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("locale is defined", [](KT::context& c)->bool{
            return KT::assert_true(c, "locale is defined", sizeof(NL::locale))
                ;
        }),
    KT::expect_success("class locale [locale]", [](KT::context& c)->bool{
            return KT::assert_declared<NL::locale>(c, "class locale")
                && KT::assert_declared<NL::locale::facet>(c, "class locale::facet")
                && KT::assert_declared<NL::locale::id>(c, "class locale::id")
                && KT::assert_type<int, NL::locale::category>(c, "type locale::category")
                && KT::assert_type<int const, decltype(NL::locale::none)>(c, "type of locale::none")
                && KT::assert_type<int const, decltype(NL::locale::collate)>(c, "type of locale::collate")
                && KT::assert_type<int const, decltype(NL::locale::ctype)>(c, "type of locale::ctype")
                && KT::assert_type<int const, decltype(NL::locale::monetary)>(c, "type of locale::monetary")
                && KT::assert_type<int const, decltype(NL::locale::numeric)>(c, "type of locale::numeric")
                && KT::assert_type<int const, decltype(NL::locale::time)>(c, "type of locale::time")
                && KT::assert_type<int const, decltype(NL::locale::messages)>(c, "type of locale::messages")
                && KT::assert_type<int const, decltype(NL::locale::all)>(c, "type of of locale::all")
                && KT::assert_equal(c, "locale::all", NL::locale::all,
                                    NL::locale::collate
                                    | NL::locale::ctype
                                    | NL::locale::monetary
                                    | NL::locale::numeric
                                    | NL::locale::time
                                    | NL::locale::messages)
                ;
        }),
    KT::expect_success("type locale::category [locale.category]", [](KT::context& c)->bool{
            return true //-dk:TODO verify that all category facets are present in locales
                ;
        }),
    KT::expect_success("class locale::facet [locale.facet]", [](KT::context& c)->bool{
            NL::locale::facet f0;
            NL::locale::facet f1{NL::size_t{0}};
            struct facet: NL::locale::facet { ~facet() override {} };
            return true
                ;
        }),
    KT::expect_success("class locale::id [locale.id]", [](KT::context& c)->bool{
            NL::locale::id id;
            return true
                ;
        }),
    KT::expect_success("locale structors [locale.cons]", [](KT::context& c)->bool{
            NL::locale        def;
            NL::locale const& classic{NL::locale::classic()};
            return KT::assert_equal(c, "default locale equal classic()",
                                    named_locale(def, "default"),
                                    named_locale(classic, "classic"))
                ;
        }),

};

int main(int ac, char* av[])
{
    return KT::run_tests("locale/locale", ac, av, ::tests);
}
