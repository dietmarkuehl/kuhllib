// nstd/locale/locale.cpp                                             -*-C++-*-
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
#include "nstd/atomics/atomic.hpp"

namespace NL = ::nstd;
namespace NA = ::nstd;

// ----------------------------------------------------------------------------

constexpr NL::locale::category NL::locale::none;
constexpr NL::locale::category NL::locale::collate;
constexpr NL::locale::category NL::locale::ctype;
constexpr NL::locale::category NL::locale::monetary;
constexpr NL::locale::category NL::locale::numeric;
constexpr NL::locale::category NL::locale::time;
constexpr NL::locale::category NL::locale::messages;
constexpr NL::locale::category NL::locale::all;

// ----------------------------------------------------------------------------

NL::locale::facet::facet(NL::size_t count)
    : count(count) {
}

NL::locale::facet::~facet() {
}

auto NL::locale::facet::increment()
    -> void {
    ++this->count;
}
auto NL::locale::facet::decrement()
    -> bool {
    return !--this->count;
}

// ----------------------------------------------------------------------------

NL::locale::id::id() {
}

// ----------------------------------------------------------------------------

struct NL::locale::rep {
    mutable NA::atomic<NL::size_t> count_;
    constexpr rep(): count_(1u) {}
    rep(rep const&) = delete;
    auto operator= (rep const&) -> void = delete;
};

struct NL::locale::global_rep {
    mutable NA::atomic<NL::locale::rep const*> rep;
    constexpr global_rep(NL::locale::rep const* rep): rep(rep) {}
};

struct NL::locale::instance {
    static constexpr rep                    classic_rep{};
    static constexpr NL::locale::global_rep global{&classic_rep};
    static const NL::locale                 classic_locale;
};

// ----------------------------------------------------------------------------

constexpr NL::locale::rep        NL::locale::instance::classic_rep;
constexpr NL::locale::global_rep NL::locale::instance::global;
const NL::locale NL::locale::instance::classic_locale{&NL::locale::instance::classic_rep};

// ----------------------------------------------------------------------------

NL::locale::locale(NL::locale::rep const* rep) noexcept(true)
    : rep_(rep) {
    ++this->rep_->count_;
}
NL::locale::locale() noexcept(true)
    : rep_(NL::locale::instance::global.rep.load()) {
    ++this->rep_->count_;
}
NL::locale::locale(NL::locale const& other) noexcept(true)
    : rep_(other.rep_) {
    ++this->rep_->count_;
}
NL::locale::locale(NL::locale const&, NL::locale::facet*, NL::locale::id const*)
    : rep_(new rep{}) {
}

NL::locale::~locale() {
    if (0u == --this->rep_->count_) {
        delete this->rep_;
    }
}

// ----------------------------------------------------------------------------

auto NL::locale::operator== (NL::locale const& other) const
    -> bool {
    return this->rep_ == other.rep_;
}

auto NL::locale::operator!= (NL::locale const& other) const
    -> bool {
    // return !this->operator==(other);
    return this->rep_ != other.rep_;
}

// ----------------------------------------------------------------------------

auto NL::locale::global(NL::locale const& loc)
    -> NL::locale {
    NL::locale::rep const* rc{NL::locale::instance::global.rep.exchange(loc.rep_)};
    return NL::locale(rc);
}

auto NL::locale::classic()
    -> NL::locale const& {
    static NL::locale rc; //-dk:TODO use a statically initialized version
    return rc;
}

