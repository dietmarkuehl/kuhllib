// hazptr.t.cpp                                                       -*-C++-*-
// ----------------------------------------------------------------------------

#include "hazptr.hpp"

namespace NE = nstd::experimental;
namespace NP = nstd::p0566r5;

// ----------------------------------------------------------------------------

struct Node
    : NE::hazptr_obj_base<Node> {
};

// ----------------------------------------------------------------------------

int main()
{
    NE::hazptr_domain& domain = NE::default_hazptr_domain();
    (void)domain;

    (new Node())->retire();
    (new Node())->retire(NE::default_hazptr_domain());

    auto holder = NP::make_hazptr();
}
