// hazptr.cpp                                                         -*-C++-*-
// ----------------------------------------------------------------------------

#include "hazptr.hpp"

namespace NE = nstd::experimental;
namespace NP = nstd::p0566r5;

// ----------------------------------------------------------------------------

NE::hazptr_domain::hazptr_domain(std::pmr::polymorphic_allocator<std::byte> alloc)
    : alloc_(alloc) {
}

NE::hazptr_domain::~hazptr_domain() {
}

// ----------------------------------------------------------------------------

NE::hazptr_domain& NE::default_hazptr_domain() noexcept {
    static NE::hazptr_domain rc;
    return rc;
}

// ----------------------------------------------------------------------------

NP::hazptr_holder::hazptr_holder() noexcept {
}

NP::hazptr_holder::hazptr_holder(hazptr_holder&&) noexcept {
}

NP::hazptr_holder::~hazptr_holder() {
}

NP::hazptr_holder&
NP::hazptr_holder::operator= (hazptr_holder&&) noexcept {
    return *this;
}

bool NP::hazptr_holder::empty() const noexcept {
    return true;
}

void NP::hazptr_holder::reset_protected(std::nullptr_t) noexcept {
}

void NP::hazptr_holder::swap(hazptr_holder&) noexcept {
}

// ----------------------------------------------------------------------------

NP::hazptr_holder NP::make_hazptr() {
    return hazptr_holder();
}
