// nstd/iostream/ios_base.t.cpp                                       -*-C++-*-
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

#include "nstd/iostream/ios_base.hpp"
#include "nstd/locale/locale.hpp"
#include "kuhl/test.hpp"

namespace NI = ::nstd;
namespace NL = ::nstd;
namespace NS = ::nstd;
namespace KT = ::kuhl::test;

// ----------------------------------------------------------------------------

namespace {
    struct simple_ios_base
        : NI::ios_base {
        simple_ios_base() noexcept(false): NI::ios_base() {}
    };
}

// ----------------------------------------------------------------------------

static KT::testcase const tests[] = {
    KT::expect_success("class ios_base [ios.base]", [](KT::context& c)->bool{
            NI::ios_base*                ib{0};
            NI::ios_base const*          cib{0};
            NL::locale const*            cl{0};
            NI::ios_base::event_callback cb{0};
            KT::use(ib);
            KT::use(cib);
            KT::use(cl);
            KT::use(cb);

            return KT::assert_declared<NI::ios_base::failure>(c, "ios_base::failure")
                && KT::assert_declared<NI::ios_base::fmtflags>(c, "ios_base::fmtflags")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::boolalpha)>(c, "ios_base::boolalpha")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::dec)>(c, "ios_base::dec")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::fixed)>(c, "ios_base::fixed")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::hex)>(c, "ios_base::hex")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::internal)>(c, "ios_base::internal")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::left)>(c, "ios_base::left")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::oct)>(c, "ios_base::oct")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::right)>(c, "ios_base::right")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::scientific)>(c, "ios_base::scientific")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::showbase)>(c, "ios_base::showbase")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::showpoint)>(c, "ios_base::showpoint")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::showpos)>(c, "ios_base::showpos")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::skipws)>(c, "ios_base::skipws")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::unitbuf)>(c, "ios_base::unitbuf")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::uppercase)>(c, "ios_base::uppercase")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::adjustfield)>(c, "ios_base::adjustfield")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::basefield)>(c, "ios_base::basefield")
                && KT::assert_type<NI::ios_base::fmtflags const, decltype(NI::ios_base::floatfield)>(c, "ios_base::floatfield")
                && KT::assert_declared<NI::ios_base::iostate>(c, "ios_base::iostate")
                && KT::assert_type<NI::ios_base::iostate const, decltype(NI::ios_base::goodbit)>(c, "ios_base::goodbit")
                && KT::assert_type<NI::ios_base::iostate const, decltype(NI::ios_base::failbit)>(c, "ios_base::failbit")
                && KT::assert_type<NI::ios_base::iostate const, decltype(NI::ios_base::badbit)>(c, "ios_base::badbit")
                && KT::assert_type<NI::ios_base::iostate const, decltype(NI::ios_base::eofbit)>(c, "ios_base::eofbit")
                && KT::assert_declared<NI::ios_base::openmode>(c, "ios_base::openmode")
                && KT::assert_type<NI::ios_base::openmode const, decltype(NI::ios_base::app)>(c, "ios_base::app")
                && KT::assert_type<NI::ios_base::openmode const, decltype(NI::ios_base::ate)>(c, "ios_base::ate")
                && KT::assert_type<NI::ios_base::openmode const, decltype(NI::ios_base::binary)>(c, "ios_base::binary")
                && KT::assert_type<NI::ios_base::openmode const, decltype(NI::ios_base::in)>(c, "ios_base::in")
                && KT::assert_type<NI::ios_base::openmode const, decltype(NI::ios_base::out)>(c, "ios_base::out")
                && KT::assert_type<NI::ios_base::openmode const, decltype(NI::ios_base::trunc)>(c, "ios_base::trunc")
                && KT::assert_declared<NI::ios_base::seekdir>(c, "ios_base::seekdir")
                && KT::assert_type<NI::ios_base::seekdir, decltype(NI::ios_base::beg)>(c, "ios_base::beg")
                && KT::assert_type<NI::ios_base::seekdir, decltype(NI::ios_base::cur)>(c, "ios_base::cur")
                && KT::assert_type<NI::ios_base::seekdir, decltype(NI::ios_base::end)>(c, "ios_base::end")
                && KT::assert_declared<NI::ios_base::Init>(c, "ios_base::Init")
                && KT::assert_type<NI::ios_base::fmtflags,
                                   decltype(cib->flags())>(c, "ios_base::flags()")
                && KT::assert_type<NI::ios_base::fmtflags,
                                   decltype(ib->flags(NI::ios_base::dec))>(c, "ios_base::flags(fmtflags)")
                && KT::assert_type<NI::ios_base::fmtflags,
                                   decltype(ib->setf(NI::ios_base::dec))>(c, "ios_base::setf(fmtflags)")
                && KT::assert_type<NI::ios_base::fmtflags,
                                   decltype(ib->setf(NI::ios_base::dec, NI::ios_base::dec))>(c,
                                                                   "ios_base::setf(fmtflags, fmtflags)")
                && KT::assert_type<NI::ios_base::fmtflags,
                                   decltype(ib->unsetf(NI::ios_base::dec))>(c, "ios_base::unsetf(fmtflags)")
                && KT::assert_type<NI::streamsize,
                                   decltype(cib->precision())>(c, "ios_base::precision()")
                && KT::assert_type<NI::streamsize,
                                   decltype(ib->precision(NI::streamsize()))>(c, "ios_base::precision(streamsize)")
                && KT::assert_type<NI::streamsize,
                                   decltype(cib->width())>(c, "ios_base::width()")
                && KT::assert_type<NI::streamsize,
                                   decltype(ib->width(NI::streamsize()))>(c, "ios_base::width(streamsize)")
                && KT::assert_type<NL::locale,
                                   decltype(ib->imbue(*cl))>(c, "ios_base::imbue(locale)")
                && KT::assert_type<NL::locale,
                                   decltype(cib->getloc())>(c, "ios_base::getloc()")
                && KT::assert_type<int, decltype(NI::ios_base::xalloc())>(c, "ios_base::xalloc()")
                && KT::assert_type<long&, decltype(ib->iword(0))>(c, "ios_base::iword()")
                && KT::assert_type<void*&, decltype(ib->pword(0))>(c, "ios_base::pword()")
                && KT::assert_declared<NI::ios_base::event>(c, "ios_base::event")
                && KT::assert_type<NI::ios_base::event,
                                   decltype(NI::ios_base::erase_event)>(c, "ios_base::erase_event")
                && KT::assert_type<NI::ios_base::event,
                                   decltype(NI::ios_base::imbue_event)>(c, "ios_base::imbue_event")
                && KT::assert_type<NI::ios_base::event,
                                   decltype(NI::ios_base::copyfmt_event)>(c, "ios_base::copyfmt_event")
                && KT::assert_type<void(*)(NI::ios_base::event, NI::ios_base&, int),
                                   NI::ios_base::event_callback>(c, "ios_base::event_callback")
                && KT::assert_type<void, decltype(ib->register_callback(cb, 0))>(c, " ios_base::register_callback")
                && KT::assert_type<bool, decltype(NI::ios_base::sync_with_stdio(false))>(c, " ios_base::sync_with_stdio()")
                && KT::assert_false(c, "create/destroy", noexcept(simple_ios_base()))
                ;
        }),
    KT::expect_success("class ios_base::failure [ios::failure]", [](KT::context& c)->bool{
            NI::string const* str{0};
            KT::use(str);
            //-dk:TODO test ctor functionality
            return KT::assert_is_base<NS::system_error, NI::ios_base::failure>(c, "system_error is base of failure")
                && KT::assert_type<NI::ios_base::failure,
                                   decltype(NI::ios_base::failure(*str))>(c, "construct failure with string")
                && KT::assert_type<NI::ios_base::failure,
                                   decltype(NI::ios_base::failure(*str, NI::io_errc::stream))>(c,
                                                                  "construct failure with string and ErrorCode")
                && KT::assert_type<NI::ios_base::failure,
                                   decltype(NI::ios_base::failure("err"))>(c, "construct failure with literal")
                && KT::assert_type<NI::ios_base::failure,
                                   decltype(NI::ios_base::failure("err", NI::io_errc::stream))>(c,
                                                                  "construct failure with literal and ErrorCode")
                ;
        }),
    KT::expect_success("class ios_base::fmtflags [ios::fmtflags]", [](KT::context& c)->bool{
            return KT::assert_type<NI::ios_base::fmtflags,
                                   decltype(NI::ios_base::fmtflags())>(c, "fmtflags type")
                && KT::assert_bitmask<NI::ios_base::fmtflags>(c, "fmtflags is a bitmask type")
                && KT::assert_equal(c, "adjustfield", NI::ios_base::adjustfield,
                                    NI::ios_base::left | NI::ios_base::internal | NI::ios_base::right)
                && KT::assert_equal(c, "basefield", NI::ios_base::basefield,
                                    NI::ios_base::dec | NI::ios_base::hex | NI::ios_base::oct)
                && KT::assert_equal(c, "floatfield", NI::ios_base::floatfield,
                                    NI::ios_base::fixed | NI::ios_base::scientific)
                ;
        }),
    KT::expect_success("class ios_base::iostate [ios::iostate]", [](KT::context& c)->bool{
            return KT::assert_type<NI::ios_base::iostate,
                                   decltype(NI::ios_base::iostate())>(c, "iostate type")
                && KT::assert_bitmask<NI::ios_base::iostate>(c, "iostate is a bitmask type")
                && KT::assert_equal(c, "goodbit", NI::ios_base::goodbit, NI::ios_base::iostate())
                ;
        }),
    KT::expect_success("class ios_base::openmode [ios::openmode]", [](KT::context& c)->bool{
            return KT::assert_type<NI::ios_base::openmode,
                                   decltype(NI::ios_base::openmode())>(c, "openmode type")
                && KT::assert_bitmask<NI::ios_base::openmode>(c, "openmode is a bitmask type")
                ;
        }),
    KT::expect_success("class ios_base::seekdir [ios::seekdir]", [](KT::context& c)->bool{
            return KT::assert_type<NI::ios_base::seekdir, decltype(NI::ios_base::beg)>(c, "ios_base::beg")
                && KT::assert_type<NI::ios_base::seekdir, decltype(NI::ios_base::cur)>(c, "ios_base::cur")
                && KT::assert_type<NI::ios_base::seekdir, decltype(NI::ios_base::end)>(c, "ios_base::end")
                ;
        }),
    KT::expect_success("class ios_base::Init [ios::Init]", [](KT::context& c)->bool{
            NI::ios_base::Init init;
            KT::use(init);
            // There is no testable semantics defined.
            return KT::assert_declared<NI::ios_base::Init>(c, "ios_base::Init")
                ;
        }),
    KT::expect_success("ios_base fmtflags state [fmtflags.state]", [](KT::context& c) -> bool {
            simple_ios_base ios;
            return KT::assert_equal(c, "flags() initial value",
                                    NI::ios_base::fmtflags{},
                                    ios.flags())
                && KT::assert_equal(c, "flags(f) result (1)",
                                    NI::ios_base::fmtflags{},
                                    ios.flags(NI::ios_base::unitbuf))
                && KT::assert_equal(c, "flags(f) effect (1)",
                                    NI::ios_base::unitbuf,
                                    ios.flags())
                && KT::assert_equal(c, "flags(f) result (2)",
                                    NI::ios_base::unitbuf,
                                    ios.flags(NI::ios_base::fixed))
                && KT::assert_equal(c, "flags(f) effect (2)",
                                    NI::ios_base::fixed,
                                    ios.flags())
                && KT::assert_equal(c, "setf(f) result (1)",
                                    NI::ios_base::fixed,
                                    ios.setf(NI::ios_base::showpos))
                && KT::assert_equal(c, "setf(f) effect (1)",
                                    NI::ios_base::fixed | NI::ios_base::showpos,
                                    ios.flags())
                && KT::assert_equal(c, "setf(f) result (2)",
                                    NI::ios_base::fixed | NI::ios_base::showpos,
                                    ios.setf(NI::ios_base::dec))
                && KT::assert_equal(c, "setf(f) effect (2)",
                                    NI::ios_base::fixed | NI::ios_base::showpos | NI::ios_base::dec,
                                    ios.flags())
                && KT::assert_equal(c, "setf(f, m) result (1)",
                                    NI::ios_base::fixed | NI::ios_base::showpos | NI::ios_base::dec,
                                    ios.setf(NI::ios_base::hex | NI::ios_base::uppercase,
                                             NI::ios_base::basefield | NI::ios_base::showpos))
                && KT::assert_equal(c, "setf(f, m) effect (1)",
                                    NI::ios_base::fixed | NI::ios_base::hex,
                                    ios.flags())
                && KT::assert_equal(c, "unsetf(f) result (1)",
                                    NI::ios_base::fixed | NI::ios_base::hex,
                                    ios.unsetf(NI::ios_base::fixed | NI::ios_base::uppercase))
                && KT::assert_equal(c, "setf(f, m) effect (1)",
                                    NI::ios_base::hex,
                                    ios.flags())
                && KT::assert_equal(c, "initial precision", 0, ios.precision())
                && KT::assert_equal(c, "precision(p) result", 0, ios.precision(8))
                && KT::assert_equal(c, "precision(p) effect", 8, ios.precision())
                && KT::assert_equal(c, "initial width", 0, ios.width())
                && KT::assert_equal(c, "width(w) result", 0, ios.width(8))
                && KT::assert_equal(c, "width(w) effect", 8, ios.width())
                ;
        }),
};

int main(int ac, char* av[])
{
    return KT::run_tests("iostream/ios_base", ac, av, ::tests);
}
